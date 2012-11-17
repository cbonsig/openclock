    // simple clock using 16x32 bicolor LED matrix, Chronodot, and Arduino
    // craig bonsignore
    // 7 may 2012 . change digits to red at night . move pin 4 to 0 . move pin 5 to 1
    // 22 may 2012 . change from bold to normal font during sleeping hours
    // 16 june 2012 . map touchscreen press to pixels
    // 30 june 2012 . recalibrate for new enclosure
    // 17 nov 2012 . rename to openclock.ino and manage revisions using github

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
    
    SimpleTimer timer;
    int menuTimer; // Timer ID
    
    int dot[3]; // on-screen location of touch dot
    
    int menuItem = 0; // 0 if in normal mode, 1+ for menu choices
    int touchTimeout = 5000; // 5000 milliseconds to timeout
    
    int digit_color = GREEN;
    int menu_color = RED;

    
    void setup () {
      
      pinMode(7, OUTPUT);
      pinMode(6, OUTPUT);
      pinMode(0, OUTPUT);
      pinMode(1, OUTPUT);
     
      Wire.begin();
      RTC.begin();
    
      if (! RTC.isrunning()) {
        // Serial.println("RTC is NOT running!");
      }      
      
      ledMatrix.clear(); // clear the display
      ledMatrix.pwm(5); // set brightness (max=15)
      
      // the following line sets the RTC to the date & time this sketch was compiled
      //RTC.adjust(DateTime(__DATE__, __TIME__));
      
    }
    
    void loop () {
      
        timer.run(); // activate SimpleTimer      
              
        // read the current time    
        DateTime now = RTC.now();    
        int hour24 = now.hour();
        int minute = now.minute();
        int hour12 = hour24;
        
        //adjust the brightness based on the time of day
        if (hour24 <= 5) ledMatrix.pwm(1); /// 12:00a to 5:59a minimum brightness
        else if (hour24 <= 6) ledMatrix.pwm(5); // 6:00a to 6:59a brighter
        else if (hour24 <= 19) ledMatrix.pwm(15); // 7:00a to 7:59p max brightness
        else if (hour24 <= 21) ledMatrix.pwm(5); // 8:00p to 9:59p dimmer
        else if (hour24 <= 24) ledMatrix.pwm(1); // 10:00p to 11:59p minimum brightness
        
        //adjust the color based on the time of day
        if (hour24 <= 6) digit_color = RED; // 12:00a to 6:59a red digits
        else if (hour24 <= 19) digit_color = GREEN; // 7:00a to 7:59p green digits 
        else if (hour24 <= 24) digit_color = RED; // 8:00p to 11:59p red digits
        
        if (digit_color == GREEN) menu_color = RED;
        if (digit_color == RED) menu_color = GREEN;

        //adjust the boldness based on the time of day. normal font = less pixels lit = darker for sleeping
        if (hour24 <= 5) ledMatrix.setfont(FONT_7x14); // 12:00a to 5:59a use a 7x14 normal font
        else if (hour24 <= 21) ledMatrix.setfont(FONT_7x14B); // 6:00a to 9:59p use a bold font for improved distance visibility
        else if (hour24 <= 24) ledMatrix.setfont(FONT_7x14); // 10:00p to 11:59p use a 7x14 normal font
        
        // theTime is a String in the format of "0000a"
        String theTime = "";
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
        
        // build theTime string with each digit of the hour and minute
        theTime += String(digit1);
        theTime += String(digit2);
        theTime += String(digit3);
        theTime += String(minute % 10); // 2nd digit of minutes uses the modulo operator
        theTime += ampm;
        theTime += letter_m;    
        
        // convert theTime string into a char array
        char charTime[7];
        theTime.toCharArray(charTime,7);
        
        // draw the time on the screen    
        
        ledMatrix.plot(dot[0],dot[1],BLACK); // clear any prior touch dots
        
        if (digit1 == '1') ledMatrix.putchar(2,-2,'1',digit_color,6);
        else ledMatrix.putchar(2,-2,'1',BLACK,6); // erase the "1" if the hour is 1-9        
        ledMatrix.putchar(9,-2,charTime[1],digit_color,6);
        ledMatrix.plot(16,3,ORANGE);
        ledMatrix.plot(16,6,ORANGE);
        ledMatrix.putchar(18,-2,charTime[2],digit_color,7);
        ledMatrix.putchar(25,-2,charTime[3],digit_color,7);

        // draw the AM pr PM
        ledMatrix.setfont(FONT_4x6);
        ledMatrix.putchar(24,11,charTime[4],ORANGE,6);
        ledMatrix.putchar(28,11,charTime[5],ORANGE,6);
                
        
        // read coordinates, map to location on screen
        
        Point p = ts.getPoint();
        
        // use TouchscreenDemo to get calibration ranges to map X and Y to screen       
        if (p.z > ts.pressureThreshhold){
          if (timer.isEnabled(menuTimer)){
            timer.restartTimer(menuTimer);
            menuItem++;
            if (menuItem == 5) menuItem = 0;
            ledMatrix.rect(0,11,22,15,BLACK);
            ledMatrix.rect(1,10,21,14,BLACK);
            ledMatrix.rect(2,9,20,13,BLACK);
            ledMatrix.rect(3,8,19,12,BLACK);
          }
          else{
            menuItem++;
            if (menuItem == 5) menuItem = 0;
            menuTimer = timer.setTimeout(5000, exitMenu);
          }
          menu(p);
        }
        
        if (timer.isEnabled(menuTimer)){
          menu(p);
        }
        
        ledMatrix.sendframe();
        
        delay(100);
    }
    
    void menu(Point p){
              
        int coords_screen[2];        
        coords_screen[0] = map(p.x,75,930,0,31);
        //coords_screen[1] = map(p.y,340,810,0,15);
        coords_screen[1] = map(p.y,300,770,0,15);
        dot[0] = coords_screen[0];
        dot[1] = coords_screen[1];
        ledMatrix.plot(dot[0],dot[1],menu_color);
        //ledMatrix.sendframe();
        
        ledMatrix.setfont(FONT_4x6);
        switch (menuItem){
          
          case 0:
            ledMatrix.putchar(0,11,'M',menu_color);
            ledMatrix.putchar(4,11,'e',menu_color);
            ledMatrix.putchar(8,11,'n',menu_color);
            ledMatrix.putchar(12,11,'u',menu_color);
            break;
         
         case 1:
            ledMatrix.putchar(0,11,'T',menu_color);
            ledMatrix.putchar(4,11,'i',menu_color);
            ledMatrix.putchar(8,11,'m',menu_color);
            ledMatrix.putchar(12,11,'e',menu_color);
            break;
         
         case 2:
            ledMatrix.putchar(0,11,'A',menu_color);
            ledMatrix.putchar(4,11,'l',menu_color);
            ledMatrix.putchar(8,11,'a',menu_color);
            ledMatrix.putchar(12,11,'r',menu_color);
            ledMatrix.putchar(16,11,'m',menu_color);
            ledMatrix.putchar(20,11,'1',menu_color);
            break;

         case 3:
            ledMatrix.putchar(0,11,'A',menu_color);
            ledMatrix.putchar(4,11,'l',menu_color);
            ledMatrix.putchar(8,11,'a',menu_color);
            ledMatrix.putchar(12,11,'r',menu_color);
            ledMatrix.putchar(16,11,'m',menu_color);
            ledMatrix.putchar(20,11,'2',menu_color);
            break;

         case 4:
            ledMatrix.putchar(0,11,'E',menu_color);
            ledMatrix.putchar(4,11,'x',menu_color);
            ledMatrix.putchar(8,11,'i',menu_color);
            ledMatrix.putchar(12,11,'t',menu_color);
            break;
        }
                 
        //ledMatrix.sendframe();

    }
    
    void exitMenu(){
        ledMatrix.rect(0,11,22,15,BLACK);
        ledMatrix.rect(1,10,21,14,BLACK);
        ledMatrix.rect(2,9,20,13,BLACK);
        ledMatrix.rect(3,8,19,12,BLACK);
        ledMatrix.sendframe();
        timer.disable(menuTimer);
        menuItem = 0; // return menu to default
        
    }
        
      


