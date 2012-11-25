// Date, time and temperature functions using 
// a Chronodot RTC connected via I2C and Wire lib

#include <Wire.h>
#include "Chronodot.h"

Chronodot RTC;

void setup () {
    Serial.begin(9600);
    Serial.println("Initializing Chronodot.");

    Wire.begin();
    RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop () {
    DateTime now = RTC.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    if(now.month() < 10) Serial.print("0");
    Serial.print(now.month(), DEC);
    Serial.print('/');
    if(now.day() < 10) Serial.print("0");
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    if(now.hour() < 10) Serial.print("0");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    if(now.minute() < 10) Serial.print("0");
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    if(now.second() < 10) Serial.print("0");
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(now.tempC(), 1);
    Serial.println(" degrees Celcius");
    Serial.print(now.tempF(), DEC);
    Serial.println(" degrees Farenheit");
    
    Serial.println();
    delay(3000);
}