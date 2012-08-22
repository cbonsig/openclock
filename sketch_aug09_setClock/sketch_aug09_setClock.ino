    // simple clock using 16x32 bicolor LED matrix, Chronodot, and Arduino
    // craig bonsignore
    // 15 april 2012
    // 7 may 2012 . move pin 4 to 0 . move pin 5 to 1


    #include <Wire.h>
    #include <ht1632c.h> // http://code.google.com/p/ht1632c/
    #include <Chronodot.h> // http://planetstephanie.net/2011/12/01/chronodot-library-update/
    
    ht1632c ledMatrix = ht1632c(&PORTD, 7, 6, 4, 5, GEOM_32x16, 2); // initialize the 16x32 display
    Chronodot RTC; // initialize the Chronodot real time clock
    
    void setup () {
    
      Wire.begin();
      RTC.begin();
    
      if (! RTC.isrunning()) {
        // Serial.println("RTC is NOT running!");
      }      
      
      ledMatrix.clear(); // clear the display
      ledMatrix.pwm(15); // set brightness (max=15)
      ledMatrix.setfont(FONT_4x6); // use a 6x13 bold font
      
      // the following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
      
    }
    
    void loop(){
    
      ledMatrix.putchar(0,0,'!',RED,6);
      ledMatrix.sendframe();
    
    }
    
