#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define BACKLIGHT_PIN     13

int temp;
int tempLCD;
int heat = 20;//start at a mid range to start the heating
const int lowTemp = 1;//min of temp range to extrude
const int highTemp = 3;//max of temp range to extrude
int count = 0;
Servo heater;
LiquidCrystal_I2C lcd(0x38);  // Set the LCD I2C address

void setup()
{

  heater.attach(6);
  Serial.begin(9600);

  lcd.begin(20,4);
  uint8_t L1[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111
  };
  lcd.createChar(0,L1);
  //loc = 1
  uint8_t L2[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111
  };
  lcd.createChar(1,L2);
  //loc = 2
  uint8_t L3[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111
  };
  lcd.createChar(2,L3);
  //loc = 3
  uint8_t L4[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };
  lcd.createChar(3,L4);

  //loc = 4
  uint8_t L5[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };
  lcd.createChar(4,L5);
  //loc = 5
  uint8_t L6[8] = {
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };
  lcd.createChar(5,L6);
  //loc = 6
  uint8_t L7[8] = {
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };
  lcd.createChar(6,L7);
  //loc = 7
  uint8_t L8[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };
  lcd.createChar(7,L8);
  lcd.clear();
  //--------------------
  //--Pppp--Iiii--Dddd--
  lcd.setCursor(0,1);
  lcd.print("--PXXX--IXXX--DXXX--");
  //--------------------
  //ideal iiii curr cccc
  lcd.setCursor(0,0);
  lcd.print("ideal-XXXX-curr-XXXX");
  lcd.setCursor(0,2);
  lcd.print(char(0));
  lcd.print(char(1));
  lcd.print(char(2));
  lcd.print(char(3));
  lcd.print(char(4));
  lcd.print(char(5));
  lcd.print(char(6));
  lcd.print(char(7));
  delay(1000);

}

void loop()
{
  temp = analogRead(1);
  if (count < 10)
  {
    count ++;
    tempLCD = 0;//equation to convert thermo couple value to deg. this will be used to display on the LCD
  }
  else if (count > 10)
  {
    if (temp < lowTemp)
    {
      heat ++;
    }
    else if (temp > highTemp)
    {
      heat --;
    }
    count = 0;
    heater.write(heat);
  }
  byte i1 = random();
  byte i2 = random();
  byte i3 = random();
  byte t1 = random();
  byte t2 = random();
  //setP
  lcd.setCursor(3,1);
  lcd.print(i1);
  //setI
  lcd.setCursor(9,1);
  lcd.print(i2);
  //setD
  lcd.setCursor(15,1);
  lcd.print(i3);
  //set Ideal temp
  lcd.setCursor(6,0);
  lcd.print(t1);
  //set current temp
  lcd.setCursor(16,0);
  lcd.print(t2);

  delay(500);

}

