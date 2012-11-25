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
    

    #include <Wire.h> // I2C library for Chronodot
    #include <ht1632c.h> // http://code.google.com/p/ht1632c/
    #include <Chronodot.h> // http://planetstephanie.net/2011/12/01/chronodot-library-update/
    #include <TouchScreen.h> // https://github.com/adafruit/Touch-Screen-Library
    #include <SimpleTimer.h> // http://arduino.cc/playground/Code/SimpleTimer
       
    // for reasons that I do not understand, moving any of these pins to 8 or higher fails
    ht1632c ledMatrix = ht1632c(&PORTD, 7, 6, 0, 1, GEOM_32x16, 2); // initialize the 16x32 display
    Chronodot RTC; // initialize the Chronodot real time clock
    
    // For this orientation, the N010-0554-T048 Fujitsu touch panel is oriented with the touch film side up
    // and the ribbon to the left. The four wires, from top to bottom, correspond to XM, YP, XP, YM
    #define XM A0  // black // must be an analog pin, use "An" notation!
    #define YP A1  // green // must be an analog pin, use "An" notation!
    #define XP A2   // yellow // can be a digital pin
    #define YM A3   // blue // can be a digital pin
    
    // For better pressure precision, we need to know the resistance
    // between X+ and X- Use any multimeter to read it
    // 400 ohms between XM and XP; now 90
    TouchScreen ts = TouchScreen(XP, YP, XM, YM, 90);
    
    // initialize SimpleTimer, used for menu timeout
    SimpleTimer timer;
    int menuTimer; // Timer ID
    int menuItem = 0; // 0 if in normal mode, 1+ for menu choices
    int touchTimeout = 5000; // 5000 milliseconds to timeout

    // initialize array to hold [x, y, prior_x, prior_y] data for touch location
    int dot[4] = {0,0,0,0}; 
    
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
      
        currentTime(); // read current time and set value of display characters 
        
        timer.run(); // activate SimpleTimer      
                        
        // read coordinates, map to location on screen        
        Point p = ts.getPoint();               // read coordinates
        if (p.z > ts.pressureThreshhold){      // is there actually a touch point?
          if (timer.isEnabled(menuTimer)){     // is the timeout timer already running?
            timer.restartTimer(menuTimer);     // if so, restart it
            menuItem++;                        // and increment the menu position
            if (menuItem == 5) menuItem = 0;   // loop back to the top of the menu
          }
          else{                                // the timer isn't running, so...
            menuItem++;                        // increment the menu position
            if (menuItem == 5) menuItem = 0;   // or loop back if necessary
            
            // start the timeout timer at 5 seconds, and call exitMenu with its done
            menuTimer = timer.setTimeout(5000, exitMenu);
          }
          menu(p);                             // now go to the menu subroutine
        }
        
        if (timer.isEnabled(menuTimer)){       // no touch, but timer is still running?
          menu(p);                             // go to menu subroutine
        }
        
               
        
        // draw the screen    
        
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
        if (menuItem > 0 && msg[1] != '\0') ledMatrix.putchar(0,11,msg[1],message_color);
        if (menuItem > 0 && msg[2] != '\0') ledMatrix.putchar(4,11,msg[2],message_color);
        if (menuItem > 0 && msg[3] != '\0') ledMatrix.putchar(8,11,msg[3],message_color);
        if (menuItem > 0 && msg[4] != '\0') ledMatrix.putchar(12,11,msg[4],message_color);
        if (menuItem > 0 && msg[5] != '\0') ledMatrix.putchar(16,11,msg[5],message_color);
        if (menuItem > 0 && msg[6] != '\0') ledMatrix.putchar(20,11,msg[6],message_color);

        // draw the AM or PM
        ledMatrix.setfont(FONT_4x6);
        ledMatrix.putchar(24,11,msg[7],ampm_color,6);
        ledMatrix.putchar(28,11,msg[8],ampm_color,6);
        
        // draw the touch dot, if the menu is active
        // and put the X,Y location of the plotted dot into dot[2], dot[3] so we know what to erase next time
        if (p.z > ts.pressureThreshhold){
          ledMatrix.plot(dot[0],dot[1],message_color);
          dot[2] = dot[0];
          dot[3] = dot[1];
        }
        
        // send the characters to the display, and draw the screen
        ledMatrix.sendframe();
        
        // wait for 100ms
        delay(100);
    }
    
    void currentTime () {
      
        // read the current time    
        DateTime now = RTC.now();    
        int hour24 = now.hour();
        int minute = now.minute();
        int hour12 = hour24;
        
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
        
        // create character variables for each digit
        char digit1 = '0';
        char digit2 = '0';
        char digit3 = '0';
        char digit4 = '0';
        char ampm = 'A';
        char letter_m = 'M';
            
        // convert to 12 hour format, adjust am/pm
        if (hour24 > 12){
          hour12 = hour24 - 12;
          ampm = 'P';
        }
        if (hour24 == 12) ampm = 'P';
        
        // set the hour digits, using the power of brute force dumbness
        if (hour12 == 0){
          digit1 = '1';
          digit2 = '2';
        }
        else if (hour12 == 1){
          digit1 = '0';
          digit2 = '1';
        }
        else if (hour12 == 2){
          digit1 = '0';
          digit2 = '2';
        }
        else if (hour12 == 3){
          digit1 = '0';
          digit2 = '3';
        }
        else if (hour12 == 4){
          digit1 = '0';
          digit2 = '4';
        }
        else if (hour12 == 5){
          digit1 = '0';
          digit2 = '5';
        }
        else if (hour12 == 6){
          digit1 = '0';
          digit2 = '6';
        }
        else if (hour12 == 7){
          digit1 = '0';
          digit2 = '7';
        }
        else if (hour12 == 8){
          digit1 = '0';
          digit2 = '8';
        }
        else if (hour12 == 9){
          digit1 = '0';
          digit2 = '9';
        }
        else if (hour12 == 10){
          digit1 = '1';
          digit2 = '0';
        }
        else if (hour12 == 11){
          digit1 = '1';
          digit2 = '1';
        }
        else if (hour12 == 12){
          digit1 = '1';
          digit2 = '2';
        }
                
        // set the first digit of the minutes
        if (minute < 10) digit3 = '0';
        else if (minute < 20) digit3 = '1';
        else if (minute < 30) digit3 = '2';
        else if (minute < 40) digit3 = '3';
        else if (minute < 50) digit3 = '4';
        else digit3 = '5';
        
        // set the second digit of the minutes
        if (minute % 10 == 0) digit4 = '0';
        else if (minute % 10 == 1) digit4 = '1';
        else if (minute % 10 == 2) digit4 = '2';
        else if (minute % 10 == 3) digit4 = '3';
        else if (minute % 10 == 4) digit4 = '4';
        else if (minute % 10 == 5) digit4 = '5';
        else if (minute % 10 == 6) digit4 = '6';
        else if (minute % 10 == 7) digit4 = '7';
        else if (minute % 10 == 8) digit4 = '8';
        else if (minute % 10 == 9) digit4 = '9';
                        
        // set global character display variables
        dig[0] = '1'; // set flag that the following characters are valid
        dig[1] = digit1;
        dig[2] = digit2;
        dig[3] = digit3;
        dig[4] = digit4;
        msg[0] = '0'; // set flag that message area 1-6 is empty 
        msg[7] = ampm;
        msg[8] = letter_m;               
        
    }
    
    
    void menu(Point p){
              
        int coords_screen[2];        
        coords_screen[0] = map(p.x,75,930,0,31);
        coords_screen[1] = map(p.y,300,770,0,15);
        dot[0] = coords_screen[0];
        dot[1] = coords_screen[1];
        
        switch (menuItem){
          
          case 0:
            msg[1]='M';
            msg[2]='e';
            msg[3]='n';
            msg[4]='u';
            msg[5]='\0';
            msg[6]='\0';
            break;
         
         case 1:
            msg[1]='T';
            msg[2]='i';
            msg[3]='m';
            msg[4]='e';
            msg[5]='\0';
            msg[6]='\0';
            break;
         
         case 2:
            msg[1]='A';
            msg[2]='l';
            msg[3]='a';
            msg[4]='r';
            msg[5]='m';
            msg[6]='1';
            break;

         case 3:
            msg[1]='A';
            msg[2]='l';
            msg[3]='a';
            msg[4]='r';
            msg[5]='m';
            msg[6]='2';
            break;

         case 4:
            msg[1]='E';
            msg[2]='x';
            msg[3]='i';
            msg[4]='t';
            msg[5]='\0';
            msg[6]='\0';
            break;
        }
                 
    }
    
    void exitMenu(){
        timer.disable(menuTimer);
        menuItem = 0; // return menu to default
        
    }
        
      


