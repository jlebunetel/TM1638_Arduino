#include <TM1638_Arduino.h>

TM1638 afficheur(7, 9, 8); // strobe, clock, data pins

int nombre[8];

void setup() {
  afficheur.init(2);
  Serial.begin(115200);

  for (int i = 0; i < 8; i++) {
    nombre[i] = 0;
  }

  for (int i = 0; i < 8; i++) {
    afficheur.setDigit(i, nombre[i]);
  }
}

void loop() {
  afficheur.readButtons();
  for (int i = 0; i < 8; i++) {
    if (afficheur.hasBeenPressed(i)) {
      nombre[i]++;
      if (nombre[i] == FONT_MAX) {
        nombre[i] = 0;
      }
      afficheur.setLED(i, ON);
      afficheur.setDigit(i, nombre[i]);
    }
    if (afficheur.hasBeenReleased(i)) {
      afficheur.setLED(i, OFF);
    }
  }

  delay(10);
}
