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
// 03 mar 2013 . debug . reworked menu . tbd -- rewrite rendering for menu states
// 04 mar 2013 . rough code for menu scheme complete -- tbd: RTC commits, fix time set
// 05 mar 2013 . debug . display logic refinement
// 07 mar 2013 . various fixes . added on/off for alarm . fixed colors . added RTC commit for time
//               added : between hour and minute for alarm display
//               tbd: fix date set display DONE.
//                    commit alarm to RTC or EEPROM?
//                    set time sometimes starts at 0:00 instead of current time (???)
//                    add wave board for and alarm trigger code
//                    perhaps 12h / 24 h mode


#include <Wire.h> // I2C library for Chronodot
#include <ht1632c.h> // http://code.google.com/p/ht1632c/
#include <TouchScreen.h> // https://github.com/adafruit/Touch-Screen-Library

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

int menuTimeout = 10000; // 5 second timeout for menu
int tapTimeout = 500; // 0.1 second timeout for tap event

// initialize array to hold [x, y, prior_x, prior_y] data for tap location
int dot[4] = {
  0,0,0,0}; 

// initialize array to hold mapped coordinates for tap location
// pixel[0,1] = [x,y] pixel ... x=0-31, y=0-15
int pixel[2];

// define hour at which clock digits change from red to green in morning
#define GREEN_HOUR      7

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

// define tapCenter "button" numbers

#define TAP_CENTER      8
int tapCenter = TAP_NONE;

boolean tapNew = false;

boolean alarmState = false;

// define display states

#define STATE_DISP_TIME  0
#define STATE_DISP_DATE  10
#define STATE_ALARM_INIT  20
#define STATE_ALARM_SET  201
#define STATE_TIME_INIT  50
#define STATE_TIME_SET  501
#define STATE_DATE_INIT  60
#define STATE_DATE_YEAR  601
#define STATE_DATE_MONTH  602
#define STATE_DATE_DAY  603
#define STATE_EXIT  99

// define render options

#define RENDER_NONE 0
#define RENDER_OFF 1
#define RENDER_ON 2

int displayState = STATE_DISP_TIME;  // 0 if in normal mode, 1+ for menu choices
int menuTaps = true;  // number of taps since menu was last activated

boolean tapActive = false; // true if screen has been taped in last tapTimeout (0.5) seconds, else false
boolean menuActive = false; // true while menu is active

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

int alarmHour12 = 7;
int alarmHour24 = 7;
int alarmMinute = 0;
int alarmPM = 0;

// "new" values are set to current when entering menu mode for the first time
int newHour12 = 0;
int newHour24 = 0;
int newMinute = 0;
int newPM = 0;
int newYear = 2013;
int newMonth = 1;
int newDay = 1;


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

  // debug: initialize serial
  //Serial.begin(9600);

}

//////////////////////////////////////////////////////////////////////////////////
// loop()
// main program loop

void loop () {

  currentTime(); // read current time and set global variables for hour, minute, am/pm         
  detectTap(); // detect tap state
  doActions(); // act upon new tap and set new display state
  setDisplay(); // change display state according to current menu mode    
  setStyle(dispHour24, dispMinute); // set color and brightness based on displayed time 

    int renderOpt = 0;
  if ( alarmState == true) renderOpt = RENDER_ON;
  else if (alarmState == false) renderOpt = RENDER_OFF;


  if (displayState == STATE_DISP_TIME){
    renderTime(nowHour12, nowMinute, nowPM);
  }
  else if (displayState == STATE_DISP_DATE){
    renderOther(RENDER_NONE);
  }  
  else if (displayState == STATE_ALARM_INIT){
    renderOther(renderOpt);
  }
  else if (displayState == STATE_ALARM_SET){
    renderTime(alarmHour12, alarmMinute, alarmPM);
  }
  else if (displayState == STATE_TIME_INIT){
    renderOther(RENDER_NONE);
  }
  else if (displayState == STATE_TIME_SET){
    renderTime(newHour12, newMinute, newPM);
  }
  else if (displayState == STATE_DATE_INIT){
    renderOther(RENDER_NONE);
  }
  else if (displayState == STATE_DATE_YEAR){
    renderOther(RENDER_NONE);
  }
  else if (displayState == STATE_DATE_MONTH){
    renderOther(RENDER_NONE);
  }
  else if (displayState == STATE_DATE_DAY){
    renderOther(RENDER_NONE);
  }
  else if (displayState == STATE_EXIT){
    renderOther(RENDER_NONE);
  }

  delay(50); // wait for 100ms
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

  // define new virtual button states based on region tapped

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
    tapZone = TAP_WTF;
  }
  else{
    tapZone = TAP_NONE;
  }
  
  // check if tap is in the middle, as for toggling ON/OFF in alarm init screen
  if (verticalRegion == 1 || verticalRegion == 2){
    if (horizontalRegion == 2) tapCenter = TAP_CENTER;
    else if (horizontalRegion == 3) tapCenter = TAP_CENTER;
    else tapCenter = TAP_NONE;
  }
  else{
    tapCenter = TAP_NONE;
  }


  menuActive = true;
  tapStartMillis = millis();
  menuStartMillis = millis();
  tapActive = true;                                       // tap event is active
  tapNew = true;                                          // this is a new tap
  menuTaps++;

  // just in case we're going to set the time, set variables according to time
  // that we entered the menu
  if ( menuTaps == 1 ){
    newHour12 = nowHour12;
    newHour24 = nowHour24;
    newMinute = nowMinute;
    newPM = nowPM;
    newYear = year();
    newMonth = month();
    newDay = day();
  }

  return tapZone;
}

//////////////////////////////////////////////////////////////////////////////////
// doActions()
// actions that occur in response to tapping a virtual button on the screen
// depending on current display state, and location of tap

void doActions(){

  // if this isn't a new tap, get outta here.
  if ( tapNew == false){
    return;
  }

  int newDisplayState = displayState;

  int backState = 0;
  int nextState = 0;

  if (displayState == STATE_DISP_TIME){
    backState = STATE_DISP_TIME;
    nextState = STATE_ALARM_INIT;
  }
  else if (displayState == STATE_DISP_DATE){
    backState = STATE_DISP_TIME;
    nextState = STATE_ALARM_INIT;
  }
  else if (displayState == STATE_ALARM_INIT){
    backState = STATE_EXIT;
    nextState = STATE_TIME_INIT;
  }
  else if (displayState == STATE_TIME_INIT){
    backState = STATE_ALARM_INIT;
    nextState = STATE_DATE_INIT;
  }
  else if (displayState == STATE_DATE_INIT){
    backState = STATE_TIME_INIT;
    nextState = STATE_EXIT;
  }
  else if (displayState == STATE_EXIT){
    backState = STATE_DATE_INIT;
    nextState = STATE_ALARM_INIT;
  }

  switch(displayState){

    // tapping in the message area will toggle the alarm on/off
    // tapping anywhere else changes to date display

  case STATE_DISP_TIME: // toggle alarm, or advance to date display

    if( tapZone == TAP_MESSAGE ){
      if(alarmState == true){
        alarmState = false;
        newDisplayState = STATE_DISP_TIME;
      }
      else if(alarmState == false){
        alarmState = true;
        newDisplayState = STATE_DISP_TIME;
      }
    }

    else{
      newDisplayState = STATE_DISP_DATE;
    }
    break;

  case STATE_DISP_DATE: // advance to alarm init
    newDisplayState = nextState;
    break;


  case STATE_ALARM_INIT: // state 20

    // go backward in menu
    
    if (tapCenter == TAP_CENTER){
      if (alarmState == false) alarmState = true;
      else if (alarmState == true) alarmState = false;
      newDisplayState = STATE_ALARM_INIT;
    }
    
    else if ( tapZone == TAP_HOURPLUS || tapZone == TAP_HOURMINUS){
      newDisplayState = backState;
    }

    // go forward in menu
    else if (tapZone == TAP_MINUTEPLUS || tapZone == TAP_MINUTEMINUS){
      newDisplayState = nextState;
    }

    // move to alarm set mode
    else{
      newDisplayState = STATE_ALARM_SET;
    }

    break;

  case STATE_ALARM_SET: // state 201
    switch(tapZone){

    case TAP_HOURPLUS:
      if (alarmHour12 < 11) alarmHour12++;
      else if (alarmHour12 == 11){
        alarmHour12++;
        if (alarmPM == 0) alarmPM = 1;
        else if (alarmPM == 1) alarmPM = 0;
      }
      else if (alarmHour12 == 12){
        alarmHour12 = 1;
      }
      newDisplayState = STATE_ALARM_SET;
      break;

    case TAP_HOURMINUS:
      if (alarmHour12 == 12){
        alarmHour12--;
        if (alarmPM == 0) alarmPM = 1;
        else if (alarmPM != 0) alarmPM = 0;
      }
      else if (alarmHour12 > 1) alarmHour12--;
      else if (alarmHour12 == 1){
        alarmHour12 = 12;
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

    case TAP_MESSAGE:
      // code to commit alarm to RTC should go here!!

      if (alarmPM == false){
        if (alarmHour12 == 12) alarmHour24 = 0; // 12:00am = 00.00
        else alarmHour24 = alarmHour12;         // 1:00am-11:59am = 01.00-11.59 
      }
      else{
        if (alarmHour12 == 12) alarmHour24 = 12; // 12:00pm = 12.00
        else alarmHour24 = alarmHour12 + 12;         // 1:00pm-11:59pm = 13.00-23.59
      }

      alarmState = true;
      menuActive = false;
      menuTaps = 0;
      newDisplayState = STATE_DISP_TIME;
      break;

    default:
      newDisplayState = displayState;
      break;
    }// switch(tapZone) for STATE_ALARM_SET
    break;

  case STATE_TIME_INIT:

    // go backward in menu
    if (tapZone == TAP_HOURPLUS || tapZone == TAP_HOURMINUS){
      newDisplayState = backState;
    }

    // go forward in menu
    else if ( tapZone == TAP_MINUTEPLUS || tapZone == TAP_MINUTEMINUS){
      newDisplayState = nextState;
    }

    // move to alarm set mode
    else{
      newDisplayState = STATE_TIME_SET;
    }
    break;

  case STATE_TIME_SET:

    switch(tapZone){

    case TAP_HOURPLUS:
      if (newHour12 < 11) newHour12++;
      else if (newHour12 == 11){
        newHour12++;
        if (newPM == 0) newPM = 1;
        else if (newPM == 1) newPM = 0;
      }
      else if (newHour12 == 12){
        newHour12 = 1;
      }
      newDisplayState = STATE_TIME_SET;
      break;

    case TAP_HOURMINUS:
      if (newHour12 == 12){
        newHour12--;
        if (newPM == 0) newPM = 1;
        else if (newPM != 0) newPM = 0;
      }
      else if (newHour12 > 1) newHour12--;
      else if (newHour12 == 1){
        newHour12 = 12;
      }
      newDisplayState = STATE_TIME_SET;
      break;

    case TAP_MINUTEPLUS:
      if (newMinute < 59) newMinute++;
      else if (newMinute == 59){
        newMinute = 0;
      }
      newDisplayState = STATE_TIME_SET;
      break;

    case TAP_MINUTEMINUS:
      if (newMinute > 0) newMinute--;
      else if (newMinute == 0){
        newMinute = 59;
      }
      newDisplayState = STATE_TIME_SET;
      break;

    case TAP_AMPM:
      if (newPM == 0) newPM = 1;
      else if (newPM != 0) newPM = 0;
      newDisplayState = STATE_TIME_SET;
      break;

    case TAP_MESSAGE:

      if (newPM == false){
        if (newHour12 == 12) newHour24 = 0; // 12:00am = 00.00
        else newHour24 = newHour12;         // 1:00am-11:59am = 01.00-11.59 
      }
      else{
        if (newHour12 == 12) newHour24 = 12; // 12:00pm = 12.00
        else newHour24 = newHour12 + 12;         // 1:00pm-11:59pm = 13.00-23.59
      }

      // commit to RTC
      setTime(newHour24, newMinute, 0, day(now()), month(now()), year(now()));      
      RTC.set(now());

      menuActive = false;
      menuTaps = 0;
      newDisplayState = STATE_DISP_TIME;
      break;

    default:
      newDisplayState = displayState;
      break;
    }// switch(tapZone) for STATE_TIME_SET
    break;

  case STATE_DATE_INIT:
    // go backward in menu
    if (tapZone == TAP_HOURPLUS || tapZone == TAP_HOURMINUS){
      newDisplayState = backState;
    }

    // go forward in menu
    else if ( tapZone == TAP_MINUTEPLUS || tapZone == TAP_MINUTEMINUS){
      newDisplayState = nextState;
    }

    // move to alarm set mode
    else{
      newDisplayState = STATE_DATE_YEAR;
    }
    break;

  case STATE_DATE_YEAR:

    // upper digit region: increment year
    if (tapZone == TAP_HOURPLUS || tapZone == TAP_MINUTEPLUS){
      newYear++;
      newDisplayState = STATE_DATE_YEAR;
    }

    // lower digit region: decrement year
    else if (tapZone == TAP_HOURMINUS || tapZone == TAP_MINUTEMINUS){
      if (newYear > 2013) newYear--;
      newDisplayState = STATE_DATE_YEAR;
    }

    // message area: advance to month
    else{
      newDisplayState = STATE_DATE_MONTH;
    }
    break;

  case STATE_DATE_MONTH:

    // upper right digit region: increment month
    if (tapZone == TAP_MINUTEPLUS){
      if(newMonth<12) newMonth++; 
      else newMonth = 1;
      newDisplayState = STATE_DATE_MONTH;
    }

    // lower right digit region: decrement year
    else if (tapZone == TAP_MINUTEMINUS){
      if(newMonth>1) newMonth--; 
      else newMonth = 12;
      newDisplayState = STATE_DATE_MONTH;
    }

    // left digit region: blank, so do nothing
    else if (tapZone == TAP_HOURPLUS || tapZone == TAP_HOURMINUS){
      newDisplayState = STATE_DATE_MONTH;
    }

    // message area: advance to month
    else{
      newDisplayState = STATE_DATE_DAY;
    }
    break;

  case STATE_DATE_DAY:
    // upper right digit region: increment day
    if (tapZone == TAP_MINUTEPLUS){
      if (newMonth == 4 || newMonth == 6 || newMonth == 9 || newMonth == 11){
        if (newDay < 30) newDay++; 
        else newDay = 1;
      }
      else if (newMonth == 2){
        if (newDay < 28) newDay++; 
        else newDay = 1;
      }
      else{
        if (newDay < 31) newDay++; 
        else newDay = 1;
      }
      newDisplayState = STATE_DATE_DAY;
    }

    // lower right digit region: decrement day
    else if (tapZone == TAP_MINUTEMINUS){
      if (newMonth == 4 || newMonth == 6 || newMonth == 9 || newMonth == 11){
        if (newDay > 1) newDay--; 
        else newDay = 30;
      }
      else if (newMonth == 2){
        if (newDay > 1) newDay--; 
        else newDay = 28;
      }
      else{
        if (newDay > 1) newDay--; 
        else newDay = 31;
      }
      newDisplayState = STATE_DATE_DAY;
    }

    // left digit region: blank, so do nothing
    else if (tapZone == TAP_HOURPLUS || tapZone == TAP_HOURMINUS){
      newDisplayState = STATE_DATE_DAY;
    }

    // message area: commit to RTC and return to normal
    else{

      //commit to RTC
      setTime(hour(now()), minute(now()), second(now()), newDay, newMonth, newYear);      
      RTC.set(now());


      newDisplayState = STATE_DISP_TIME;
      menuActive = false;
      menuTaps = 0;
    }

    break;

  case STATE_EXIT:

    // go backward in menu
    if (tapZone == TAP_HOURPLUS || tapZone == TAP_HOURMINUS){
      newDisplayState = backState;
    }

    // go forward in menu (wrap around to beginning)
    else if (tapZone == TAP_MINUTEPLUS || tapZone == TAP_MINUTEMINUS){
      newDisplayState = nextState;
    }

    // if < or > not tapped, exit from menu
    else{
      newDisplayState = STATE_DISP_TIME;
      menuActive = false;
      menuTaps = 0;
    }
    break;

  default:
    newDisplayState = displayState;
    break;

  } // end of switch(displayState)

  displayState = newDisplayState;
  tapNew = false; // clear new tap flag
}


//////////////////////////////////////////////////////////////////////////////////
// setDisplay()
// define display variables according to display state

void setDisplay(){

  char buf[5];


  if (menuActive == true){    
    if ((millis() - menuStartMillis) > menuTimeout){
      menuActive = false;
      displayState = STATE_DISP_TIME;
      menuTaps = 0;
      // exit menu triggers here, if necessary
    }
  }

  switch(displayState){

  case STATE_DISP_TIME:
    // use renderTime

    dispHour12 = nowHour12;
    dispMinute = nowMinute;
    dispPM = nowPM;    

    if(alarmState == true){ // display alarm time in message area if alarm is on

      // set the hours digits
      itoa(alarmHour12, buf, 10);
      if (alarmHour12 < 10){
        msg[1] = ' ';
        msg[2] = buf[0];
      }
      else{
        msg[1] = buf[0];
        msg[2] = buf[1];
      }

      // set the minutes digits
      itoa(alarmMinute, buf, 10);
      if (alarmMinute < 10){
        msg[3] = '0';
        msg[4] = buf[0];
      }
      else{
        msg[3] = buf[0];
        msg[4] = buf[1];
      }

      // change zeros to O's for better readability
      if (msg[1] == '0') msg[1] = 'O';
      if (msg[2] == '0') msg[2] = 'O';
      if (msg[3] == '0') msg[3] = 'O';
      if (msg[4] == '0') msg[4] = 'O';

      // set the AM/PM state
      if ( alarmPM == 0 ) msg[5] = 'A';
      else msg[5] = 'P';

      msg[6] = ' ';
    }

    else if(alarmState == false){
      msg[1] = ' ';
      msg[2] = ' ';
      msg[3] = ' ';
      msg[4] = ' ';
      msg[5] = ' ';
      msg[6] = ' ';

    }

    if(nowPM == false) msg[7] = 'A';
    else msg[7] = 'P';
    msg[8] = 'M';

    break;

  case STATE_DISP_DATE: 
    // use renderOther

    // digits display day of month

    dig[1] = ' ';
    dig[2] = ' ';

    itoa(day(), buf, 10);
    if (day() < 10){
      dig[3] = ' ';
      dig[4] = buf[0];
    }
    else{
      dig[3] = buf[0];
      dig[4] = buf[1];
    }

    // message area displays day of week + month of year

    if(weekday()==1){
      msg[1] = 'S'; 
      msg[2] = 'U'; 
      msg[3] = 'N';
    }
    else if(weekday()==2){
      msg[1] = 'M'; 
      msg[2] = 'O'; 
      msg[3] = 'N';
    }
    else if(weekday()==3){
      msg[1] = 'T'; 
      msg[2] = 'U'; 
      msg[3] = 'E';
    }
    else if(weekday()==4){
      msg[1] = 'W'; 
      msg[2] = 'E'; 
      msg[3] = 'D';
    }
    else if(weekday()==5){
      msg[1] = 'T'; 
      msg[2] = 'H'; 
      msg[3] = 'U';
    }
    else if(weekday()==6){
      msg[1] = 'F'; 
      msg[2] = 'R'; 
      msg[3] = 'I';
    }
    else if(weekday()==7){
      msg[1] = 'S'; 
      msg[2] = 'A'; 
      msg[3] = 'T';
    }
    else{
      msg[1] = 'W'; 
      msg[2] = 'T'; 
      msg[3] = 'F';
    }    

    msg[4] = ' ';
    msg[5] = ' ';

    if(month()==1){
      msg[6] = 'J'; 
      msg[7] = 'A'; 
      msg[8] = 'N';
    }
    else if(month()==2){
      msg[6] = 'F'; 
      msg[7] = 'E'; 
      msg[8] = 'B';
    }
    else if(month()==3){
      msg[6] = 'M'; 
      msg[7] = 'A'; 
      msg[8] = 'R';
    }
    else if(month()==4){
      msg[6] = 'A'; 
      msg[7] = 'P'; 
      msg[8] = 'R';
    }
    else if(month()==5){
      msg[6] = 'M'; 
      msg[7] = 'A'; 
      msg[8] = 'Y';
    }
    else if(month()==6){
      msg[6] = 'J'; 
      msg[7] = 'U'; 
      msg[8] = 'N';
    }
    else if(month()==7){
      msg[6] = 'J'; 
      msg[7] = 'U'; 
      msg[8] = 'L';
    }
    else if(month()==8){
      msg[6] = 'A'; 
      msg[7] = 'U'; 
      msg[8] = 'G';
    }
    else if(month()==9){
      msg[6] = 'S'; 
      msg[7] = 'E'; 
      msg[8] = 'P';
    }
    else if(month()==10){
      msg[6] = 'O'; 
      msg[7] = 'C'; 
      msg[8] = 'T';
    }
    else if(month()==11){
      msg[6] = 'N'; 
      msg[7] = 'O'; 
      msg[8] = 'V';
    }
    else if(month()==12){
      msg[6] = 'D'; 
      msg[7] = 'E'; 
      msg[8] = 'C';
    }
    else{
      msg[6] = 'W'; 
      msg[7] = 'T'; 
      msg[8] = 'F';
    }

    break;

  case STATE_ALARM_INIT:
    // use renderOther
    dig[1] = '<';
    dig[2] = ' ';
    dig[3] = ' ';
    dig[4] = '>';

    msg[1] = 'A';
    msg[2] = 'L';
    msg[3] = 'A';
    msg[4] = 'R';
    msg[5] = 'M';
    msg[6] = 'S';
    msg[7] = 'E';
    msg[8] = 'T';
    break;

  case STATE_ALARM_SET:
    // use renderTime
    dispHour12 = alarmHour12;
    dispMinute = alarmMinute;
    dispPM = alarmPM;           
    msg[1] = 'D';
    msg[2] = 'O';
    msg[3] = 'N';
    msg[4] = 'E';
    msg[5] = ' ';
    msg[6] = ' ';
    if(alarmPM==false) msg[7] = 'A';
    else msg[7] = 'P';
    msg[8] = 'M';
    break;

  case STATE_TIME_INIT:
    // use renderOther
    dig[1] = '<';
    dig[2] = ' ';
    dig[3] = ' ';
    dig[4] = '>';

    msg[1] = 'T';
    msg[2] = 'I';
    msg[3] = 'M';
    msg[4] = 'E';
    msg[5] = ' ';
    msg[6] = 'S';
    msg[7] = 'E';
    msg[8] = 'T';
    break;

  case STATE_TIME_SET:
    // use renderTime
    dispHour12 = newHour12;
    dispMinute = newMinute;
    dispPM = newPM;           
    msg[1] = 'D';
    msg[2] = 'O';
    msg[3] = 'N';
    msg[4] = 'E';
    msg[5] = ' ';
    msg[6] = ' ';
    if(newPM==false) msg[7] = 'A';
    else msg[7] = 'P';
    msg[8] = 'M';
    break;
    //break;

  case STATE_DATE_INIT:
    // use renderOther
    dig[1] = '<';
    dig[2] = ' ';
    dig[3] = ' ';
    dig[4] = '>';

    msg[1] = 'D';
    msg[2] = 'A';
    msg[3] = 'T';
    msg[4] = 'E';
    msg[5] = ' ';
    msg[6] = 'S';
    msg[7] = 'E';
    msg[8] = 'T';
    break;

  case STATE_DATE_YEAR:
    // use renderOther

    dig[1] = '2';
    dig[2] = '0';

    // set the year digits
    itoa(newYear-2000, buf, 10);
    dig[3] = buf[0];
    dig[4] = buf[1];

    msg[1] = 'Y';
    msg[2] = 'E';
    msg[3] = 'A';
    msg[4] = 'R';
    msg[5] = ' ';
    msg[6] = ' ';
    msg[7] = '>';
    msg[8] = '>';
    break;

  case STATE_DATE_MONTH:
    // use renderOther
    
    dig[1] = ' ';
    dig[2] = ' ';
    
    itoa(newMonth, buf, 10);
    if(newMonth < 10){
      dig[3] = ' ';
      dig[4] = buf[0];
    }
    else{
      dig[3] = buf[0];
      dig[4] = buf[1];
    }

    msg[1] = 'M';
    msg[2] = 'O';
    msg[3] = 'N';
    msg[4] = 'T';
    msg[5] = 'H';
    msg[6] = ' ';
    msg[7] = '>';
    msg[8] = '>';
    break;

  case STATE_DATE_DAY:
    // use renderOther

    dig[1] = ' ';
    dig[2] = ' ';
    
    itoa(newDay, buf, 10);
    if(newDay < 10){
      dig[3] = ' ';
      dig[4] = buf[0];
    }
    else{
      dig[3] = buf[0];
      dig[4] = buf[1];
    }

    msg[1] = 'D';
    msg[2] = 'O';
    msg[3] = 'N';
    msg[4] = 'E';
    msg[5] = ' ';
    msg[6] = 'D';
    msg[7] = 'A';
    msg[8] = 'Y';
    break;

  case STATE_EXIT:
    // use renderOther
    dig[1] = '<';
    dig[2] = ' ';
    dig[3] = ' ';
    dig[4] = '>';

    msg[1] = 'E';
    msg[2] = 'X';
    msg[3] = 'I';
    msg[4] = 'T';
    msg[5] = ' ';
    msg[6] = ' ';
    msg[7] = ' ';
    msg[8] = ' ';
    break;


  default:
    break;

  } // end switch(displayState)

  return;
}


//////////////////////////////////////////////////////////////////////////////////
// setStyle()
// set color, brightness and boldness based on hour and minute passed to function
// ** intervene here to adjust brightness based on photoresistor reading **

void setStyle(int hour24, int minute) {

  // adjust the brightness based on the time of day
  if (hour24 <= 5) ledMatrix.pwm(1); /// 12:00a to 5:59a minimum brightness
  else if (hour24 <= 6) ledMatrix.pwm(5); // 6:00a to 6:59a brighter
  else if (hour24 <= 19) ledMatrix.pwm(15); // 7:00a to 7:59p max brightness
  else if (hour24 <= 21) ledMatrix.pwm(5); // 8:00p to 9:59p dimmer
  else if (hour24 <= 24) ledMatrix.pwm(1); // 10:00p to 11:59p minimum brightness

  // adjust the color based on the time
  if (hour24 <= (GREEN_HOUR-1)) digit_color = RED; // 12:00a to 6:59a red digits
  else if (hour24 <= (GREEN_HOUR+12-1) ) digit_color = GREEN; // 7:00a to 6:59p green digits 
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
// renderTime()
// draw the screen, using the digits and characters passed to the function

void renderTime(int hour12, int minute, int pm){    

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

  // clear the digit area
  ledMatrix.rect(10,1,20,5,BLACK);
  ledMatrix.rect(11,2,19,4,BLACK);
  ledMatrix.rect(12,3,18,3,BLACK);
  ledMatrix.rect(13,4,17,2,BLACK);


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

  int hour24 = 0;

  if (pm == false){
    if (hour12 == 12) hour24 = 0; // 12:00am = 00.00
    else hour24 = hour12;         // 1:00am-11:59am = 01.00-11.59 
  }
  else{
    if (hour12 == 12) hour24 = 12; // 12:00pm = 12.00
    else hour24 = hour12 + 12;         // 1:00pm-11:59pm = 13.00-23.59
  }

  // adjust the color based on the time
  if (hour24 <= (GREEN_HOUR-1)) digit_color = RED; // 12:00a to 6:59a red digits
  else if (hour24 <= (GREEN_HOUR+12-1) ) digit_color = GREEN; // 7:00a to 6:59p green digits 
  else if (hour24 <= 24) digit_color = RED; // 8:00p to 11:59p red digits

  if (digit_color == GREEN) message_color = RED;
  if (digit_color == RED) message_color = GREEN;

  // fist digit of hour
  if (dig[1] == '1') ledMatrix.putchar(2,-2,'1',digit_color,6);
  else ledMatrix.putchar(2,-2,'1',BLACK,6); // erase the "1" if the hour is 1-9   

  ledMatrix.putchar(9,-2,dig[2],digit_color,6); // second digit of hour
  ledMatrix.plot(16,3,ORANGE); // hour:min colon, top
  ledMatrix.plot(16,6,ORANGE); // hour:min colon, bottom
  ledMatrix.putchar(18,-2,dig[3],digit_color,7); // first digit of minute
  ledMatrix.putchar(25,-2,dig[4],digit_color,7); // second digit of minute

  int shift = 0;

  // on main screen, adjust the color of the alarm time to red or green
  if (alarmState == true && displayState == STATE_DISP_TIME){  
    // adjust the color based on the time of day
    if (alarmHour24 <= 6) message_color = RED; // 12:00a to 6:59a red digits
    else if (alarmHour24 <= 19) message_color = GREEN; // 7:00a to 7:59p green digits 
    else if (alarmHour24 <= 24) message_color = RED; // 8:00p to 11:59p red digits
    shift = 2; // shift the minutes over to make room for the : 
  }


  // draw the characters of the message area if a message is active
  // otherwise, black out the message area
  ledMatrix.setfont(FONT_4x6);
  if (msg[1] != '\0') ledMatrix.putchar(0,11,msg[1],message_color);
  if (msg[2] != '\0') ledMatrix.putchar(4,11,msg[2],message_color);
  if (msg[3] != '\0') ledMatrix.putchar(8+shift,11,msg[3],message_color);
  if (msg[4] != '\0') ledMatrix.putchar(12+shift,11,msg[4],message_color);
  if (msg[5] != '\0') ledMatrix.putchar(16+shift,11,msg[5],message_color);
  if (msg[6] != '\0') ledMatrix.putchar(20+shift,11,msg[6],message_color);

  if (alarmState == true && displayState == STATE_DISP_TIME){  
    ledMatrix.plot(8,12,ORANGE); // dot for alarm
    ledMatrix.plot(8,14,ORANGE); // dot for alarm
  }

  // draw the AM or PM
  ledMatrix.setfont(FONT_4x6);
  ledMatrix.putchar(24,11,msg[7],ampm_color,6);
  ledMatrix.putchar(28,11,msg[8],ampm_color,6);


  // send the characters to the display, and draw the screen
  ledMatrix.sendframe();

}


//////////////////////////////////////////////////////////////////////////////////
// renderOther( int renderOption )
// draw the screen, dig[] and msg[] defined in setDisplay()

void renderOther( int renderOption ){    

  // clear any prior tap dots
  ledMatrix.plot(dot[2],dot[3],BLACK); // clear any prior tap dots

  // clear the message area        
  ledMatrix.rect(0,11,22,15,BLACK);
  ledMatrix.rect(1,10,21,14,BLACK);
  ledMatrix.rect(2,9,20,13,BLACK);
  ledMatrix.rect(3,8,19,12,BLACK);

  // clear the time colon
  ledMatrix.plot(16,3,BLACK); // hour:min colon, top
  ledMatrix.plot(16,6,BLACK); // hour:min colon, bottom

  // clear the digit area
  ledMatrix.rect(10,1,20,5,BLACK);
  ledMatrix.rect(11,2,19,4,BLACK);
  ledMatrix.rect(12,3,18,3,BLACK);
  ledMatrix.rect(13,4,17,2,BLACK);


  // large digits at top of screen
  ledMatrix.putchar(2,-2,dig[1],digit_color,6); // first digit
  ledMatrix.putchar(9,-2,dig[2],digit_color,6); // second digit
  ledMatrix.putchar(18,-2,dig[3],digit_color,7); // third digit
  ledMatrix.putchar(25,-2,dig[4],digit_color,7); // fourth digit

  // draw the characters of the message area if a message is active
  // otherwise, black out the message area
  ledMatrix.setfont(FONT_4x6);
  ledMatrix.putchar(0,11,msg[1],message_color);
  ledMatrix.putchar(4,11,msg[2],message_color);
  ledMatrix.putchar(8,11,msg[3],message_color);
  ledMatrix.putchar(12,11,msg[4],message_color);
  ledMatrix.putchar(16,11,msg[5],message_color);
  ledMatrix.putchar(20,11,msg[6],ampm_color,6);
  ledMatrix.putchar(24,11,msg[7],ampm_color,6);
  ledMatrix.putchar(28,11,msg[8],ampm_color,6);

  if ( renderOption == RENDER_ON ){
    ledMatrix.putchar(12,1,'O',message_color);
    ledMatrix.putchar(16,1,'N',message_color);
  }
  else if ( renderOption == RENDER_OFF){
    ledMatrix.putchar(11,1,'O',message_color);
    ledMatrix.putchar(15,1,'F',message_color);
    ledMatrix.putchar(19,1,'F',message_color);
  }


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




