//--------------------------------------------------------------------------------------------------
// JC CLOCK
// Jason Colman 2016
//--------------------------------------------------------------------------------------------------

#include <Arduino.h>

//--------------------------------------------------------------------------------------------------
// Hardware pin numbers on ESP module
//--------------------------------------------------------------------------------------------------

// Alarm! High = sound the buzzer, low = no sound.
const int HARDWARE_BUZZER_PIN = 14;

// Push button, to turn off alarm etc.
// Button is push to close, connected between this pin and GND.
// A 10k pull up resistor is connected from this pin to VCC.
const int HARDWARE_PUSH_BUTTON_PIN = 12;

// Analog input pin for the light-dependent resistor
const int HARDWARE_LDR_INPUT_PIN = A0;

// Max display brightness, 0..15. This is a property of the MAX display driver chip. 
const int MAX_BRIGHTNESS = 15;

// Roughtly the range of values we get from the LDR, due to bad choice of voltage div resistor
const int MIN_LDR_VALUE = 600;
const int MAX_LDR_VALUE = 1024;

// Pins to control MAX7219
const int HARDWARE_DISPLAY_DATA_IN = 2;
const int HARDWARE_DISPLAY_LOAD = 3;
//const int HARDWARE_DISPLAY_CLOCK = 4;  // just temp, it's 15 on the real board
const int HARDWARE_DISPLAY_CLOCK = 15;  

// How many MAX7219 8 * 8 display matrix units used
const int NUM_DISPLAY_UNITS = 2;  

// How many columns of LEDs
const int NUM_DISPLAY_COLUMNS = NUM_DISPLAY_UNITS * 8;

// Max numbers of characters in scrolling text - TODO Use String?
const int NUM_CHARS_IN_STRING = 44;

// Max number of virtual columns we display, assuming max char width of 5 + 1 spacer
const int NUM_COLUMNS = NUM_CHARS_IN_STRING * 6;



