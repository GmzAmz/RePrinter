#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//#include <Servo.h>

#define BACKLIGHT_PIN     13


//int temp;
//int tempLCD;
//int heat = 20;//start at a mid range to start the heating
//const int lowTemp = 1;//min of temp range to extrude
//const int highTemp = 3;//max of temp range to extrude
//int count = 0;
//Servo heater;
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//Setup for motor controllers
//Servo mc1;
//Servo mc2;
//Servo mc3;
//Servo mc4;

//int mc1_speed = 10;
//int mc2_speed = 60;
//int mc3_speed = 100;
//int mc4_speed = 140;
const uint8_t ROWS = 4;
const uint8_t COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'  }
  ,{'4','5','6'  }
  ,{'7','8','9'  }
  ,{'*','0','#'  }
};
byte rowPins[ROWS] = {5,4,3,2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8,7,6}; //connect to the column pinouts of the keypad
Keypad pad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
int screen = 0;
String enteredTemp;
uint16_t graph[20]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t pos = 0;

void setup()
{
//  heater.attach(6);
  Serial.begin(9600);
  lcd.begin(20,4);
  setupLCDChars();

//  Setup for motor controller pin out
//  mc1.attach(3);
//  mc2.attach(5);
//  mc3.attach(6);
//  mc4.attach(9);
  pad.addEventListener(keypadEvent);

}

int bar = 0;
boolean barIncrease = true;

void loop()
{
//  Serial.println(screen);
  char key = (char)pad.getKey;
//  if (key) {
//    Serial.println();
//  }
  //this section is example, remove when we get thermocouple
  // end section of example code
  if ( screen == 0) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("1: Enter info screen");
    lcd.setCursor(0,1);
    lcd.print("2: Set Temperature");
    screen = 8;
  }
  if ( screen == 1 ) {
    lcd.clear();
    lcd.setCursor(0,1);
    //         --------------------
    //         --Pppp--Iiii--Dddd--
    lcd.print("  P     I     D     ");
    lcd.setCursor(0,0);
    //         --------------------
    //         ideal iiii curr cccc
    lcd.print("ideal      curr     ");
    screen = 2;
  }
  if (screen == 2) {
    randomizePIDT();
    showBargraph();
    delay(200);
    //    Serial.println(bar);
  }
  if (screen == 3){
    lcd.clear();
    lcd.setCursor(0,0);
    delay(100);
    //         --------------------
    lcd.print(" Enter desired temp ");
    lcd.setCursor(0,1);
    delay(100);
    //         --------------------
    lcd.print("Temp:");
    lcd.setCursor(0,4);
    delay(100);
    //         --------------------
    lcd.print("Back: (*) Enter: (#)");
    delay(100);
    screen = 4;
  }
  if (screen == 4){
    lcd.setCursor(6,1);
    lcd.print(enteredTemp);
  }
  //motor controller speed is -180 to 180 closer to zero is faster for some reason
//  mc1.write(mc1_speed);
//  mc2.write(mc2_speed);
//  mc3.write(mc3_speed);
//  mc4.write(mc4_speed);
	incDecBar();
}

void keypadEvent(KeypadEvent key){
  Serial.println("pressed");
  switch (pad.getState()){
  case PRESSED:
  //enter info screen
      if (key == '1'){
        screen = 1;
      }
      
      if (key == '*') {
        screen = 0;
      }
      if (key == '2') {
        screen = 3;
      }
  break;
  }
}

void incDecBar(){
  if (barIncrease == true){
    updateBars(bar);
    bar++;
  } 
  else {
    updateBars(bar);
    bar--;
  }
  if ( bar >= 15 ) {
    barIncrease = false;
  } 
  else if ( bar == 0 ){
    barIncrease = true;
  }
}

void updateBars(uint8_t temp){
  pos++;
  if (pos > 19){ 
    pos = 0;
  }
  graph[pos] = temp;
}

void showBargraph(){
  for (int i = 0; i < 20;i++){
    if (pos + i >= 19){
      writeBar(graph[pos + i - 19],  i);
    } 
    else {
      writeBar(graph[pos + i + 1], i);
    }
  }
}

void writeBar(uint8_t height, uint8_t pos){
  if (height <=7){
    lcd.setCursor(pos, 3);
    lcd.print(char(height));
    lcd.setCursor(pos,2);
    lcd.print(" ");
  } 
  else {
    height -= 8;
    lcd.setCursor(pos, 2);
    lcd.print(char(height));
    lcd.setCursor(pos, 3);
    lcd.print(char(7));
  }
}

void randomizePIDT(){
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
}

void setupLCDChars(){
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
}

