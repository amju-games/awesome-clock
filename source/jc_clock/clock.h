//--------------------------------------------------------------------------------------------------
// JC CLOCK
// Jason Colman 2016
//--------------------------------------------------------------------------------------------------

#pragma once

#include "clock_state.h"
#include "display.h"

class Clock
{
private:
  Clock();

public:
  static Clock* instance();

  void setup();
  void loop();

  Display* getDisplay();

private:
  ClockState* m_state;
  Display m_display;
};


