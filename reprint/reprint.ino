#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <MAX6675.h>
#include <Servo.h>
#include <PID_v1.h>

Servo spooler;  // a maximum of eight servo objects can be created 
Servo auger;
Servo heater;                
 
int spooler_speed(0);   
int auger_speed(0);  
int heat_level(0);

//These are all the offsets
int spooler_offset(0);
int auger_offset(0);
int heater_offset(0);

int heatPin = 11;

int tempSensor = 0;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//begin keypad stuff
const uint8_t ROWS = 4;
const uint8_t COLS = 3;
char keys[ROWS][COLS] = {
   {'1','2','3'  }
  ,{'4','5','6'  }
  ,{'7','8','9'  }
  ,{'*','0','#'  }
};
byte rowPins[ROWS] = {5,4,3,2};
byte colPins[COLS] = {8,7,6};
Keypad pad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//end keypad stuff

int screen = 0;
String enteredTemp,entP,entI,entD;
int setP = 0;
int setI = 0;
int setD = 0;
double goalTemp = 0;
double currTemp = 0;

int LED1 = 9;             // Status LED Pin
int CS = 10;             // CS pin on MAX6675
int SO = 12;              // SO pin of MAX6675
int sCK = 13;             // SCK pin of MAX6675
int units = 2;            // Units to readout temp (0 = raw, 1 = ˚C, 2 = ˚F)
float temperature = 0.0;  // Temperature output variable

MAX6675 temp(CS,SO,SCK,units);

//begin bar graph variables
uint16_t graph[20]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
String sPID[] = {"","",""};
uint8_t pos = 0;
// end bar graph variables

//PID Setup, the current temp and goal temp are updated directly in myPID
double Output; //Define Variables we'll be connecting to
PID myPID(&currTemp, &Output, &goalTemp,(double)2,(double)5,(double)1, DIRECT); //Specify the links and initial tuning parameters

void setup()
{
  //initialize the variables we're linked to
  currTemp = temp.read_temp();
  myPID.SetMode(AUTOMATIC);  //turn the PID on
  
  //spooler.attach(9); //attaches spooler motor controller to pin 11
  //auger.attach(21);  //attaches auger motor controller to pin 6
  //heater.attach(11);  //attaches heater motor controller to pin 5
  
  pinMode(heatPin, OUTPUT);
   
  Serial.begin(9600);
  lcd.begin(20,4);
  setupLCDChars();
  
  pad.addEventListener(keypadEvent);
}

void loop()
{    
  currTemp = temp.read_temp();
  myPID.Compute(); //pid compute
  
  heat_level = (Output/2.834); //conversion from 255 to 90 from PID
  Serial.println(heat_level);
  analogWrite(heatPin, heat_level);
  
  spooler_offset = (spooler_speed+90); //Conversion from servo to motor controller
  auger_offset = (auger_speed+90);  //Conversion for servo to motor controller
  heater_offset = (heat_level+90);  //Conversion for servo to motor controller
  
  //spooler.write(spooler_offset);  //Writes the spooler speed
  //auger.write(auger_offset);      //Writes the auger speed
  //heater.write(heater_offset);    //Writes the heater speed
   
  switch (screen){
    case 0:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("1: Enter info screen");
      lcd.setCursor(0,1);
      lcd.print("2: Set temperature");
      lcd.setCursor(0,2);
      lcd.print("3: Edit PID values");
      screen = 42;
      break;
    case 1:
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
      break;
    
    case 2:
      //setP
      lcd.setCursor(3,1);
      lcd.print(sPID[0]);
      //setI
      lcd.setCursor(9,1);
      lcd.print(sPID[1]);
      //setD
      lcd.setCursor(15,1);
      lcd.print(sPID[2]);
      //set Ideal temp
      lcd.setCursor(6,0);
      lcd.print(goalTemp);
      //set current temp
      lcd.setCursor(16,0);
      lcd.print(temp.read_temp());
      //showBargraph();
      break;
      
    case 3:
      lcd.clear();
      lcd.setCursor(0,0);
      //         --------------------
      lcd.print(" Enter desired temp ");
      lcd.setCursor(0,1);
      //         --------------------
      lcd.print("Temp:");
      lcd.setCursor(0,4);
      //         --------------------
      lcd.print("Back: (*) Enter: (#)");
      screen = 4;
      break;
    case 4:
      lcd.setCursor(6,1);
      lcd.print(enteredTemp);
      break;
    case 5:
      lcd.clear();
      lcd.setCursor(0,0);
      delay(100);
      //         --------------------
      lcd.print("  Enter PID values ");
      lcd.setCursor(0,1);
      delay(100);
      //         --------------------
      lcd.print(" P:    I:    D:     ");
      lcd.setCursor(0,4);
      delay(100);
      //         --------------------
      lcd.print("Back: (*) Enter: (#)");
      delay(100);
      screen = 6;
      break;
    case 6:
      lcd.setCursor(3,1);
      break;
  }
  char key = pad.getKey();
  
	incDecBar();
}

void keypadEvent(KeypadEvent key){
  switch (pad.getState()){
    case PRESSED:
      Serial.println(key);
      Serial.println("key");
      //enter info screen
      if (screen == 0 || screen == 42) {
        Serial.println("Screen = 42 || 0");
        if (key == '1') {
          screen = 1;
          break;
        } else if (key == '2') {
          screen = 3;
          break;
        } else if (key == '3') {
          screen = 5;
          break;
        }
      }
      if (screen == 2) {
        Serial.println("ent");
        if (key == '*'){
          Serial.println("ered");
          screen = 0;
          break;
        }
      }
      if (screen == 4){
        if (key != '*' && key != '#') {
          if (enteredTemp.length() < 7) {
            enteredTemp += key;
            break;
          } else {
            break;
          }
        } else if (key == '#') {
          goalTemp = enteredTemp.toInt();
          screen = 0;
          break;
        } else if (key == '*') {
          enteredTemp.remove(enteredTemp.length() - 1);
          lcd.setCursor(enteredTemp.length() + 6,1);
          lcd.print(" ");
          break;
        } else {
          Serial.println("Something went wrong");
          break;
        }
      }
      if (screen == 6) {
        updatePIDs(key);
        break;
      }
  }
}

void updatePIDs(KeypadEvent key){
  static int num = 0;
  static int PIDpos[] = {3,9,15};
  if (key == '*') {
    if (sPID[num].length() != 0){
      sPID[num].remove(sPID[num].length() - 1);
      lcd.setCursor(PIDpos[num] + sPID[num].length(),1);
      lcd.print(" ");
    } else if (num != 0){
      num--;
    } else {
      screen = 0;
      num = 0;
    }
  } else if (key == '#') {
    if (num != 2) {
      num++;
    } else {
      num = 0;
      //insert setPID methods here
      screen = 0;
    }
  } else {
    if (sPID[num].length() <= 4) {
      sPID[num] += key;
      lcd.setCursor(PIDpos[num],1);
      lcd.print(sPID[num]);
    }
  }
}
void incDecBar(){
	static int bar = 0;
	static boolean barIncrease = true;
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
