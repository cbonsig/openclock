#include <ht1632c.h>

ht1632c ledMatrix = ht1632c(&PORTD, 7, 6, 4, 5, GEOM_32x16, 2);

void setup() {
  ledMatrix.clear();
}

void loop() {
  byte x,y, neighbors, newval;

  ledMatrix.plot(12,9,1);  // Plant an "acorn"; a simple pattern that
  ledMatrix.plot(14,10,1); //  grows for quite a while..
  ledMatrix.plot(11,11,1);
  ledMatrix.plot(12,11,1);
  ledMatrix.plot(15,11,1);
  ledMatrix.plot(16,11,1);
  ledMatrix.plot(17,11,1);

  ledMatrix.plot(10,9,1);  // Plant an "acorn"; a simple pattern that
  ledMatrix.plot(12,10,1); //  grows for quite a while..
  ledMatrix.plot(9,11,1);
  ledMatrix.plot(10,11,1);
  ledMatrix.plot(13,11,1);
  ledMatrix.plot(14,11,1);
  ledMatrix.plot(15,11,1);

  delay(1000);   // Play life
  ledMatrix.sendframe();

  while (1) {
    for (x=1; x < ledMatrix.x_max; x++) {
      for (y=1; y < ledMatrix.y_max; y++) {
        neighbors = ledMatrix.getpixel(x, y+1) +
          ledMatrix.getpixel(x, y-1) +
          ledMatrix.getpixel(x+1, y) +
          ledMatrix.getpixel(x+1, y+1) +
          ledMatrix.getpixel(x+1, y-1) +
          ledMatrix.getpixel(x-1, y) +
          ledMatrix.getpixel(x-1, y+1) +
          ledMatrix.getpixel(x-1, y-1);

        switch (neighbors) {
        case 0:
        case 1:
          newval = 0;   // death by loneliness
          break;
        case 2:
          newval = ledMatrix.getpixel(x,y);
          break;  // remains the same
        case 3:
          newval = 1;
          break;
        default:
          newval = 0;  // death by overcrowding
          break;
        }
        ledMatrix.plot(x,y, newval);
      }
    }
    ledMatrix.sendframe();
    delay(1000);
  }
  delay(1000);
}
