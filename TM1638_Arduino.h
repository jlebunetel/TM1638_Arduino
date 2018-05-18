/*
  TM1638_Arduino.h
*/

// ensure this library description is only included once
#ifndef TM1638_Arduino_h
#define TM1638_Arduino_h

// uncomment to activate debugging mode
//#define DEBUG 1

// include description files for other libraries used

#define OFF 0
#define ON 1

// Data instruction set

// Data write mode set
#define WRITE_DATA_TO_REGISTER B01000000
#define READ_KEY_SCAN_DATA     B01000010

// Address add mode set
#define AUTO_ADDRESS_ADD       B01000000
#define FIXED_ADDRESS          B01000100

// Test mode set (inner use)
#define NORMAL_MODE            B01000000
#define TEST_MODE              B01001000


// Display control instruction set

// clean light quantity set
#define SET_PULSE_WIDTH_1_16   B10000000
#define SET_PULSE_WIDTH_2_16   B10000001
#define SET_PULSE_WIDTH_4_16   B10000010
#define SET_PULSE_WIDTH_10_16  B10000011
#define SET_PULSE_WIDTH_11_16  B10000100
#define SET_PULSE_WIDTH_12_16  B10000101
#define SET_PULSE_WIDTH_13_16  B10000110
#define SET_PULSE_WIDTH_14_16  B10000111

// Display turn on/off set
#define SHOW_TURN_OFF          B10000000
#define SHOW_TURN_ON           B10001000

#define ADDRESS_INSTRUCTION_SET B11000000

#define FONT_MAX 24

// library interface description
class TM1638 {
  // user-accessible "public" interface
  public:
    TM1638(uint8_t strobe_pin, uint8_t clock_pin, uint8_t data_pin);
    void init();
    void init(uint8_t brightness);
    void clear();
    void setAddress(uint8_t address, uint8_t data);
    void setLED(uint8_t led, uint8_t status);
    void setDigit(uint8_t digit, uint8_t number);
    void setDigit(uint8_t digit, uint8_t number, bool dotPoint);
    void setBrightness(uint8_t value);
    uint8_t readButtons();
    bool getButton(uint8_t button);
    bool getButtonPrevious(uint8_t button);
    bool isPressed(uint8_t button);
    bool isReleased(uint8_t button);
    bool hasBeenPressed(uint8_t button);
    bool hasBeenReleased(uint8_t button);

  // library-accessible "private" interface
  private:
    void sendCommand(uint8_t value);

    uint8_t strobe_pin;
    uint8_t clock_pin;
    uint8_t data_pin;
    uint8_t current_button_state;
    uint8_t previous_button_state;
};

#endif
