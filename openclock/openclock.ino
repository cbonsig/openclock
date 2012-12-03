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
    int menuTimer; // Timer ID
    int menuItem = 0; // 0 if in normal mode, 1+ for menu choices
    int touchTimeout = 5000; // 5000 milliseconds to timeout

    // initialize array to hold [x, y, prior_x, prior_y] data for touch location
    int dot[4] = {0,0,0,0}; 
    
    // initialize array to hold mapped coordinates for touch location
    // pixel[0,1] = [x,y] pixel ... x=0-31, y=0-15
    int pixel[2];
    
    // define the state of each touch "button"
    boolean touchHourPlus = false;
    boolean touchHourMinus = false;
    boolean touchMinutePlus = false;
    boolean touchMinuteMinus = false;
    boolean touchAMPM = false;
    boolean touchMessageArea = false;
    
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
        
        int changedStates = 0;
        
        char buf[7];
        
        // read coordinates, map to location on screen        
        Point p = ts.getPoint();                   // read coordinates

        // define variables for screen regions
        int verticalRegion = 0;
        int horizontalRegion = 0;
        
        // define variables for new touch states
        boolean touchHourPlus_now = false;
        boolean touchHourMinus_now = false;
        boolean touchMinutePlus_now = false;
        boolean touchMinuteMinus_now = false;
        boolean touchAMPM_now = false;
        boolean touchMessageArea_now = false;

        // map coordinates to pixel location
        pixel[0] = map(p.x,75,930,0,31);           // x calibration by trial-and-error
        pixel[1] = map(p.y,300,770,0,15);          // y calibration by trial-and-error
        
        // map pixel location to screen region
        if (pixel[0] < 9) horizontalRegion = 0;           // x pixel 0-8   = 1st digit of hour
        else if (pixel[0] < 17) horizontalRegion = 1;     // x pixel 9-17  = 2nd digit of hour
        else if (pixel[0] < 25) horizontalRegion = 2;     // x pixel 18-25 = 1st digit of minute
        else if (pixel[0] < 33) horizontalRegion = 3;     // x pixel 26-33 = 2nd digit of minute
        
        if (pixel[1] < 6) verticalRegion = 0;          // y pixel 0-5   = top of time digits
        else if (pixel[1] < 11) verticalRegion = 1;     // y pixel 6-10  = bottom of time digits
        else if (pixel[1] < 16) verticalRegion = 2;     // y pixel 11-16 = message / ampm area
        
        // define new button states based on region touched
        
        //if (p.z > ts.pressureThreshhold){
          if (verticalRegion == 0){
            if (horizontalRegion == 0) touchHourPlus = true;
            if (horizontalRegion == 1) touchHourPlus = true;
            if (horizontalRegion == 2) touchMinutePlus = true;
            if (horizontalRegion == 3) touchMinutePlus = true;
          }
          else if(verticalRegion == 1){
            if (horizontalRegion == 0) touchHourMinus = true;
            if (horizontalRegion == 1) touchHourMinus = true;
            if (horizontalRegion == 2) touchMinuteMinus = true;
            if (horizontalRegion == 3) touchMinuteMinus = true;
          }
          else if(verticalRegion == 2){
            if (horizontalRegion == 0) touchMessageArea = true;
            if (horizontalRegion == 1) touchMessageArea = true;
            if (horizontalRegion == 2) touchMessageArea = true;
            if (horizontalRegion == 3) touchAMPM = true;
          }
        //}
        /**else{
          touchHourPlus_now    = false;
          touchHourMinus_now   = false;
          touchMinutePlus_now  = false;
          touchMinuteMinus_now = false;
          touchAMPM_now        = false;
          touchMessageArea_now = false;
        }**/
        
        changedStates = changedStates +
         (touchHourPlus_now - touchHourPlus)+
         (touchHourMinus_now - touchHourMinus)+
         (touchMinutePlus_now - touchMinutePlus)+
         (touchMinuteMinus_now - touchMinuteMinus)+
         (touchAMPM_now - touchAMPM)+
         (touchMessageArea_now - touchMessageArea);
          
        touchHourPlus = touchHourPlus_now;
        touchHourMinus = touchHourPlus_now;
        touchMinutePlus = touchHourPlus_now;
        touchMinuteMinus = touchHourPlus_now;
        touchAMPM = touchHourPlus_now;
        touchMessageArea = touchHourPlus_now;
        
        // temp for troubleshooting
        
        if (touchHourPlus){
            msg[1]='h';
            msg[2]='o';
            msg[3]='u';
            msg[4]='r';
            msg[5]='+';
            msg[6]='\0';
        }
        
        if (touchHourMinus){
            msg[1]='h';
            msg[2]='o';
            msg[3]='u';
            msg[4]='r';
            msg[5]='-';
            msg[6]='\0';
        }
        
        if (touchMinutePlus){
            msg[1]='m';
            msg[2]='i';
            msg[3]='n';
            msg[4]='+';
            msg[5]='\0';
            msg[6]='\0';
        }
        if (touchMinuteMinus){
            msg[1]='m';
            msg[2]='i';
            msg[3]='n';
            msg[4]='-';
            msg[5]='\0';
            msg[6]='\0';
        }
        if (touchAMPM){
            msg[1]='a';
            msg[2]='m';
            msg[3]='p';
            msg[4]='m';
            msg[5]='\0';
            msg[6]='\0';
        }
        if (touchMessageArea){
            msg[1]='m';
            msg[2]='s';
            msg[3]='g';
            msg[4]='\0';
            msg[5]='\0';
            msg[6]='\0';
        }
        
       if (changedStates == false){
            msg[1]='\0';
            msg[2]='\0';
            msg[3]='\0';
            msg[4]='\0';
            msg[5]='\0';
            msg[6]='\0';
       }        
       
       if (p.z > ts.pressureThreshhold){
         touchActive = true;
       }
       else{
         touchActive = false;
       }
       
       itoa(pixel[0],buf,10);
       if (pixel[0] < 10){
         msg[1] = '\0';
         msg[2] = buf[0];
       }
       else{
         msg[1] = buf[0];
         msg[2] = buf[1];
       }
         
       itoa(pixel[1],buf,10);
       if (pixel[1] < 10){
         msg[3] = '\0';
         msg[4] = buf[0];
       }
       else{
         msg[3] = buf[0];
         msg[4] = buf[1];
       }
         
       itoa(p.z,buf,10);
       if (p.z < 10){
         msg[5] = '\0';
         msg[6] = buf[0];
       }
       else{
         msg[5] = buf[0];
         msg[6] = buf[1];
       }

        // draw the touch dot, if the menu is active
        // and put the X,Y location of the plotted dot into dot[2], dot[3] so we know what to erase next time
        if (p.z > ts.pressureThreshhold){
          ledMatrix.plot(dot[0],dot[1],message_color);
          dot[2] = dot[0];
          dot[3] = dot[1];
        }

                
/*******

        // is there a touch point in the message region?
        if (touchMessageArea){ 
          if (timer.isEnabled(menuTimer)){         // is the timeout timer already running?
            timer.restartTimer(menuTimer);         // if so, restart it
            menuItem++;                            // and increment the menu position
            if (menuItem == 4) menuItem = 0;       // loop back to the top of the menu
          }
          else{                                    // the timer isn't running, so...
            menuItem++;                            // increment the menu position
            if (menuItem == 4) menuItem = 0;       // or loop back if necessary
            
            // start the timeout timer at 5 seconds, and call exitMenu with its done
            menuTimer = timer.setTimeout(5000, exitMenu);
          }
          menu(p);                                 // now go to the menu subroutine
        }
        
        if (timer.isEnabled(menuTimer)){           // no touch, but timer is still running?
          menu(p);                                 // go to menu subroutine
        }
******/
 
         return(changedStates); 
 
    }
    
    
    void menu(Point p){
              
        dot[0] = pixel[0];
        dot[1] = pixel[1];
        
/****
        
        switch (menuItem){
          
          case 1:
            msg[1]='A';
            msg[2]='l';
            msg[3]='a';
            msg[4]='r';
            msg[5]='m';
            msg[6]='\0';
            
            // if (region[0] <= 1 && region[1] == 0){
            //   dispHour++;
            // }
            // else if (region[0] <= 1 && region[1] == 1){
            //   dispHour--;
            // }
            // else if (region[0] >= 2 && region[1] == 0){
            //   dispMin++;
            // }
            // else if (region[0] >= 2 && region[1] == 1){
            //   dispMin--;
            // }
            
            break;
         
         case 2:
            msg[1]='T';
            msg[2]='i';
            msg[3]='m';
            msg[4]='e';
            msg[5]='\0';
            msg[6]='\0';
            break;
         
         case 3:
            msg[1]='E';
            msg[2]='x';
            msg[3]='i';
            msg[4]='t';
            msg[5]='\0';
            msg[6]='\0';
            break;
        }
            ****/
                 
    }
    
    // draw the screen
    
    void drawScreen( int hour12, int minute, int pm, boolean msgState, char message[10]){    
        
        char buf[5];
        char ampm = 'A';
        char letter_m = 'M';
        
        dig[0] = '1'; // set flag for valid time
        
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
      
        // clear any prior touch dots
        ledMatrix.plot(dot[2],dot[3],BLACK); // clear any prior touch dots
        
        // clear the message area        
        ledMatrix.rect(0,11,22,15,BLACK);
        ledMatrix.rect(1,10,21,14,BLACK);
        ledMatrix.rect(2,9,20,13,BLACK);
        ledMatrix.rect(3,8,19,12,BLACK);
        
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
        
    
    void exitMenu(){
        timer.disable(menuTimer);
        menuItem = 0; // return menu to default        
        
        
        /*
        // temporary for testing
        DateTime now = RTC.now();    
        dispHour = now.hour();
        dispMin = now.minute();
        if (dispHour > 12) dispHour = dispHour - 12;
        */
        
    }
        
      


