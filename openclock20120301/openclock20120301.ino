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
// 02 dec 2012 . more refactoring . debugging new tap detection
// 03 dec 2012 . fix hour bug introduced yesterday 
// 03 dec 2012 . tap detection virtual buttons: hr+/- min+/- ampm message
// 04 dec 2012 . fix bug: midnight renders as 0:00
// 09 dec 2012 . addeded disfunctional proto-alarm-set-mode
// 16 dec 2012 . replaced <Chronodot.h> with Arduino Time Library
//               to-do: improve tap/button management, time variables,
//               alarm variables, and menu logic. try to use similar logic
//               and code as alphaclockfive
// 04 feb 2013 . reworked logic in detectTap and menuMode to improve alarm setting
//               now more functional, but not quite right. menu timer doesn't reset.
//               improved comments in a few places
// 02 mar 2013 . major rewrite for menu/tap timers

#include <Wire.h> // I2C library for Chronodot
#include <ht1632c.h> // http://code.google.com/p/ht1632c/
#include <TouchScreen.h> // https://github.com/adafruit/Touch-Screen-Library
//#include <SimpleTimer.h> // http://arduino.cc/playground/Code/SimpleTimer

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

// For this orientation, the N010-0554-T048 Fujitsu tap panel is oriented with the tap film side up
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
unsigned long tapStartMillis;

// initialize SimpleTimer, used for menu timeout
// SimpleTimer timer;
// int menuTimer; // Timer ID for menu
// int tapTimer; // Timer ID for tap event
//int menuItem = 0; // 0 if in normal mode, 1+ for menu choices
int menuTimeout = 5000; // 5 second timeout for menu
int tapTimeout = 100; // 0.1 second timeout for tap event

// initialize array to hold [x, y, prior_x, prior_y] data for tap location
int dot[4] = {
  0,0,0,0}; 

// initialize array to hold mapped coordinates for tap location
// pixel[0,1] = [x,y] pixel ... x=0-31, y=0-15
int pixel[2];

// define tapZone "button" numbers

#define TAP_NONE        0
#define TAP_HOURPLUS    1
#define TAP_HOURMINUS   2
#define TAP_MINUTEPLUS  3
#define TAP_MINUTEMINUS 4
#define TAP_AMPM        5
#define TAP_MESSAGE     6
#define TAP_WTF         7

int tapZone = TAP_NONE;
boolean tapNew = false;

boolean alarmState = false;

// define display states

#define STATE_DISP_TIME  0
#define STATE_DISP_DATE  1
#define STATE_ALARM_INIT  2
#define STATE_ALARM_SET  3
#define STATE_ALARM_COMMIT  4
#define STATE_ALARM_OFF  5
#define STATE_ALARM_ON  6
#define STATE_TIME_INIT  7
#define STATE_TIME_SET  8
#define STATE_YEAR_INIT  9
#define STATE_YEAR_SET  10
#define STATE_YEAR_COMMIT  11
#define STATE_MONTH_INIT  12
#define STATE_MONTH_SET  13
#define STATE_MONTH_COMMIT  14
#define STATE_DAY_INIT  15
#define STATE_DAY_SET  16
#define STATE_DAY_COMMIT  17

int displayState = 0;  // 0 if in normal mode, 1+ for menu choices
boolean menuNew = false; 

boolean tapActive = false; // true if screen has been taped in last tapTimeout (0.5) seconds, else false
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

  currentTime(); // read current time and set global variables for hour, minute, am/pm         
  detectTap(); // detect tap state
  doActions(); // act upon new tap
  setDisplay(); // change display state according to current menu mode    
  setStyle(dispHour24, dispMinute); // set color and brightness based on displayed time   
  renderScreen(dispHour12, dispMinute, dispPM, menuActive, msg); // draw the screen             
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
// detectTap()
// determine if touchscreen pressure exceeds threshold
// if no, and tap timer has not elapsed, keep tap state the same
// if no, and tap timer has elapsed, set tap state to "none"
// if yes, and tap timer has not elapsed, keep tap state the same
// if yes, and tap timer is not active, this is a new tap event, so...
//    set the tap state ("activate button") based on screen region taped


int detectTap(){

  int lastTapZone = tapZone;

  // if a tap is active, check the tap timer.
  // if the timer hasn't elapsed, don't bother to check for a new tap (debounce)
  // if the timer has elapsed, reset and return

  if (tapActive == true){
    if ((millis() - tapStartMillis) < tapTimeout){
      return lastTapZone;
    }
    else{
      tapActive = false;
      return TAP_NONE;
    }
  }

  // if we made it this far, we are ready to check for a new tap

  // read X,Y coordinates and pressure
  Point p = ts.getPoint();

  if (p.z < ts.pressureThreshhold ){  // touchscreen isn't being taped
    return lastTapZone;                // so don't change anything, and return 
  }

  // if we have made it this far...
  // the tap timer is not active, and the screen is being taped
  // so this is a new tap, and we have to figure out what "button" was pushed

  tapActive = true;                                       // tap event is active
  tapNew = true;                                          // this is a new tap

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

  // define new virtual button states based on region taped

  if (verticalRegion == 1){
    if (horizontalRegion == 1) tapZone = TAP_HOURPLUS;
    if (horizontalRegion == 2) tapZone = TAP_HOURPLUS;
    if (horizontalRegion == 3) tapZone = TAP_MINUTEPLUS;
    if (horizontalRegion == 4) tapZone = TAP_MINUTEPLUS;
  }
  else if(verticalRegion == 2){
    if (horizontalRegion == 1) tapZone = TAP_HOURMINUS;
    if (horizontalRegion == 2) tapZone = TAP_HOURMINUS;
    if (horizontalRegion == 3) tapZone = TAP_MINUTEMINUS;
    if (horizontalRegion == 4) tapZone = TAP_MINUTEMINUS;
  }
  else if(verticalRegion == 3){
    if (horizontalRegion == 1) tapZone = TAP_MESSAGE;
    if (horizontalRegion == 2) tapZone = TAP_MESSAGE;
    if (horizontalRegion == 3) tapZone = TAP_MESSAGE;
    if (horizontalRegion == 4) tapZone = TAP_AMPM;
  }
  else if(verticalRegion == 0 || horizontalRegion == 0){
    tapZone = 0;
    tapZone = TAP_WTF;
  }
  else{
    tapZone = TAP_NONE;
  }

  return tapZone;
}

//////////////////////////////////////////////////////////////////////////////////
// doActions()
// actions that occur in response to tapping a virtual button on the screen
// depending on current display state, and location of tap

void doActions(){

  if ( tapNew == false) return;

  int newDisplayState = displayState;

  switch(displayState){

    // tapping in the message area will toggle the alarm on/off
    // tapping anywhere else changes to date display

  case STATE_DISP_TIME: // state 0
    switch(tapZone){

      if( tapZone == TAP_MESSAGE ){
        if(alarmState == true) newDisplayState = STATE_ALARM_OFF;
        else if(alarmState == false) newDisplayState = STATE_ALARM_ON;
      }

      else{
        newDisplayState = STATE_DISP_DATE;
      }


      // nothing to do here, i think

    }
    break;

    // when in date mode, any tap will advance to alarm init

  case STATE_DISP_DATE: // state 1
    newDisplayState = STATE_ALARM_INIT;
    break;

    // the alarm init state captures the first tap on the alarm set screen
    // if the first tap is in the message area, advance to next menu
    // if the first tap is elsewhere, change the alarm digit accordingly

  case STATE_ALARM_INIT: // state 2
    switch(tapZone){

    case TAP_MESSAGE:
      if(alarmState == true) newDisplayState = STATE_ALARM_ON;
      else if(alarmState == false) newDisplayState = STATE_ALARM_OFF;
      break;

    case TAP_HOURPLUS:
      if (alarmHour12 < 12) alarmHour12++;
      else if (alarmHour12 == 12){
        alarmHour12 = 1;
        if (alarmPM == 0) alarmPM = 1;
        else if (alarmPM != 0) alarmPM = 0;
      }
      newDisplayState = STATE_ALARM_SET;
      break;

    case TAP_HOURMINUS:
      if (alarmHour12 > 1) alarmHour12--;
      else if (alarmHour12 == 1){
        alarmHour12 = 12;
        if (alarmPM == 0) alarmPM = 1;
        else if (alarmPM != 0) alarmPM = 0;
      }
      newDisplayState = STATE_ALARM_SET;
      break;

    case TAP_MINUTEPLUS:
      if (alarmMinute < 59) alarmMinute++;
      else if (alarmMinute == 59){
        alarmMinute = 0;
      }
      newDisplayState = STATE_ALARM_SET;
      break;

    case TAP_MINUTEMINUS:
      if (alarmMinute > 0) alarmMinute--;
      else if (alarmMinute == 0){
        alarmMinute = 59;
      }
      newDisplayState = STATE_ALARM_SET;
      break;

    case TAP_AMPM:
      if (alarmPM == 0) alarmPM = 1;
      else if (alarmPM != 0) alarmPM = 0;
      newDisplayState = STATE_ALARM_SET;
      break;

    default:
      newDisplayState = displayState;
      break;
    }
    break;

    // the alarm set state captures everything after the first tap
    // tapping the message area = "done"
    // tapping anywhere else changes the alarm digit accordingly

  case STATE_ALARM_SET: // state 3
    switch(tapZone){

    case TAP_MESSAGE:
      newDisplayState = STATE_ALARM_COMMIT;
      break;

    case TAP_HOURPLUS:
      if (alarmHour12 < 12) alarmHour12++;
      else if (alarmHour12 == 12){
        alarmHour12 = 1;
        if (alarmPM == 0) alarmPM = 1;
        else if (alarmPM != 0) alarmPM = 0;
      }
      newDisplayState = STATE_ALARM_SET;
      break;

    case TAP_HOURMINUS:
      if (alarmHour12 > 1) alarmHour12--;
      else if (alarmHour12 == 1){
        alarmHour12 = 12;
        if (alarmPM == 0) alarmPM = 1;
        else if (alarmPM != 0) alarmPM = 0;
      }
      newDisplayState = STATE_ALARM_SET;
      break;

    case TAP_MINUTEPLUS:
      if (alarmMinute < 59) alarmMinute++;
      else if (alarmMinute == 59){
        alarmMinute = 0;
      }
      newDisplayState = STATE_ALARM_SET;
      break;

    case TAP_MINUTEMINUS:
      if (alarmMinute > 0) alarmMinute--;
      else if (alarmMinute == 0){
        alarmMinute = 59;
      }
      newDisplayState = STATE_ALARM_SET;
      break;

    case TAP_AMPM:
      if (alarmPM == 0) alarmPM = 1;
      else if (alarmPM != 0) alarmPM = 0;
      newDisplayState = STATE_ALARM_SET;
      break;

    default:
      newDisplayState = displayState;
      break;
    }
    break;

    // commit alarm settings, and reset flags

  case STATE_ALARM_COMMIT: // state 4

    // do something here to write alarm time to RTC
    alarmState = true;
    menuActive = false;
    newDisplayState = STATE_DISP_TIME;
    break;

    // if alarm is OFF, the word "OFF" is displayed in the AMPM region
    // tapping "OFF" toggles the alarm to "ON"
    // tapping anything else advances to time init menu
  case STATE_ALARM_OFF: // state 5
    switch(tapZone){

    case TAP_AMPM:
      alarmState = true;
      newDisplayState = STATE_ALARM_ON;
      break;

    default:
      newDisplayState = STATE_TIME_INIT;
      break;
    }

    break;

    // if alarm is ON, the word "ON" is displayed in the AMPM region
    // tapping "ON" toggles the alarm to "OFF"
    // tapping anything else advances to time init menu
  case STATE_ALARM_ON: // state 6
    switch(tapZone){

    case TAP_AMPM:
      alarmState = false;
      newDisplayState = STATE_ALARM_OFF;
      break;

    default:
      newDisplayState = STATE_TIME_INIT;
      break;
    }
    break;

    // these modes TBD

  case STATE_TIME_INIT: // state 7
    newDisplayState = STATE_DISP_TIME;
    break;

  case STATE_TIME_SET: // state 8
    newDisplayState = STATE_DISP_TIME;
    break;

  case STATE_YEAR_INIT: // state 9
    newDisplayState = STATE_DISP_TIME;
    break;

  case STATE_YEAR_SET: // state 10
    newDisplayState = STATE_DISP_TIME;
    break;

  case STATE_YEAR_COMMIT: // state 11
    newDisplayState = STATE_DISP_TIME;
    break;

  case STATE_MONTH_INIT: // state 12
    newDisplayState = STATE_DISP_TIME;
    break;

  case STATE_MONTH_SET: // state 13
    newDisplayState = STATE_DISP_TIME;
    break;

  case STATE_MONTH_COMMIT: // state 14
    newDisplayState = STATE_DISP_TIME;
    break;

  case STATE_DAY_INIT : // state 15
    break;

  case STATE_DAY_SET: // state 16
    newDisplayState = STATE_DISP_TIME;
    break;

  case STATE_DAY_COMMIT: // state 17
    newDisplayState = STATE_DISP_TIME;
    break;

  default:
    break;

  } // end of switch(displayState)

  displayState = newDisplayState;
  tapZone = TAP_NONE; //reset tap zone
  tapNew = false; // clear new tap flag
}


//////////////////////////////////////////////////////////////////////////////////
// setDisplay()
// define display variables according to display state

void setDisplay(){

  if (menuActive == true){    
    if ((millis() - menuStartMillis) > menuTimeout){
      menuActive = false;
      displayState = STATE_DISP_TIME;
      // exit menu triggers here, if necessary
    }
  }

  switch(displayState){

  case STATE_DISP_TIME:
    dispHour12 = nowHour12;
    dispMinute = nowMinute;
    dispPM = nowPM;

    if(alarmState == true){ 
      msg[1] = 'H';
      msg[2] = 'H';
      msg[3] = 'M';
      msg[4] = 'M';
      msg[5] = 'A';
      msg[6] = '\0';
    }

    else if(alarmState == false){
      msg[1] = '\0';
      msg[2] = '\0';
      msg[3] = '\0';
      msg[4] = '\0';
      msg[5] = '\0';
      msg[6] = '\0';
    }

    break;

  case STATE_DISP_DATE: // to be revised for proper date display
    dispHour12 = 88;
    dispMinute = 88;
    dispPM = nowPM;           
    msg[1] = 'D';
    msg[2] = 'A';
    msg[3] = 'T';
    msg[4] = 'E';
    msg[5] = '\0';
    msg[6] = '\0';
    break;

  case STATE_ALARM_INIT:
    dispHour12 = alarmHour12;
    dispMinute = alarmMinute;
    dispPM = alarmPM;           
    msg[1] = 'A';
    msg[2] = 'L';
    msg[3] = 'A';
    msg[4] = 'R';
    msg[5] = 'M';
    msg[6] = '\0';
    break;

  case STATE_ALARM_SET:
    dispHour12 = alarmHour12;
    dispMinute = alarmMinute;
    dispPM = alarmPM;           
    msg[1] = 'D';
    msg[2] = 'O';
    msg[3] = 'N';
    msg[4] = 'E';
    msg[5] = '\0';
    msg[6] = '\0';
    break;

  case STATE_ALARM_COMMIT:
    break;

  case STATE_ALARM_OFF:
    dispHour12 = alarmHour12;
    dispMinute = alarmMinute;
    dispPM = alarmPM;           
    msg[1] = 'A';
    msg[2] = 'L';
    msg[3] = 'A';
    msg[4] = 'R';
    msg[5] = 'M';
    msg[6] = 'O';
    msg[7] = 'F';
    msg[8] = 'F';
    break;

  case STATE_ALARM_ON:
    dispHour12 = alarmHour12;
    dispMinute = alarmMinute;
    dispPM = alarmPM;           
    msg[1] = 'A';
    msg[2] = 'L';
    msg[3] = 'A';
    msg[4] = 'R';
    msg[5] = 'M';
    msg[6] = '\0';
    msg[7] = 'O';
    msg[8] = 'N';
    break;

  default:
    break;

  } // end switch(displayState)

  return;
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
// renderScreen()
// draw the screen, using the digits and characters passed to the function

void renderScreen( int hour12, int minute, int pm, boolean msgState, char message[10]){    

  char buf[5];
  char ampm = 'A';
  char letter_m = 'M';

  dig[0] = '1'; // set flag for valid time

  // clear any prior tap dots
  ledMatrix.plot(dot[2],dot[3],BLACK); // clear any prior tap dots

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



/*** this needs to be relocated
 * // draw the tap dot, if the menu is active
 * // and put the X,Y location of the plotted dot into dot[2], dot[3] so we know what to erase next time
 * if (p.z > ts.pressureThreshhold){
 * ledMatrix.plot(dot[0],dot[1],message_color);
 * dot[2] = dot[0];
 * dot[3] = dot[1];
 * }
 ***/








