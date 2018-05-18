// include Arduino core library
#include "Arduino.h"

// include this library's description file
#include "TM1638_Arduino.h"

// include description files for other libraries used

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

TM1638::TM1638(uint8_t strobe_pin, uint8_t clock_pin, uint8_t data_pin) {
  this->strobe_pin = strobe_pin;
  this->clock_pin = clock_pin;
  this->data_pin = data_pin;
};

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in TM1638_Arduino sketches, this library, and other libraries

void TM1638::init() {
  init(0);
};

void TM1638::init(uint8_t brightness) {
  pinMode(strobe_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);

  digitalWrite(strobe_pin, HIGH);
  digitalWrite(clock_pin, LOW);
  digitalWrite(data_pin, LOW);

  setBrightness(brightness);
  clear();
  current_button_state = B00000000;
  previous_button_state = B00000000;
};

void TM1638::setBrightness(uint8_t value) {
  // 0 <= value <= 7
  if (value > 7) { value = 0; }
  sendCommand(value | SHOW_TURN_ON); // set brightness and activate
};

void TM1638::clear() {
  sendCommand(AUTO_ADDRESS_ADD);
  digitalWrite(strobe_pin, LOW);
  //shiftOut(data_pin, clock_pin, LSBFIRST, AUTO_ADDRESS_ADD);
  shiftOut(data_pin, clock_pin, LSBFIRST, 0xc0);   // set starting address to 0
  for (uint8_t i = 0; i < 16; i++) {
    shiftOut(data_pin, clock_pin, LSBFIRST, 0x00);
  }
  digitalWrite(strobe_pin, HIGH);
};

void TM1638::setAddress(uint8_t address, uint8_t data) {
  sendCommand(FIXED_ADDRESS);
  digitalWrite(strobe_pin, LOW);
  //shiftOut(data_pin, clock_pin, LSBFIRST, FIXED_ADDRESS);
  shiftOut(data_pin, clock_pin, LSBFIRST, ADDRESS_INSTRUCTION_SET | address);
  shiftOut(data_pin, clock_pin, LSBFIRST, data);
  digitalWrite(strobe_pin, HIGH);
};

void TM1638::setLED(uint8_t led, uint8_t status) {
  // 0 <= led <= 7
  if (led > 7) { return; }

  // status = 0 ou 1
  if (status > 1) { return; }

  setAddress(led * 2 + 1, status);
};
/*
void TM1638::setDigit(uint8_t digit, uint8_t data) {
  // 0 <= digit <= 7
  if (digit > 7) { return; }

  setAddress(digit * 2, data);
};
*/

uint8_t font[FONT_MAX] = {
  B00111111, // 0
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
  B01100110, // 4
  B01101101, // 5
  B01111101, // 6
  B00000111, // 7
  B01111111, // 8
  B01101111, // 9
  B01110111, // A
  B01111100, // b
  B00111001, // C
  B01011110, // d
  B01111001, // E
  B01110001, // F
  B00111101, // G
  B10000000, // .
  B00000000, //
  B00001000, // segment bas _
  B01000000, // segment milieu -
  B00000001, // segment haut
  B01011100, // rond bas o
  B01100011, // rond haut °
};

void TM1638::setDigit(uint8_t digit, uint8_t number) {
  setDigit(digit, number, false);
};

void TM1638::setDigit(uint8_t digit, uint8_t number, bool dotPoint) {
  // 0 <= digit <= 7
  if (digit > 7) { return; }

  // 0 <= number <= 9
  if (digit > FONT_MAX) { return; }

  uint8_t data = font[number];
  if (dotPoint) {
    data = data | B10000000;
  }
  setAddress(digit * 2, data);
};


uint8_t TM1638::readButtons() {
  previous_button_state = current_button_state;
  digitalWrite(strobe_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, READ_KEY_SCAN_DATA);
  pinMode(data_pin, INPUT);
  current_button_state = shiftIn(data_pin, clock_pin, LSBFIRST);
  current_button_state = current_button_state | (shiftIn(data_pin, clock_pin, LSBFIRST)) << 1;
  current_button_state = current_button_state | (shiftIn(data_pin, clock_pin, LSBFIRST)) << 2;
  current_button_state = current_button_state | (shiftIn(data_pin, clock_pin, LSBFIRST)) << 3;
  pinMode(data_pin, OUTPUT);
  digitalWrite(strobe_pin, HIGH);
  return current_button_state ;
};

bool TM1638::getButton(uint8_t button) {
  // 0 <= button <= 7
  if (button > 7) { return(0); }
  return (current_button_state >> button) & B00000001;
};

bool TM1638::getButtonPrevious(uint8_t button) {
  // 0 <= button <= 7
  if (button > 7) { return(0); }
  return (previous_button_state >> button) & B00000001;
};

bool TM1638::isPressed(uint8_t button) {
  return getButton(button);
};

bool TM1638::hasBeenPressed(uint8_t button) {
  return getButton(button) && !getButtonPrevious(button);
};

bool TM1638::hasBeenReleased(uint8_t button) {
  return !getButton(button) && getButtonPrevious(button);
};

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

void TM1638::sendCommand(uint8_t value) {
  digitalWrite(strobe_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, value);
  digitalWrite(strobe_pin, HIGH);
};
