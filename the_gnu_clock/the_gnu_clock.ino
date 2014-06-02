// large numbers clock on lcd
/*
A set of custom made large numbers for a 20x4 LCD using the
LiquidCrystal librabry. Works with displays compatible with the
Hitachi HD44780 driver (YM2004A) and DS1307 RTC.

Made by paolinux
01/06/2014

The Cuicuit:
  LCD RS pin to Digital 12
  LCD Enable pin to Digital 11
  LCD DB4 pin to Digital 5
  LCD DB5 pin to Digital 4
  LCD DB6 pin to Digital 3
  LCD DB7 pin to Digital 2
  LCD V0 tied to a pot to control brightness
  LCD Vss and R/W tied to ground
  LCD Vcc to +5V
*/

// include the library
#include <Wire.h>
#define DS1307_I2C_ADDRESS 0x68
#include <LiquidCrystal.h>

// initialize the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned long timer, timerDisplay;
int currHour, currMinute, currSecond, currDayOfWeek, x;
boolean a1Toggle = true;
int timeoutDisplay = 30; //secondi dopo i quali disattiva il display
int displayMonitor = 13;
int contrastPwm = 10, defaultContrastPwm = 75;
int backlightPwm = 9, defaultBacklightPwm = 150;

// the 8 arrays that form each segment of the custom numbers
byte LT[8] =
{
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte UB[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte RT[8] =
{
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte LL[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111
};
byte LB[8] =
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};
byte LR[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100
};
byte UMB[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte LMB[8] =
{
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};

void customN(byte cDisp)
{
  switch (cDisp) {
    case 0:
      lcd.setCursor(x, 0); // set cursor to column 0, line 0 (first row)
      lcd.write((byte) 0);  // call each segment to create
      lcd.write((byte) 1);  // top half of the number
      lcd.write((byte) 2);
      lcd.setCursor(x, 1); // set cursor to colum 0, line 1 (second row)
      lcd.write((byte) 3);  // call each segment to create
      lcd.write((byte) 4);  // bottom half of the number
      lcd.write((byte) 5);
      break;
    case 1:
      lcd.setCursor(x, 0);
      lcd.write((byte) 1);
      lcd.write((byte) 2);
      lcd.write(" ");
      lcd.setCursor(x, 1);
      lcd.write(" ");
      lcd.write((byte) 5);
      lcd.write(" ");
      break;
    case 2:
      lcd.setCursor(x, 0);
      lcd.write((byte) 6);
      lcd.write((byte) 6);
      lcd.write((byte) 2);
      lcd.setCursor(x, 1);
      lcd.write((byte) 3);
      lcd.write((byte) 7);
      lcd.write((byte) 7);
      break;
    case 3:
      lcd.setCursor(x, 0);
      lcd.write((byte) 6);
      lcd.write((byte) 6);
      lcd.write((byte) 2);
      lcd.setCursor(x, 1);
      lcd.write((byte) 7);
      lcd.write((byte) 7);
      lcd.write((byte) 5);
      break;
    case 4:
      lcd.setCursor(x, 0);
      lcd.write((byte) 3);
      lcd.write((byte) 4);
      lcd.write((byte) 2);
      lcd.setCursor(x, 1);
      lcd.write(" ");
      lcd.write(" ");
      lcd.write((byte) 5);
      break;
    case 5:
      lcd.setCursor(x, 0);
      lcd.write((byte) 0);
      lcd.write((byte) 6);
      lcd.write((byte) 6);
      lcd.setCursor(x, 1);
      lcd.write((byte) 7);
      lcd.write((byte) 7);
      lcd.write((byte) 5);
      break;
    case 6:
      lcd.setCursor(x, 0);
      lcd.write((byte) 0);
      lcd.write((byte) 6);
      lcd.write((byte) 6);
      lcd.setCursor(x, 1);
      lcd.write((byte) 3);
      lcd.write((byte) 7);
      lcd.write((byte) 5);
      break;
    case 7:
      lcd.setCursor(x, 0);
      lcd.write((byte) 1);
      lcd.write((byte) 1);
      lcd.write((byte) 2);
      lcd.setCursor(x, 1);
      lcd.write(" ");
      lcd.write((byte) 0);
      lcd.write(" ");
      break;
    case 8:
      lcd.setCursor(x, 0);
      lcd.write((byte) 0);
      lcd.write((byte) 6);
      lcd.write((byte) 2);
      lcd.setCursor(x, 1);
      lcd.write((byte) 3);
      lcd.write((byte) 7);
      lcd.write((byte) 5);
      break;
    case 9:
      lcd.setCursor(x, 0);
      lcd.write((byte) 0);
      lcd.write((byte) 6);
      lcd.write((byte) 2);
      lcd.setCursor(x, 1);
      lcd.write(" ");
      lcd.write(" ");
      lcd.write((byte) 5);
      break;
  }
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

// 1) Sets the date and time on the ds1307
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers
void setDateDs1307(byte second,        // 0-59
                   byte minute,        // 0-59
                   byte hour,          // 1-23
                   byte dayOfWeek,     // 1-7
                   byte dayOfMonth,    // 1-28/29/30/31
                   byte month,         // 1-12
                   byte year)          // 0-99
{
   Wire.beginTransmission(DS1307_I2C_ADDRESS);
   Wire.write(0);
   Wire.write(decToBcd(second));    // 0 to bit 7 starts the clock
   Wire.write(decToBcd(minute));
   Wire.write(decToBcd(hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateDs1307)
   Wire.write(decToBcd(dayOfWeek));
   Wire.write(decToBcd(dayOfMonth));
   Wire.write(decToBcd(month));
   Wire.write(decToBcd(year));
   Wire.endTransmission();
}

void getDateDs1307(byte *second,
          byte *minute,
          byte *hour,
          byte *dayOfWeek,
          byte *dayOfMonth,
          byte *month,
          byte *year)
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}

void printHour(byte hour)
{
  x = 2;
  if (int(hour) < 10) {
    customN(0);
    x = x + 4;
  } else {
    customN(abs(hour / 10));
    x = x + 4;
  }
  customN(int(hour) - (abs(hour / 10) * 10));
}  

void printMinute(byte minute)
{
  x = 11;
  if (int(minute) < 10) {
    customN(0);
    x = x + 4;
  } else {
    customN(abs(minute / 10));
    x = x + 4;
  }
  customN(int(minute) - (abs(minute / 10) * 10));
}  

void printSecond(byte second)
{
  lcd.setCursor(18,3);
  if (int(second) < 10) {
    lcd.print("0");
  }
  lcd.print(second);
}  

void printDate(byte dayOfMonth, byte month, byte year)
{
  lcd.setCursor(0,3);
  if (int(dayOfMonth) < 10) {
    lcd.print("0");
  }
  lcd.print(dayOfMonth);
  lcd.print("/");
  if (int(month) < 10) {
    lcd.print("0");
  }
  lcd.print(month);
  lcd.print("/");
  lcd.print("20");
  lcd.print(year);
}  

void printSplash()
{
  lcd.setCursor(6,0);
  lcd.print("paolinux");
  lcd.setCursor(3,1);
  lcd.print("THE GNU CLOCK");
  lcd.setCursor(2,3);
  lcd.print("... starting ...");
  
  delay(5000);
  
  lcd.clear();
}

void setup()
{
  // assignes each segment a write number
  lcd.createChar(0, LT);
  lcd.createChar(1, UB);
  lcd.createChar(2, RT);
  lcd.createChar(3, LL);
  lcd.createChar(4, LB);
  lcd.createChar(5, LR);
  lcd.createChar(6, UMB);
  lcd.createChar(7, LMB);

  // sets the LCD's rows and colums:
  lcd.begin(20, 4);
  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  Wire.begin();
  //Serial.begin(9600);
  
  /*
  // run once if you have to setup the RTC clock
  second = 0;
  minute = 28;
  hour = 16;
  dayOfWeek = 1;
  dayOfMonth = 2;
  month = 6;
  year = 14;
  setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
  */
  
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  currHour = hour;
  currMinute = minute;
  currSecond = second;
  
  pinMode(displayMonitor, OUTPUT);
  pinMode(contrastPwm, OUTPUT);
  pinMode(backlightPwm, OUTPUT);
  
  // just for debug
  digitalWrite(displayMonitor, HIGH);
  
  // YM2004A 50 (max contrast) - 100 (min contrast) ** hardware dependant ... have to test it yourself
  analogWrite(contrastPwm, defaultContrastPwm);
  
  analogWrite(backlightPwm, defaultBacklightPwm);
  
  printSplash();
  printHour(hour);
  printMinute(minute);
  
  //uint8_t brightnessPin_,
  //uint8_t contrastPin_,
  //uint8_t initialBrightnessPercent_,
  //uint8_t initialContrastPercent_
}

void loop()
{
  int valA1 = digitalRead(A1);
  int valA2 = digitalRead(A2);
  int valA3 = digitalRead(A3);
  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  
  // disable display on specific timeout
  // you can enable it again by sending a signal to A1
  if (millis() - timerDisplay > (timeoutDisplay * 1000)) {
    if (a1Toggle == true) {
      a1Toggle = false;
      for (int i = defaultBacklightPwm; i >= 0; i--){
        analogWrite(backlightPwm, i);
        delay(10);
      } 
      digitalWrite(displayMonitor, LOW);
      lcd.noDisplay();
    }
  }
  
  if (valA1 == 1) {
    // input signal on A1
    // enable or disable display
    if (a1Toggle == true) {
      a1Toggle = false;
      for (int i = defaultBacklightPwm; i >= 0; i--){
        analogWrite(backlightPwm, i);
        delay(10);
      } 
      digitalWrite(displayMonitor, LOW);
      lcd.noDisplay();
    } else {
      a1Toggle = true;
      timerDisplay = millis();
      lcd.display();
      digitalWrite(displayMonitor, HIGH);
      for (int i = 0; i <= defaultBacklightPwm; i++){
        analogWrite(backlightPwm, i);
        delay(10);
      } 
    }
    delay(500);
  }
  
  if (valA2 == 1) {
    // input signal on A2
    delay(500);
  }
  
  if (valA3 == 1) {
    // input signal on A3
    delay(500);
  }
  
  if (currDayOfWeek != dayOfWeek) {
    printDate(dayOfMonth, month, year);
    currDayOfWeek = dayOfWeek;
  }
  
  if (currHour != hour) {
    printHour(hour);
    currHour = hour;
  }
  
  if (currMinute != minute) {
    printMinute(minute);
    currMinute = minute;
  }
  
  if (currSecond != second) {
    printSecond(second);
    currSecond = second;
    timer = millis();
  }
  
  if (millis() - timer < 500) {
    lcd.setCursor(9,1);
    lcd.print(":");
  } else {
    lcd.setCursor(9,1);
    lcd.print(" ");
  }
}
