#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <SimpleTimer.h>

LiquidCrystal lcd(7,8,9,10,11,12);
boolean flag = false;
SimpleTimer timer;
String uploadString = "";

SoftwareSerial softSerial(2, 3);

void setup() {
  //Wire.begin(); // join i2c bus (address optional for master)
  softSerial.begin(9600);
  lcd.begin(16,2);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(LCD_Update); // register event
}

void toggleFlag(){
    flag = !flag;
  }

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void LCD_Update(int howMany) {
  
  if(flag)
    lcd.setCursor(0,1);

  else{
    lcd.clear();
    lcd.setCursor(0,0);
  }

  char tempString[16];
  int i = 0;

  while (Wire.available() > 0) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    tempString[i] = c;
    i++;
  }
  
  while(i < 16){
    tempString[i] = ' ';
    i++;
  }

  int a;
  String temp = tempString;
  String temp2 = "";

  a = temp.indexOf('|');
  temp2 = temp.substring(0, a);
  if(temp == "Upload")
  {
    uploadString = temp2.substring(a+1);
    softSerial.print(uploadString);
    delay(100);
    profile_Update();
    toggleFlag();
  }
    else{
      for(int i = 0; i < 17; i++) 
        lcd.write(tempString[i]);
      toggleFlag();
    }

}

void profile_Update(){
  if(softSerial.available()){
    String update_String = "";
    int i = 0;
    while(softSerial.available() >= 0){
      char c = softSerial.read();
      update_String += c;
      i++;
    }

    Wire.beginTransmission(1); // transmit to device #8
    char buffer[i];
    update_String.getBytes(buffer,i);
    Wire.write(buffer);            
    Wire.endTransmission(1);    // stop transmitting
  }
}


void loop() {
}
