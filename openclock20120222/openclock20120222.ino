// simple clock using 16x32 bicolor LED matrix, Chronodot, and Arduino
// craig bonsignore
// 7 may 2012 . change digits to red at night . move pin 4 to 0 . move pin 5 to 1
// 22 may 2012 . change from bold to normal font during sleeping hours
// 16 june 2012 . map touchscreen press to pixels
// 30 june 2012 . recalibrate for new enclosure
// 17 nov 2012 . rename to openclock.ino and manage revisions using github
// 24 nov 2012 . move contributed libraries to sketch folder, per Arduino 1.0.2 guidance
//             + major refactoring . created currentTime subroutine 
//             + all characters now defined by global variables
//             + all characters drawn to screen in one place @ end of main loop()
// 02 dec 2012 . more refactoring . debugging new touch detection
// 03 dec 2012 . fix hour bug introduced yesterday 
// 03 dec 2012 . touch detection virtual buttons: hr+/- min+/- ampm message
// 04 dec 2012 . fix bug: midnight renders as 0:00
// 09 dec 2012 . addeded disfunctional proto-alarm-set-mode
// 16 dec 2012 . replaced <Chronodot.h> with Arduino Time Library
//               to-do: improve touch/button management, time variables,
//               alarm variables, and menu logic. try to use similar logic
//               and code as alphaclockfive
// 04 feb 2013 . reworked logic in detectTouch and menuMode to improve alarm setting
//               now more functional, but not quite right. menu timer doesn't reset.
//               improved comments in a few places

#include <Wire.h> // I2C library for Chronodot
#include <ht1632c.h> // http://code.google.com/p/ht1632c/
#include <TouchScreen.h> // https://github.com/adafruit/Touch-Screen-Library
#include <SimpleTimer.h> // http://arduino.cc/playground/Code/SimpleTimer

#include <Time.h>       // The Arduino Time library, http://www.arduino.cc/playground/Code/Time
#include <DS1307RTC.h>  // For optional RTC module. (This library included with the Arduino Time library)


//====================================================================================
// message from redlegoman
// this following comment from you interested me too, so I did a little research.
// go here http://www.arduino.cc/en/Reference/PortManipulation for an explanation  

// initialize the 16x32 display
// for reasons that I do not understand, moving any of these pins to 8 or higher fails


ht1632c ledMatrix = ht1632c(&PORTD, 7, 6, 0, 1, GEOM_32x16, 2); 
//====================================================================================    
// initialize the Chronodot real time clock
//Chronodot RTC; 

// For this orientation, the N010-0554-T048 Fujitsu touch panel is oriented with the touch film side up
// and the ribbon to the left. The four wires, from top to bottom, correspond to XM, YP, XP, YM
#define XM A0  // black // must be an analog pin, use "An" notation!
#define YP A1  // green // must be an analog pin, use "An" notation!
#define XP A2   // yellow // can be a digital pin
#define YM A3   // blue // can be a digital pin

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// 90 ohms between XM and XP
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 90);

// new millis() based timer variables
unsigned long menuStartMillis;
unsigned long touchStartMillis;

// initialize SimpleTimer, used for menu timeout
// SimpleTimer timer;
// int menuTimer; // Timer ID for menu
// int touchTimer; // Timer ID for touch event
//int menuItem = 0; // 0 if in normal mode, 1+ for menu choices
int menuTimeout = 5000; // 5 second timeout for menu
int touchTimeout = 100; // 0.1 second timeout for touch event

// initialize array to hold [x, y, prior_x, prior_y] data for touch location
int dot[4] = {
  0,0,0,0}; 

// initialize array to hold mapped coordinates for touch location
// pixel[0,1] = [x,y] pixel ... x=0-31, y=0-15
int pixel[2];

// define "button" numbers

#define TOUCH_NONE        0
#define TOUCH_HOURPLUS    1
#define TOUCH_HOURMINUS   2
#define TOUCH_MINUTEPLUS  3
#define TOUCH_MINUTEMINUS 4
#define TOUCH_AMPM        5
#define TOUCH_MESSAGE     6
#define TOUCH_WTF         7

int touchState = TOUCH_NONE;
boolean touchNew = false;

// define menu states

#define MENU_NONE = 0;
#define MENU_ALARM = 1;
#define MENU_TIME = 2;
#define MENU_EXIT = 3;

int menuState = 0;  // 0 if in normal mode, 1+ for menu choices
boolean menuNew = false; 

boolean touchActive = false; // true if screen has been touched in last touchTimeout (0.5) seconds, else false
boolean menuActive = false; // true if menu mode is active, else false

// initialize colors    
int digit_color = GREEN;
int message_color = RED;
int ampm_color = GREEN;

// initialize values for each character on display
//   ---------------------------------------------------------
//  |                                                         |
//  |    dig[1]      dig[2]      :      dig[3]      dig[4]    |
//  |                                                         |
//  | msg[1] msg[2] msg[3] msg[4] msg[5] msg[6] msg[7] msg[8] |
//   ---------------------------------------------------------

char dig[6]  = "01234";
char msg[10] = "012345678";

int nowHour12 = 0;
int nowHour24 = 0;
int nowMinute = 0;
int nowPM = 0;

int dispHour12 = 0;
int dispHour24 = 0;
int dispMinute = 0;
int dispPM = 0;

int alarmHour12 = 6;
int alarmHour24 = 6;
int alarmMinute = 0;
int alarmPM = 0;

//////////////////////////////////////////////////////////////////////////////////
// setup()
// set the pins, initialize the display, etc.

void setup () {

  pinMode(7, OUTPUT); // initialize ht1632c pin
  pinMode(6, OUTPUT); // initialize ht1632c pin
  pinMode(0, OUTPUT); // initialize ht1632c pin
  pinMode(1, OUTPUT); // initialize ht1632c pin

  Wire.begin(); // initialize I2C
  setSyncProvider(RTC.get);   // the function to get the time from the RTC

  ledMatrix.clear(); // clear the display
  ledMatrix.pwm(5); // set brightness (max=15)

  // the following line sets the RTC to the date & time this sketch was compiled
  //RTC.adjust(DateTime(__DATE__, __TIME__));

}

//////////////////////////////////////////////////////////////////////////////////
// loop()
// main program loop

void loop () {

  //timer.run(); // activate SimpleTimer                             
  currentTime(); // read current time and set global variables for hour, minute, am/pm         
  detectTouch(); // detect touch state
  menuMode(); // change display state according to current menu mode    
  setStyle(dispHour24, dispMinute); // set color and brightness based on displayed time   
  drawScreen(dispHour12, dispMinute, dispPM, menuActive, msg); // draw the screen             
  delay(100); // wait for 100ms
}

//////////////////////////////////////////////////////////////////////////////////
// currentTime()
// read the current time from the RTC
// set global variables: nowHour24, nowHour12, nowPM, nowMinute
// set dispHour24, dispHour12, dispMinute, dispPM to current time
// (if in menu mode, disp* variables will be overwritten before going to screen)

void currentTime () {

  // read the current time    
  setSyncProvider(RTC.get);   // the function to get the time from the RTC

  // set global variables for current time 
  nowHour24 = hour();
  if (nowHour24 > 12){
    nowHour12 = nowHour24 - 12;
    nowPM = 1;
  }
  else if (nowHour24 == 0){
    nowHour12 = 12;
    nowPM = 0;
  }
  else{
    nowHour12 = nowHour24;
    nowPM = 0;
  }

  if (nowHour24 == 12) nowPM = 1;   

  nowMinute = minute();

  dispHour24 = nowHour24;
  dispHour12 = nowHour12;
  dispMinute = nowMinute;
  dispPM = nowPM;
}

//////////////////////////////////////////////////////////////////////////////////
// setStyle()
// set color, brightness and boldness based on hour and minute passed to function

void setStyle(int hour24, int minute) {

  // adjust the brightness based on the time of day
  if (hour24 <= 5) ledMatrix.pwm(1); /// 12:00a to 5:59a minimum brightness
  else if (hour24 <= 6) ledMatrix.pwm(5); // 6:00a to 6:59a brighter
  else if (hour24 <= 19) ledMatrix.pwm(15); // 7:00a to 7:59p max brightness
  else if (hour24 <= 21) ledMatrix.pwm(5); // 8:00p to 9:59p dimmer
  else if (hour24 <= 24) ledMatrix.pwm(1); // 10:00p to 11:59p minimum brightness

  // adjust the color based on the time of day
  if (hour24 <= 6) digit_color = RED; // 12:00a to 6:59a red digits
  else if (hour24 <= 19) digit_color = GREEN; // 7:00a to 7:59p green digits 
  else if (hour24 <= 24) digit_color = RED; // 8:00p to 11:59p red digits

  // set am/pm color .. this should probably = digit_color if there is an alarm display active in message area
  //ampm_color = digit_color;
  ampm_color = ORANGE;

  if (digit_color == GREEN) message_color = RED;
  if (digit_color == RED) message_color = GREEN;

  // adjust the boldness based on the time of day. normal font = less pixels lit = darker for sleeping
  if (hour24 <= 5) ledMatrix.setfont(FONT_7x14); // 12:00a to 5:59a use a 7x14 normal font
  else if (hour24 <= 21) ledMatrix.setfont(FONT_7x14B); // 6:00a to 9:59p use a bold font for improved distance visibility
  else if (hour24 <= 24) ledMatrix.setfont(FONT_7x14); // 10:00p to 11:59p use a 7x14 normal font

}

//////////////////////////////////////////////////////////////////////////////////
// detectTouch()
// determine if touchscreen pressure exceeds threshold
// if no, and touch timer has not elapsed, keep touch state the same
// if no, and touch timer has elapsed, set touch state to "none"
// if yes, and touch timer has not elapsed, keep touch state the same
// if yes, and touch timer is not active, this is a new touch event, so...
//    set the touch state ("activate button") based on screen region touched


int detectTouch(){

  //touchNew = false;            // this isn't a new touch unless we change this flag below 
  int lastTouchState = touchState; // save the previous touch state here
  
  if (timer.isEnabled(touchTimer)){ // touch timer is active, so don't change anything
    touchState = lastTouchState;    // just set touch value to previous value
    return touchState;              // and get outta here
  }
  else{                          // otherwise, the timer is disabled
    if (touchActive == true){    // if the active flag is "true", we just timed out, so 
      touchActive = false;       // set the touchActive flag to false
      touchState = TOUCH_NONE;   // set the touch state to none
      return touchState;         // and get outta here
    }
    if (touchActive == false){ // timer isn't active, so there's probably nothing happening here
      touchState = TOUCH_NONE; // so set the touch state to none
    }                          // but don't return yet, because we have to check the if the
  }                            // touchscreen is being touched
    
  // commented next 3 lines feb4
  //boolean endTouch = false;
  //boolean activeTouch = false;
  //boolean activeTimer = false;

  // read X,Y coordinates and pressure
  Point p = ts.getPoint();
  
  if (p.z < ts.pressureThreshhold ){  // touchscreen isn't being touched
    touchState = lastTouchState;      // so don't change the touch state
    return touchState;                // and return 
  }
  
  // if we have made it this far...
  // the touch timer is not active, and the screen is being touched
  // so this is a new touch, and we have to figure out what "button" was pushed
  
  touchTimer = timer.setTimeout(touchTimeout, exitTouch);   // start up the touch timer
  touchActive = true;                                       // touch event is active
  touchNew = true;                                          // this is a new touch
  
  //timer.restartTimer(menuTimer);      // this is probably not the right place
                                                            // but this should reset the menu timer
                                                            // any time the screen is touched

  // define variables for screen regions
  int verticalRegion = 0;
  int horizontalRegion = 0;

  // map coordinates to pixel location
  pixel[0] = map(p.x,75,930,0,31);           // x calibration by trial-and-error
  pixel[1] = map(p.y,300,770,0,15);          // y calibration by trial-and-error

  int px = pixel[0];
  int py = pixel[1];

  // map pixel location to screen region
  if      (px >=0  && px <=8 )  horizontalRegion = 1;        // x pixel 0-8   = 1st digit of hour
  else if (px >=9  && px <=17)  horizontalRegion = 2;        // x pixel 9-17  = 2nd digit of hour
  else if (px >=18 && px <=25)  horizontalRegion = 3;        // x pixel 18-25 = 1st digit of minute
  else if (px >=26 && px <=33)  horizontalRegion = 4;        // x pixel 26-33 = 2nd digit of minute
  else                          horizontalRegion = 0;

  if      (py >=0  && py <=5 )  verticalRegion = 1;          // y pixel 0-5   = top of time digits
  else if (py >=6  && py <=10)  verticalRegion = 2;          // y pixel 6-10  = bottom of time digits
  else if (py >=11 && py <=16)  verticalRegion = 3;          // y pixel 11-16 = message / ampm area
  else                          verticalRegion = 0;

  // define new virtual button states based on region touched

  if (verticalRegion == 1){
    if (horizontalRegion == 1) touchState = TOUCH_HOURPLUS;
    if (horizontalRegion == 2) touchState = TOUCH_HOURPLUS;
    if (horizontalRegion == 3) touchState = TOUCH_MINUTEPLUS;
    if (horizontalRegion == 4) touchState = TOUCH_MINUTEPLUS;
  }
  else if(verticalRegion == 2){
    if (horizontalRegion == 1) touchState = TOUCH_HOURMINUS;
    if (horizontalRegion == 2) touchState = TOUCH_HOURMINUS;
    if (horizontalRegion == 3) touchState = TOUCH_MINUTEMINUS;
    if (horizontalRegion == 4) touchState = TOUCH_MINUTEMINUS;
  }
  else if(verticalRegion == 3){
    if (horizontalRegion == 1) touchState = TOUCH_MESSAGE;
    if (horizontalRegion == 2) touchState = TOUCH_MESSAGE;
    if (horizontalRegion == 3) touchState = TOUCH_MESSAGE;
    if (horizontalRegion == 4) touchState = TOUCH_AMPM;
  }
  else if(verticalRegion == 0 || horizontalRegion == 0){
    touchState = 0;
    touchState = TOUCH_WTF;
  }
  else{
    touchState = 0;
  }
  
  return touchState;

}

//////////////////////////////////////////////////////////////////////////////////
// drawScreen()
// draw the screen, using the digits and characters passed to the function

void drawScreen( int hour12, int minute, int pm, boolean msgState, char message[10]){    

  char buf[5];
  char ampm = 'A';
  char letter_m = 'M';

  dig[0] = '1'; // set flag for valid time

  // clear any prior touch dots
  ledMatrix.plot(dot[2],dot[3],BLACK); // clear any prior touch dots

  // clear the message area        
  ledMatrix.rect(0,11,22,15,BLACK);
  ledMatrix.rect(1,10,21,14,BLACK);
  ledMatrix.rect(2,9,20,13,BLACK);
  ledMatrix.rect(3,8,19,12,BLACK);


  // set the hours digits
  itoa(hour12, buf, 10);
  if (hour12 < 10){
    dig[1] = '\0';
    dig[2] = buf[0];
  }
  else{
    dig[1] = buf[0];
    dig[2] = buf[1];
  }

  // set the minutes digits
  itoa(minute, buf, 10);
  if (minute < 10){
    dig[3] = '0';
    dig[4] = buf[0];
  }
  else{
    dig[3] = buf[0];
    dig[4] = buf[1];
  }

  // set the AM/PM state
  if ( pm == 0 ) ampm = 'A';
  else ampm = 'P';

  msg[7] = ampm;
  msg[8] = letter_m;                       


  // fist digit of hour
  if (dig[1] == '1') ledMatrix.putchar(2,-2,'1',digit_color,6);
  else ledMatrix.putchar(2,-2,'1',BLACK,6); // erase the "1" if the hour is 1-9   

  ledMatrix.putchar(9,-2,dig[2],digit_color,6); // second digit of hour
  ledMatrix.plot(16,3,ORANGE); // hour:min colon, top
  ledMatrix.plot(16,6,ORANGE); // hour:min colon, bottom
  ledMatrix.putchar(18,-2,dig[3],digit_color,7); // first digit of minute
  ledMatrix.putchar(25,-2,dig[4],digit_color,7); // second digit of minute

  // draw the characters of the message area if a message is active
  // otherwise, black out the message area
  ledMatrix.setfont(FONT_4x6);
  if (msgState && msg[1] != '\0') ledMatrix.putchar(0,11,msg[1],message_color);
  if (msgState && msg[2] != '\0') ledMatrix.putchar(4,11,msg[2],message_color);
  if (msgState && msg[3] != '\0') ledMatrix.putchar(8,11,msg[3],message_color);
  if (msgState && msg[4] != '\0') ledMatrix.putchar(12,11,msg[4],message_color);
  if (msgState && msg[5] != '\0') ledMatrix.putchar(16,11,msg[5],message_color);
  if (msgState && msg[6] != '\0') ledMatrix.putchar(20,11,msg[6],message_color);

  // draw the AM or PM
  ledMatrix.setfont(FONT_4x6);
  ledMatrix.putchar(24,11,msg[7],ampm_color,6);
  ledMatrix.putchar(28,11,msg[8],ampm_color,6);


  // send the characters to the display, and draw the screen
  ledMatrix.sendframe();

}

//////////////////////////////////////////////////////////////////////////////////
// menuMode()
// wonky menu logic is presently here

void menuMode(){

  // decide what to do based on touch state, and get outta here
  
  if (menuNew == false && touchNew == true){
    menuActive = true;
    menuTimer = timer.setTimeout(menuTimeout, exitMenu);
  }
  
  if (touchNew == true){
    timer.restartTimer(menuTimer);
  }
    
  if (menuActive == false){
    menuActive = false; //unnecessary
    menuState = TOUCH_NONE; //unnecessary
    return;
  }



  msg[1] = 'A';
  msg[2] = 'L';
  msg[3] = 'A';
  msg[4] = 'R';
  msg[5] = 'M';
  msg[6] = '\0';

  if (touchState > 0 && touchNew == true){

    //menuTimer = timer.setTimeout(menuTimeout, exitMenu);

    switch (touchState){

    case TOUCH_HOURPLUS:
      if (alarmHour12 < 12) alarmHour12++;
      else if (alarmHour12 == 12){
        alarmHour12 = 1;
        if (alarmPM == 0) alarmPM = 1;
        else if (alarmPM != 0) alarmPM = 0;
      }
      break;

    case TOUCH_HOURMINUS:
      if (alarmHour12 > 1) alarmHour12--;
      else if (alarmHour12 == 1){
        alarmHour12 = 12;
        if (alarmPM == 0) alarmPM = 1;
        else if (alarmPM != 0) alarmPM = 0;
      }
      break;

    case TOUCH_MINUTEPLUS:
      if (alarmMinute < 59) alarmMinute++;
      else if (alarmMinute == 59){
        alarmMinute = 0;
      }
      break;

    case TOUCH_MINUTEMINUS:
      if (alarmMinute > 0) alarmMinute--;
      else if (alarmMinute == 0){
        alarmMinute = 59;
      }
      break;

    case TOUCH_AMPM:
      if (alarmPM == 0) alarmPM = 1;
      else if (alarmPM != 0) alarmPM = 0;
      break;

    case 0:
      break;  
    }
  
  touchNew = false; // now that an action has been taken, clear the new touch flag
  timer.restartTimer(menuTimer); // and restart the menu timer
    
  }


  dispHour12 = alarmHour12;
  dispMinute = alarmMinute;
  dispPM = alarmPM;           

  return;
}

//////////////////////////////////////////////////////////////////////////////////
// exitTouch()
// called when touch timer elapses

void exitTouch(){
  timer.disable(touchTimer);
  touchActive = false;        // i don't think these are necessary here
  touchNew = false;
  touchState = TOUCH_NONE;
}

//////////////////////////////////////////////////////////////////////////////////
// exitMenu()
// called when menu timer elapses

void exitMenu(){
  timer.disable(menuTimer);
  menuActive = false;
  menuState = 0;
  touchState = 0;
  msg[1] = '\0';
  msg[2] = '\0';
  msg[3] = '\0';
  msg[4] = '\0';
  msg[5] = '\0';
  msg[6] = '\0';
}


////////
// junk code here, in case i want to use it for something later

  // set boolean variables according to state of touch, timer, etc.

  // if (lastTouchState == 0 && touchState > 0)               touchNew = true;
  // else if (touchState > 0 && touchState != lastTouchState) touchNew = true;
  // else if (touchState == lastTouchState)                   touchNew = false;
  // else                                                     touchNew = false;
  // if (touchState == 0 && lastTouchState > 0)               endTouch = true;
  // if (p.z > ts.pressureThreshhold ){
  //  touchActive = true;
  //}
  //else{
  //  touchActive = false;
  //  touchState = 0;
  //}
  //else                                                     activeTouch = false;
  //if (timer.isEnabled(touchTimer) )                        activeTimer = true;
  //else                                                     activeTimer = false;

  // decide what to do based on touch state, and get outta here

  /* this is all screwed up. I think touchState is never getting back to 0.
   
   
   
   if (activeTouch == false || touchState == TOUCH_WTF){
   if (touchState == TOUCH_WTF) touchState = 0;
   return 0;
   }
   
   if (activeTimer == false && activeTouch == true){
   touchTimer = timer.setTimeout(touchTimeout, exitTouch);
   touchActive = true;
   tempFunction(touchState); // temporary test of virtual buttons
   return touchState;
   }
   
   else if (activeTimer == true && activeTouch == false){
   touchState = lastTouchState;
   touchActive = true;
   tempFunction(touchState); // temporary test of virtual buttons
   return touchState;
   }
   
   
   else{
   touchActive = false;
   touchState = 0;
   tempFunction(touchState); // temporary test of virtual buttons
   return touchState;
   }
   
   */



  /*** this needs to be relocated
   * // draw the touch dot, if the menu is active
   * // and put the X,Y location of the plotted dot into dot[2], dot[3] so we know what to erase next time
   * if (p.z > ts.pressureThreshhold){
   * ledMatrix.plot(dot[0],dot[1],message_color);
   * dot[2] = dot[0];
   * dot[3] = dot[1];
   * }
   ***/


