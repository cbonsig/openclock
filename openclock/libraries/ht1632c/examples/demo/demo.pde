#include <avr/io.h> 
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <TimerOne.h>
#include <Wire.h>
#include <ht1632c.h>

ht1632c dotmatrix = ht1632c(&PORTD, 7, 6, 4, 5, GEOM_32x16, 2);

void setup ()
{
  wdt_enable(WDTO_8S);
  Wire.begin();
  Serial.begin(9600);
  dotmatrix.clear();
  //setTime(20, 40, 0, 15, 5, 11);
  //RTC.set(now());
  setSyncProvider(RTC.get);
  Timer1.initialize(100000);
  Timer1.attachInterrupt(callback); 
  dotmatrix.pwm(1);
}

char fps_str[15];

// Vertical lines moving from right to left. With this pattern, at each new
// frame half of the pixels are completely modified (i.e. both in the red and green
// plane), and half change only in one plane (i.e. only red or green).
// This leads to ~15fps with two screens.
void launch_fps_benchmark() {
  word frames = 0;
  for (byte k = 0; ; k = (k + 1) % (ORANGE + 1)) {
    frames++;
    for (byte i = 0; i <= dotmatrix.y_max; i++) {
      for (byte j = 0; j <= dotmatrix.x_max; ++j) {
	dotmatrix.plot(j, i, k++);
	if (k > ORANGE) k = 0;
      }
    }
    dotmatrix.sendframe();
    dotmatrix.profile();
    if (frames > 500) return;
  }
}

void callback()
{
  char tmp[20];

  sprintf(tmp, "%02d:%02d:%02d", hour(), minute(), second());
  byte len = strlen(tmp);
  for (int i = 0; i < len; i++)
     dotmatrix.putchar(6*i,  0, tmp[i], GREEN);
  dotmatrix.sendframe();
  wdt_reset();
}

word counter = 0;

void loop ()
{
  counter++;  
  dotmatrix.hscrolltext(8,"MULTICOLOR ",MULTICOLOR, 20, 1, LEFT);
  dotmatrix.hscrolltext(8,"RED ",RED, 10, 1, LEFT);
  dotmatrix.hscrolltext(8,"RED+BLINK ",RED | BLINK, 30, 1, LEFT);
  dotmatrix.hscrolltext(8,"RANDOM COLOR ",RANDOMCOLOR, 50, 1, LEFT);
  dotmatrix.hscrolltext(8,"GREEN ",GREEN, 10, 1, LEFT);
  dotmatrix.hscrolltext(8,"ORANGE ",ORANGE, 10, 1, LEFT);
  dotmatrix.clear();

  Timer1.detachInterrupt(); 
  launch_fps_benchmark();
  Timer1.attachInterrupt(callback); 
  
  dotmatrix.clear();
  sprintf(fps_str, "%d fps ", dotmatrix.fps);
  dotmatrix.hscrolltext(8, fps_str, RED, 50, 1, LEFT);

  char tmp[20] = "The end ;-)";
  byte len = strlen(tmp);
  for (int i = 0; i < len; i++)
     dotmatrix.putchar(6*i,  8, tmp[i], ORANGE);
  dotmatrix.sendframe();

  byte v;
  for (v = 0; v < 16; v++)
  {
    dotmatrix.pwm(v);
    (v < 4) ? delay(50) : delay (100);
  } 
  for (v = 15; v > 0; v--)
  {
    dotmatrix.pwm(v);
    (v < 4) ? delay(50) : delay (100);
  } 
  for (v = 0; v < 16; v++)
  {
    dotmatrix.pwm(v);
    (v < 4) ? delay(50) : delay (100);
  } 
  for (v = 15; v > 0; v--)
  {
    dotmatrix.pwm(v);
    (v < 4) ? delay(50) : delay (100);
  } 
  delay (1000);
  dotmatrix.clear();
  return;  
}

