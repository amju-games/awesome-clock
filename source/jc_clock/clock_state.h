#pragma once

//--------------------------------------------------------------------------------------------------
// JC CLOCK
// Jason Colman 2016
//--------------------------------------------------------------------------------------------------

class ClockState
{
public:
    virtual ~ClockState() = default;
    virtual void loop() = 0;
    virtual void onActivated() {}

protected:
    int m_loops = 0; // number of times we have looped: reset to zero when state activated
};

