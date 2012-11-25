#include <ht1632c.h>

ht1632c dotmatrix = ht1632c(&PORTD, 7, 6, 4, 5, GEOM_32x16, 2);

void setup() {
  dotmatrix.clear();
}

void loop() {
  int x = random(1, dotmatrix.x_max);
  int y = random(1, dotmatrix.y_max);
  byte color = random(1,4);
  for (byte rad = 1; rad < dotmatrix.y_max; rad ++) {
    if ((x+rad > dotmatrix.x_max) || (y+rad > dotmatrix.y_max)) break;
    if ((x-rad <= 0) || (y-rad < 0)) break;
    dotmatrix.circle(x,y,rad,color);
    dotmatrix.sendframe();
    delay(100);
    dotmatrix.circle(x,y,rad,0);
    dotmatrix.sendframe();
  }
}
