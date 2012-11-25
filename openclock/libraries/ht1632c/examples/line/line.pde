#include <ht1632c.h>

ht1632c ledMatrix = ht1632c(&PORTD, 7, 6, 4, 5, GEOM_32x16, 2);

void setup() {
  ledMatrix.clear();
}

void loop() {
  char x1,y1, x2,y2, dx1, dy1, dx2, dy2;

  x1 = random(0,ledMatrix.x_max);
  x2 = random(0,ledMatrix.x_max);
  y1 = random(0,ledMatrix.y_max);
  y2 = random(0,ledMatrix.y_max);
  dx1 = random(1,4);
  dx2 = random(1,4);
  dy1 = random(1,4);
  dy2 = random(1,4);
  while (1) {
    ledMatrix.line(x1,y1, x2,y2, random(1,4));
    ledMatrix.sendframe();
    delay(100);
    ledMatrix.line(x1,y1, x2,y2, 0);
    ledMatrix.sendframe();

    x1 += dx1;
    if (x1 > ledMatrix.x_max) {
      x1 = ledMatrix.x_max;
      dx1 = -random(1,4);
    } 
    else if (x1 < 0) {
      x1 = 0;
      dx1 = random(1,4);
    }

    x2 += dx2;
    if (x2 > ledMatrix.x_max) {
      x2 = ledMatrix.x_max;
      dx2 = -random(1,4);
    } 
    else if (x2 < 0) {
      x2 = 0;
      dx2 = random(1,4);
    }

    y1 += dy1;
    if (y1 > ledMatrix.y_max) {
      y1 = ledMatrix.y_max;
      dy1 = -random(1,3);
    } 
    else if (y1 < 0) {
      y1 = 0;
      dy1 = random(1,3);
    }

    y2 += dy2;
    if (y2 > ledMatrix.y_max) {
      y2 = ledMatrix.y_max;
      dy2 = -random(1,3);
    } 
    else if (y2 < 0) {
      y2 = 0;
      dy2 = random(1,3);
    }
  }
}
