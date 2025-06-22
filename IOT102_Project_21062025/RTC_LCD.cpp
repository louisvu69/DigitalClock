#include "RTC_LCD.h"

// Initialize objects
RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = { "Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat" };

void initRTC() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
}

String getDate() {
  DateTime now = rtc.now();
  String getDateString = "";
  getDateString += now.year();
  getDateString += '/';
  getDateString += now.month();
  getDateString += '/';
  getDateString += now.day();
  getDateString += ' ';
  // getDateString += '(';
  getDateString += daysOfTheWeek[now.dayOfTheWeek()];
  return getDateString;
}

String getTime() {
  DateTime now = rtc.now();
  String getTimeString = "";
  getTimeString += now.hour();
  getTimeString += ':';
  getTimeString += now.minute();
  getTimeString += ':';
  getTimeString += now.second();
  return getTimeString;
}
