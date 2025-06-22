#ifndef LCD_CONTROL_H
#define LCD_CONTROL_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define object
extern LiquidCrystal_I2C lcd;

// Function declarations
void initLCD();
void displayTime(const String& dateTimeString);
void displayDate(const String& dateTimeString);

#endif