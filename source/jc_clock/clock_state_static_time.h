#pragma once

//--------------------------------------------------------------------------------------------------
// JC CLOCK
// Jason Colman 2016
//--------------------------------------------------------------------------------------------------

#include "clock_state.h"

class ClockStateStaticTime : public ClockState
{
    ClockStateStaticTime() = default; // SINGLETON

public:
    static ClockStateStaticTime* instance();

    virtual void loop() override;
};

