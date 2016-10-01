#pragma once

//--------------------------------------------------------------------------------------------------
// JC CLOCK
// Jason Colman 2016
//--------------------------------------------------------------------------------------------------

class Display
{
public:
    // Called once from setup()
    void setUp();

    // Call when string to display changes
    void setDisplayText(const char* text);

    // Create bit pattern for fixed, non-scrolling time text
    void setTimeText(int hour, int minute, int second);

    // Show scrolling text, using incrementing counter to set the current position
    void showScrollingText(int counter);

    void showStaticText();

    void adjustDisplayBrightness();

    void setInverse(bool inv_) { m_inverse = inv_; }

private:
    bool m_inverse = false;
};

