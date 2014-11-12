#include <Servo.h>

int temp;
int tempLCD;
int heat = 20;//start at a mid range to start the heating
const int lowTemp = 1;//min of temp range to extrude
const int highTemp = 3;//max of temp range to extrude
int count = 0;
Servo heater;

void setup(){
  heater.attach(6);
  Serial.begin(9600);
}

  void loop(){
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
  }
