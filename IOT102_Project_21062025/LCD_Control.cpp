#include "LCD_Control.h"

// Define object (only defined here, not in other files)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void initLCD() {
  lcd.begin();
  lcd.backlight();
}

void displayDate(const String& dateTimeString) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(dateTimeString); // Display date (YYYY/MM/DD/Thứ)
  // lcd.setCursor(0, 1);
  // lcd.print(dateTimeString.substring(17)); // Display day and time
}

void displayTime(const String& dateTimeString) {
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print(dateTimeString.substring(0, 16)); // Display date (YYYY/MM/DD/Thứ)
  lcd.setCursor(0, 1);
  lcd.print(dateTimeString); // Display day and time
}