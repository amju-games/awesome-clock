//--------------------------------------------------------------------------------------------------
// JC CLOCK
// Jason Colman 2016
//--------------------------------------------------------------------------------------------------

#include <Arduino.h>
#include "clock.h"
#include "clock_state_static_time.h"
#include "hardware_defines.h"
#include "display.h"
#include "rtc.h"

Clock* Clock::instance()
{
  static Clock c;
  return &c;
}

Clock::Clock()
{
  m_state = ClockStateStaticTime::instance();
}

Display* Clock::getDisplay()
{
  return &m_display;
}

void Clock::setup()
{
    Serial.begin(57600);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    // Set up alarm/buzzer output pin
    pinMode(HARDWARE_BUZZER_PIN, OUTPUT);
    digitalWrite(HARDWARE_BUZZER_PIN, LOW);

    // Set up push button input pin
    pinMode(HARDWARE_PUSH_BUTTON_PIN, INPUT);

    m_display.setUp();

    setUpRtc();

    RtcTemperature temp = Rtc.GetTemperature();
    Serial.print(temp.AsFloat());
    Serial.println("C");

    // Get alarm time from rtc, saved so we don't lose it
    auto alarm = Rtc.GetAlarmOne();
//    m_alarmHour = alarm.Hour();
//    m_alarmMin = alarm.Minute();

//    char buf[100];
//    sprintf(buf, "Alarm: %d:%d", m_alarmHour, m_alarmMin);
//    Serial.println(buf);
}

void Clock::loop()
{
    m_state->loop();

    // TODO TEMP TEST
    // Buzzer test
    bool buzz = false;
    if (digitalRead(HARDWARE_PUSH_BUTTON_PIN) == LOW)
    {
        buzz = true;
    }
    if (buzz)
    {
        // Wow, that's annoying
        digitalWrite(HARDWARE_BUZZER_PIN, HIGH);
    }
    else
    {
        digitalWrite(HARDWARE_BUZZER_PIN, LOW);
    }

    m_display.adjustDisplayBrightness();

    //    showScrollingText(loops);
//    showStaticText();

    //loops++;


}

