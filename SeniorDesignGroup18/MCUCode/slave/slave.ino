// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7,8,9,10,11,12);
boolean flag = false;

void setup() {
  lcd.begin(16,2);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output

}

void loop() {  
  delay(100);
}



void toggleFlag(){
  if(flag){
    flag=false;
  }
  else
    flag=true;
  }

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  
  if(flag)
    lcd.setCursor(0,1);
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    }
    

   
    

  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    lcd.write(c);
  }
  char c = Wire.read();
  if(isAlphaNumeric(c))
    lcd.write(c);

    

  toggleFlag();
}
