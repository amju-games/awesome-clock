//--------------------------------------------------------------------------------------------------
// JC CLOCK
// Jason Colman 2016
//--------------------------------------------------------------------------------------------------

#include <Arduino.h>
#include "hardware_defines.h"
#include "display.h"
#include "bitpattern.h"

static byte columns[NUM_COLUMNS + 1];

// define max7219 registers
static const byte max7219_reg_noop = 0x00;
static const byte max7219_reg_digit0 = 0x01;
static const byte max7219_reg_digit1 = 0x02;
static const byte max7219_reg_digit2 = 0x03;
static const byte max7219_reg_digit3 = 0x04;
static const byte max7219_reg_digit4 = 0x05;
static const byte max7219_reg_digit5 = 0x06;
static const byte max7219_reg_digit6 = 0x07;
static const byte max7219_reg_digit7 = 0x08;
static const byte max7219_reg_decodeMode = 0x09;
static const byte max7219_reg_intensity = 0x0a;
static const byte max7219_reg_scanLimit = 0x0b;
static const byte max7219_reg_shutdown = 0x0c;
static const byte max7219_reg_displayTest = 0x0f;

static void putByte(byte data) 
{
    byte i = 8;
    byte mask;
    while (i > 0) {
        mask = 0x01 << (i - 1);      // get bitmask
        digitalWrite(HARDWARE_DISPLAY_CLOCK, LOW);   // tick
        if (data & mask){            // choose bit
            digitalWrite(HARDWARE_DISPLAY_DATA_IN, HIGH);// send 1
        }
        else{
            digitalWrite(HARDWARE_DISPLAY_DATA_IN, LOW); // send 0
        }
        digitalWrite(HARDWARE_DISPLAY_CLOCK, HIGH);   // tock
        --i;                         // move to lesser bit
    }
}

void maxSingle(byte reg, byte col) 
{
    //maxSingle is the "easy"  function to use for a single max7219

    digitalWrite(HARDWARE_DISPLAY_LOAD, LOW);       // begin     
    putByte(reg);                  // specify register
    putByte(col);//((data & 0x01) * 256) + data >> 1); // put data   
    digitalWrite(HARDWARE_DISPLAY_LOAD, LOW);       // and load da stuff
    digitalWrite(HARDWARE_DISPLAY_LOAD, HIGH);
}

void maxAll(byte reg, byte col) 
{    // initialize  all  MAX7219's in the system
    int c = 0;
    digitalWrite(HARDWARE_DISPLAY_LOAD, LOW);  // begin     
    for (c = 1; c <= NUM_DISPLAY_UNITS; c++) {
        putByte(reg);  // specify register
        putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
    }
    digitalWrite(HARDWARE_DISPLAY_LOAD, LOW);
    digitalWrite(HARDWARE_DISPLAY_LOAD, HIGH);
}

void maxOne(byte maxNr, byte reg, byte col) 
{
    //maxOne is for addressing different MAX7219's, 
    //while having a couple of them cascaded

    int c = 0;
    digitalWrite(HARDWARE_DISPLAY_LOAD, LOW);  // begin     

    for (c = NUM_DISPLAY_UNITS; c > maxNr; c--) {
        putByte(0);    // means no operation
        putByte(0);    // means no operation
    }

    putByte(reg);  // specify register
    putByte(col);//((data & 0x01) * 256) + data >> 1); // put data 

    for (c = maxNr - 1; c >= 1; c--) {
        putByte(0);    // means no operation
        putByte(0);    // means no operation
    }

    digitalWrite(HARDWARE_DISPLAY_LOAD, LOW); // and load da stuff
    digitalWrite(HARDWARE_DISPLAY_LOAD, HIGH);
}

void Display::setUp()
{
    pinMode(HARDWARE_DISPLAY_DATA_IN, OUTPUT);
    pinMode(HARDWARE_DISPLAY_CLOCK, OUTPUT);
    pinMode(HARDWARE_DISPLAY_LOAD, OUTPUT);

    digitalWrite(13, HIGH);

    //initiation of the max 7219
    maxAll(max7219_reg_scanLimit, 0x07);
    maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    maxAll(max7219_reg_displayTest, 0x00); // no display test
    for (int e = 1; e <= 8; e++)
    {
        // empty registers, turn all LEDs off 
        maxAll(e, 0);
    }
    // Set initial display brightness
    maxAll(max7219_reg_intensity, MAX_BRIGHTNESS);
}

void Display::adjustDisplayBrightness()
{
    // Set display brightness depending on ambient light level
    int ldrValue = analogRead(HARDWARE_LDR_INPUT_PIN);
    // Get ldr value in range
    ldrValue = min(MAX_LDR_VALUE, max(MIN_LDR_VALUE, ldrValue));
    int brightness = MAX_BRIGHTNESS * (ldrValue - MIN_LDR_VALUE) / (MAX_LDR_VALUE - MIN_LDR_VALUE);
    // Set display brightness according to LDR value
    brightness = max(0, min(MAX_BRIGHTNESS, brightness)); // make sure in range 0..15
    maxAll(max7219_reg_intensity, brightness);
}

// Number of columns containing useful, readable text (the rest are just spare)
// This is recalculated only when we change the text, but is used every time the display scrolls by
//  one bit position.
int numUsefulColumns = 0;

void Display::setDisplayText(const char* theString)
{
    // Fill up columns with bit patterns to show the text
    int charInStr = strlen(theString) - 1; // current character we are converting to bit pattern, starting with rightmost
    int columnThisChar = 0; // current column bit pattern for the current charater
    for (int i = 0; i < NUM_COLUMNS; i++)
    {
        columns[i] = 0;
        if (charInStr < 0)
        {
            continue; // no more text to convert to bit patterns
        }
        char ch = theString[charInStr];
        columns[i] = get_bitpattern(ch, columnThisChar);
        columnThisChar++;
        if (columnThisChar == get_bitpattern_width(ch))
        {
            // Add empty spacer column
            i++;
            columns[i] = 0;

            columnThisChar = 0;
            if (charInStr == 0)
            {
                numUsefulColumns = i;
            }
            charInStr--;
        }
    }
}

void Display::showScrollingText(int loops)
{
    // Display virtual columns
    for (int i = 0; i < NUM_DISPLAY_COLUMNS; i++)
    {
        int c = numUsefulColumns - NUM_DISPLAY_COLUMNS + i - (loops % numUsefulColumns);
        int displayNum = i / 8 + 1;
        int columnNum = i % 8 + 1;

#ifdef YES_SCROLLING_MSG_REPEATS
        if (c < 0)
        {
            // Repeat -- but needs space
            c += numUsefulColumns;
        }
        maxOne(displayNum, columnNum, columns[c]);
#endif

        int bitpattern = 0; // TODO inverse?
        if (c >= 0)
        {
            bitpattern = columns[c];
        }
        if (m_inverse)
        {
            bitpattern = 255 - bitpattern;
        }
        maxOne(displayNum, columnNum, bitpattern);
    }
}

void Display::showStaticText()
{
    showScrollingText(0);
}

void Display::setTimeText(int hour, int minute, int second)
{
    // Convert 24 to 12 hour
    if (hour == 0)
    {
        hour = 12;
    }
    if (hour > 12)
    {
        hour -= 12;
    }

    const int NUM_CHARS_IN_TIME_STR = 10;
    char timeStr[NUM_CHARS_IN_TIME_STR];

    if (hour > 9)
    {
        // Special character for first '1' so the whole thing fits

        snprintf_P(timeStr,
            NUM_CHARS_IN_STRING,
            PSTR("|%01u%s%02u"),
            hour - 10, ((second & 1) ? ":" : " "), minute);
    }
    else
    {
        snprintf_P(timeStr,
            NUM_CHARS_IN_STRING,
            PSTR(" %01u%s%02u"),
            hour, ((second & 1) ? ":" : " "), minute);
    }
    setDisplayText(timeStr);
}
