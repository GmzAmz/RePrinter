#include <Servo.h> 
#include <PID_v1.h>

double Setpoint, Input, Output; //Define Variables we'll be connecting to
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT); //Specify the links and initial tuning parameters

Servo spooler;  // a maximum of eight servo objects can be created 
Servo auger;
Servo heater;                
 
int spooler_speed(0); 
int auger_speed(0);  
int heat_level(0);

int spooler_offset(0);
int auger_offset(0);
int heater_offset(0);

void setup() 
{   
  spooler.attach(9); //attaches spooler motor controller to pin 11
  auger.attach(10);  //attaches auger motor controller to pin 6
  heater.attach(11);  //attaches heater motor controller to pin 5
 
  Serial.begin(9600);
}

void loop() 
{  
  Serial.println(Output);
  
  heat_level = analogRead(3);
  auger_speed = analogRead(2);
  spooler_speed = analogRead(1);
  
  spooler_offset = (spooler_speed+90); //Conversion from servo to motor controller
  auger_offset = (auger_speed+90);  //Conversion for servo to motor controller
  heater_offset = (heat_level+90);  //Conversion for servo to motor controller
  
  spooler.write(spooler_offset);  //Writes the spooler speed
  auger.write(auger_offset);  //Writes the auger speed
  heater.write(heater_offset);  //Writes the heater speed
}
