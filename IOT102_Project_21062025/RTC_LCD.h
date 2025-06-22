#ifndef RTC_CONTROL_H
#define RTC_CONTROL_H

#include <Wire.h>
#include <RTClib.h>

// Define objects
extern RTC_DS1307 rtc;
extern char daysOfTheWeek[7][12];

// Function declarations
void initRTC();
String getDate();
String getTime();
#endif