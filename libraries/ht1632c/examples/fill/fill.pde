#include <ht1632c.h>

ht1632c ledMatrix = ht1632c(&PORTD, 7, 6, 4, 5, GEOM_32x16, 2);

void setup() {
  ledMatrix.clear();
}

void loop() {
  for (byte i=0; i<64; i++) {
    for (byte j=0; j<16; j++) {
      ledMatrix.plot(i, j, RED);
      ledMatrix.sendframe();
      delay(100);
    }
  }

  for (byte i=0; i<64; i++) {
    for (byte j=0; j<16; j++) {
      ledMatrix.plot(i, j, GREEN);
      ledMatrix.sendframe();
      delay(100);
    }
  }
  
  for (byte i=0; i<64; i++) {
    for (byte j=0; j<16; j++) {
      ledMatrix.plot(i, j, BLACK);
      ledMatrix.sendframe();
      delay(100);
    }
  }
}

