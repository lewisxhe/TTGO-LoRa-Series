#include "ds3231.h"

#ifdef ENABLE_DS3231
#include <RtcDS3231.h>

RtcDS3231<TwoWire> rtc(Wire);

String ds3231_test()
{
    rtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    if (!rtc.IsDateTimeValid()) {
        rtc.SetDateTime(compiled);
    }
    if (!rtc.GetIsRunning()) {
        Serial.println("RTC was not actively running, starting now");
        rtc.SetIsRunning(true);
    }
    RtcDateTime now = rtc.GetDateTime();
    if (now < compiled) {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        rtc.SetDateTime(compiled);
    }

    rtc.Enable32kHzPin(false);

    rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmOne);

    rtc.LatchAlarmsTriggeredFlags();

    RtcTemperature temp = rtc.GetTemperature();
    Serial.print("RTC Temp : ");
    temp.Print(Serial);
    Serial.println();

    now = rtc.GetDateTime();
    char datestring[20];
    Serial.print("RTC Date : ");

    snprintf_P(datestring,
               sizeof(datestring) / sizeof(datestring[0]),
               PSTR("%02u/%02u/%04u"),
               now.Month(),
               now.Day(),
               now.Year());

    return String(datestring);
}
#endif