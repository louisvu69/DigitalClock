#include <Wire.h>
#include "RTClib.h"
// Chân output của LM35
// #define sensorPin A3
RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = { "Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat" };

void setup() {
  Serial.begin(115200);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
  // Năm - tháng - ngày - giờ - phút - giây
  // rtc.adjust(DateTime(2025,6, 21, 16, 52, 0));
}

void loop() {
  DateTime now = rtc.now();
  String dateTimeString = "";  // Initialize an empty string
  dateTimeString += now.year();
  dateTimeString += '/';
  dateTimeString += now.month();
  dateTimeString += '/';
  dateTimeString += now.day();
  dateTimeString += ' ';
  dateTimeString += '(';
  dateTimeString += daysOfTheWeek[now.dayOfTheWeek()];
  dateTimeString += ") ";
  dateTimeString += now.hour();
  dateTimeString += ':';
  dateTimeString += now.minute();
  dateTimeString += ':';
  dateTimeString += now.second();
  Serial.println(dateTimeString);  // Print the entire string
  // Get the voltage reading from the LM35
  // int reading = analogRead(sensorPin);
  // Convert that reading into voltage
  // float voltage = reading * (5.0 / 1024.0);
  // Convert the voltage into the temperature in Celsius
  // float temperatureC = voltage * 100;
  // Print the temperature in Celsius
  // Serial.print("  Temperature: ");
  // Serial.print(temperatureC);
  // Serial.print("\xC2\xB0");  // shows degree symbol
  // Serial.println("C");
  delay(1000);
  
}