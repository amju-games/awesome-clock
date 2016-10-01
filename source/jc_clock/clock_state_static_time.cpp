//--------------------------------------------------------------------------------------------------
// JC CLOCK
// Jason Colman 2016
//--------------------------------------------------------------------------------------------------

#include <Arduino.h>
#include "clock.h"
#include "clock_state_static_time.h"
#include "hardware_defines.h"
#include "rtc.h"

ClockStateStaticTime* ClockStateStaticTime::instance()
{
    static ClockStateStaticTime t;
    return &t;
}

void ClockStateStaticTime::loop()
{
    //static int loops = 0; // number of times we have looped

    // Scrolling time display
    // Make a string, then convert into column bit patterns
    if (m_loops % 20 == 0)
    {
        // Get time etc from rtc

        RtcDateTime dt = Rtc.GetDateTime();

        char theString[NUM_CHARS_IN_STRING];

        //snprintf_P(theString,
        //    NUM_CHARS_IN_STRING,
        //    //PSTR("M %02u  D %02u  Y %04u %02u:%02u:%02u"),
        //    PSTR("hello"), //, this is a fixed length message with no changng part"), //time is %02u:%02u.%02u"),
        //    PSTR("%02u:%02u.%02u"),
        //    //dt.Month(),
        //    //dt.Day(),
        //    //dt.Year(),
        //    dt.Hour(),
        //    dt.Minute(),
        //    dt.Second());

        // Set up display to show the string
        //setDisplayText(theString);

        Clock::instance()->getDisplay()->setTimeText(dt.Hour(), dt.Minute(), dt.Second());

        ////m_display.setInverse(dt.Second() % 3 == 0);

        // TODO Can do once when this state activated
        Clock::instance()->getDisplay()->showStaticText();

        //Serial.print(theString);

        //Serial.println();

    }

    m_loops++; // TODO base class impl

    // Time (ms) we wait (before scrolling one position; but this is static, so we could wait longer)
    delay(50);

}

