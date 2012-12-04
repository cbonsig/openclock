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
    

    #include <Wire.h> // I2C library for Chronodot
    #include <ht1632c.h> // http://code.google.com/p/ht1632c/
    #include <Chronodot.h> // http://planetstephanie.net/2011/12/01/chronodot-library-update/
    #include <TouchScreen.h> // https://github.com/adafruit/Touch-Screen-Library
    #include <SimpleTimer.h> // http://arduino.cc/playground/Code/SimpleTimer


//====================================================================================
// message from redlegoman
// this following comment from you interested me too, so I did a little research.
// go here http://www.arduino.cc/en/Reference/PortManipulation fo an explaination 

    // initialize the 16x32 display
    // for reasons that I do not understand, moving any of these pins to 8 or higher fails


    ht1632c ledMatrix = ht1632c(&PORTD, 7, 6, 0, 1, GEOM_32x16, 2); 
//====================================================================================    
    // initialize the Chronodot real time clock
    Chronodot RTC; 
    
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
    
    // initialize SimpleTimer, used for menu timeout
    SimpleTimer timer;
    int menuTimer; // Timer ID for menu
    int touchTimer; // Timer ID for touch event
    int menuItem = 0; // 0 if in normal mode, 1+ for menu choices
    int menuTimeout = 5000; // 5 second timeout for menu
    int touchTimeout = 1000; // 1 second timeout for touch event

    // initialize array to hold [x, y, prior_x, prior_y] data for touch location
    int dot[4] = {0,0,0,0}; 
    
    // initialize array to hold mapped coordinates for touch location
    // pixel[0,1] = [x,y] pixel ... x=0-31, y=0-15
    int pixel[2];
    
    // define "button" numbers
    
    #define TOUCH_HOURPLUS    1
    #define TOUCH_HOURMINUS   2
    #define TOUCH_MINUTEPLUS  3
    #define TOUCH_MINUTEMINUS 4
    #define TOUCH_AMPM        5
    #define TOUCH_MESSAGE     6
    #define TOUCH_WTF         7
    
    int touchState = 0;
    
    boolean touchActive = false;
    
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
    
    void setup () {
      
      pinMode(7, OUTPUT); // initialize ht1632c pin
      pinMode(6, OUTPUT); // initialize ht1632c pin
      pinMode(0, OUTPUT); // initialize ht1632c pin
      pinMode(1, OUTPUT); // initialize ht1632c pin
     
      Wire.begin(); // initialize I2C
      RTC.begin(); // initialize Chronodot
    
      ledMatrix.clear(); // clear the display
      ledMatrix.pwm(5); // set brightness (max=15)
      
      // the following line sets the RTC to the date & time this sketch was compiled
      //RTC.adjust(DateTime(__DATE__, __TIME__));
      
    }
    
    void loop () {
      
        timer.run(); // activate SimpleTimer                             
        currentTime(); // read current time and set global variables for hour, minute, am/pm         
        setStyle(dispHour24, dispMinute); // set color and brightness based on displayed time   
        detectTouch(); // detect touch state        
        drawScreen(dispHour12, dispMinute, dispPM, touchActive, msg); // draw the screen             
        delay(100); // wait for 100ms
    }
    
    void currentTime () {
      
        // read the current time    
        DateTime now = RTC.now();
       
        // set global variables for current time 
        nowHour24 = now.hour();
        if (nowHour24 > 12){
          nowHour12 = nowHour24 - 12;
          nowPM = 1;
        }
        else{
          nowHour12 = nowHour24;
          nowPM = 0;
        }
        
        if (nowHour24 == 12) nowPM = 1;   
        
        nowMinute = now.minute();
        
        dispHour24 = nowHour24;
        dispHour12 = nowHour12;
        dispMinute = nowMinute;
        dispPM = nowPM;
    }

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


                                    
    int detectTouch(){
        
        int lastTouchState = touchState;
        
        boolean newTouch = false;
        boolean endTouch = false;
        boolean activeTouch = false;
        boolean activeTimer = false;
        
        // read X,Y coordinates and pressure
        Point p = ts.getPoint();  

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
          
          // set boolean variables according to state of touch, timer, etc.
          
          if (lastTouchState == 0 && touchState > 0)          newTouch = true;
          if (touchState > 0 && touchState != lastTouchState) newTouch = true;
          if (touchState == 0 && lastTouchState > 0)          endTouch = true;
          if (p.z > ts.pressureThreshhold )                activeTouch = true;
          if (timer.isEnabled(touchTimer) )                activeTimer = true;
          
          // decide what to do based on touch state, and get outta here
          
          if (activeTouch == false || touchState == TOUCH_WTF){
            if (touchState == TOUCH_WTF) touchState = 0;
            return 0;
          }
          
          else if (activeTimer == false){
            touchTimer = timer.setTimeout(touchTimeout, exitTouch);
            touchActive = true;
            tempFunction(touchState); // temporary test of virtual buttons
            return touchState;
          }
          
          else if (activeTimer == true){
            touchState = lastTouchState;
            touchActive = true;
            tempFunction(touchState); // temporary test of virtual buttons
            return touchState;
          }
          
          else if (activeTimer == false){
            exitTouch();
          }
                    
          else{
            touchActive = false;
            touchState = TOUCH_WTF;
            tempFunction(touchState); // temporary test of virtual buttons
            return touchState;
          }


        /*** this needs to be relocated
        // draw the touch dot, if the menu is active
        // and put the X,Y location of the plotted dot into dot[2], dot[3] so we know what to erase next time
        if (p.z > ts.pressureThreshhold){
          ledMatrix.plot(dot[0],dot[1],message_color);
          dot[2] = dot[0];
          dot[3] = dot[1];
        }
        ***/
    }
    
    // called when touch event timer elapses
    
    void exitTouch(){
      timer.disable(touchTimer);
      touchActive = false;
    }
    
    
    // temporary function to test touch functions
    
    void tempFunction(int ts){
      
      switch (ts){
        
        case TOUCH_HOURPLUS:
            msg[1]='H';
            msg[2]='O';
            msg[3]='U';
            msg[4]='R';
            msg[5]='+';
            msg[6]='\0';
            break;

        case TOUCH_HOURMINUS:
            msg[1]='H';
            msg[2]='O';
            msg[3]='U';
            msg[4]='R';
            msg[5]='-';
            msg[6]='\0';
            break;

        case TOUCH_MINUTEPLUS:
            msg[1]='M';
            msg[2]='I';
            msg[3]='N';
            msg[4]='+';
            msg[5]='\0';
            msg[6]='\0';
            break;
          
        case TOUCH_MINUTEMINUS:
            msg[1]='M';
            msg[2]='I';
            msg[3]='N';
            msg[4]='-';
            msg[5]='\0';
            msg[6]='\0';
            break;
          
        case TOUCH_AMPM:
            msg[1]='A';
            msg[2]='M';
            msg[3]='P';
            msg[4]='M';
            msg[5]='\0';
            msg[6]='\0';
            break;
          
        case TOUCH_MESSAGE:
            msg[1]='M';
            msg[2]='S';
            msg[3]='G';
            msg[4]='\0';
            msg[5]='\0';
            msg[6]='\0';
            break;

        case TOUCH_WTF:
            msg[1]='W';
            msg[2]='T';
            msg[3]='F';
            msg[4]='?';
            msg[5]='\0';
            msg[6]='\0';
            break;

      }
    }
     
    
    // draw the screen
    
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
    
    

