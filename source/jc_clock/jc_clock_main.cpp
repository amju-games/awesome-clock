//--------------------------------------------------------------------------------------------------
// JC CLOCK
// Arduino clock
// Jason Colman 2016
//--------------------------------------------------------------------------------------------------

// Credits for libraries/example code:
// Max7219 library: tomek ness /FH-Potsdam / Feb 2007 http://design.fh-potsdam.de/ 
// DS3231 real time clock library: Rtc by Makuna

#include <Arduino.h>
#include "clock.h"

void setup()
{
  Clock::instance()->setup();
}


void loop()
{
  Clock::instance()->loop();
}

