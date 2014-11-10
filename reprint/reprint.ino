#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



#define BACKLIGHT_PIN     13

//LiquidCrystal_I2C lcd(0x3F);  // Set the LCD I2C address

//LiquidCrystal_I2C lcd(0x3F, BACKLIGHT_PIN, POSITIVE);  // Set the LCD I2C address


LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//// Creat a set of new characters
//const uint8_t charBitmap[][8] = {
//   { 0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0 },
//   { 0x6, 0x9, 0x9, 0x6, 0, 0, 0, 0 },
//   { 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0, 0x0 },
//   { 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0, 0x0 },
//   { 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0x0 },
//   { 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0x0 },
//   { 0x0, 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0 },
//   { 0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0 }
//   
//};

void setup()
{
//   int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));

//  // Switch on the backlight
//  pinMode ( BACKLIGHT_PIN, OUTPUT );
//  digitalWrite ( BACKLIGHT_PIN, LOW );
  
  lcd.begin(20,4);               // initialize the lcd 

//   for ( int i = 0; i < charBitmapSize; i++ )
//   {
//      lcd.createChar ( i, (uint8_t *)charBitmap[i] );
//   }

  lcd.home ();                   // go home
  lcd.print("Hello, ARDUINO ");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print (" FORUM - fm   ");
  delay ( 200 );
  lcd.clear();
  //--------------------
  //--Pppp--Iiii--Dddd--
  lcd.setCursor(0,1);
  lcd.print("--PXXX--IXXX--DXXX--");
  //--------------------
  //ideal iiii curr cccc
  lcd.setCursor(0,0);
  lcd.print("ideal-XXXX-curr-XXXX");
  delay(1000);
}
void loop()
{
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

