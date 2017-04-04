#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7,8,9,10,11,12);
boolean flag = false;
String uploadString = "";
String pH = "";
String TDS = "";
String WaterLevel= "";
String Lux = "";
int Pump_Counter_TDS = 300;
int Pump_Counter_Flower = 300;
int Pump_Counter_pH_Up = 300;
int Pump_Counter_pH_Down = 300;
boolean gotSomeThing = false;

SoftwareSerial softSerial(2, 3);

void setup() {
  softSerial.begin(9600);
  Serial.begin(9600);
  lcd.begin(16,2);
  Wire.begin(8);                    // join i2c bus with address #8
  Wire.onReceive(LCD_Update);       // register event
  Wire.onRequest(Requesting_Data);
}

void Requesting_Data(){
  Wire.write("wow");
}

void toggleFlag(){
    flag = !flag;
  }

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void LCD_Update(int howMany) {
  noInterrupts();
  if(flag)
      lcd.setCursor(0,1);
    else{
      lcd.clear();
      lcd.setCursor(0,0);
    }
  
    String temp = "";
    char tempString[16] = {' '};
    int i = 0;
  
    while(Wire.available() > 0) { // loop through all but the last
      char c = Wire.read(); // receive byte as a character
      tempString[i] = c;
      i++;
      delay(100);
    }
  
    while(i < 16){
      tempString[i] = ' ';
      i++;
    }
    
    temp = tempString;
    
    if(temp.startsWith("Upload")){
      uploadString = "PH: " + pH + "|TDS: " + TDS + "|LUX: " + Lux + "|WATERLEVEL: " + WaterLevel + "|counterTDS: " + String(Pump_Counter_TDS) + "|counterPHup: " + String(Pump_Counter_pH_Up) + "|counterPHdown: " + String(Pump_Counter_pH_Down) + "|counterFlowering: " + String(Pump_Counter_Flower) + "|";
      softSerial.print(uploadString);
      softSerial.listen();
      delay(1000);
    }
      else{
        for(i = 0; i < 16; i++) 
          lcd.write(tempString[i]);
          
        if(temp.startsWith("pH: ")){
          pH = temp.substring(4);
          i = pH.indexOf(' ');
          if(isDigit(pH[0]))
            pH = pH.substring(0, i);
        }
          
        if(temp.startsWith("TDS: ")){
          TDS = temp.substring(5);
          i = TDS.indexOf(' ');
          if(isDigit(TDS[0]))
            TDS = TDS.substring(0, i);
        }
        if(temp.startsWith("Tank ")){
          WaterLevel = temp.substring(5);
          i = WaterLevel.indexOf('%');
          if(isDigit(WaterLevel[0]))
            WaterLevel = WaterLevel.substring(0, i);
        }
          
        if(temp.startsWith("Lux: ")){
          Lux = temp.substring(5);
          i = Lux.indexOf(' ');
          if(isDigit(Lux[0]))
            Lux = Lux.substring(0, i);
        }
  
        if(temp.startsWith("pH is low"))
          Pump_Counter_pH_Up--;
  
        if(temp.startsWith("pH is high"))
          Pump_Counter_pH_Down--;
  
        if(temp.startsWith("Adding Nutrients"))
          Pump_Counter_Flower--;
  
        if(temp.startsWith("TDS is low"))
          Pump_Counter_TDS--;
                  
        gotSomeThing = false;
      }

     if(softSerial.available()){
          String temp = "";
          String update_String = "";
          int i = 1;
        while(softSerial.available() > 0){
          char c = softSerial.read();
          Serial.print(c);
          Serial.print(i);
          if((c == '|') && (i == 1)){
            String temp = "TDS High: " + update_String + "|";
            update_String = "";
            Wire.beginTransmission(1); // transmit to device #1
            char buffer[16];
            temp.getBytes(buffer,16);
            Wire.write(buffer);       
            Wire.endTransmission(1);    // stop transmitting
            i++;
          }
          else if((c == '|') && (i == 2)){
            String temp = "TDS Low: " + update_String + "|";
            update_String = "";
            Wire.beginTransmission(1); // transmit to device #1
            char buffer[30];
            temp.getBytes(buffer,30);
            Wire.write(buffer);       
            Wire.endTransmission(1);    // stop transmitting
            i++;
          }
          else if((c == '|') && (i == 3)){
            String temp = "pH High: " + update_String + "|";
            update_String = "";
            Wire.beginTransmission(1); // transmit to device #1
            char buffer[30];
            temp.getBytes(buffer,30);
            Wire.write(buffer);       
            Wire.endTransmission(1);    // stop transmitting
            i++;
          }
          else if((c == '|') && (i == 4)){
            String temp = "pH Low: " + update_String + "|";
            update_String = "";
            Wire.beginTransmission(1); // transmit to device #1
            char buffer[30];
            temp.getBytes(buffer,30);
            Wire.write(buffer);       
            Wire.endTransmission(1);    // stop transmitting
            i++;
          }
          else if((c == '|') && (i == 5)){
            String temp = "counterTDS: " + update_String + "|";
            update_String = "";
            Wire.beginTransmission(1); // transmit to device #1
            char buffer[30];
            temp.getBytes(buffer,30);
            Wire.write(buffer);       
            Wire.endTransmission(1);    // stop transmitting
            i++;
          }
          else if((c == '|') && (i == 6)){
            String temp = "counterPHup: " + update_String + "|";
            update_String = "";
            Wire.beginTransmission(1); // transmit to device #1
            char buffer[30];
            temp.getBytes(buffer,30);
            Wire.write(buffer);       
            Wire.endTransmission(1);    // stop transmitting
            i++;
          }
          else if((c == '|') && (i == 7)){
            String temp = "counterPHdown: " + update_String + "|";
            update_String = "";
            Wire.beginTransmission(1); // transmit to device #1
            char buffer[30];
            temp.getBytes(buffer,30);
            Wire.write(buffer);       
            Wire.endTransmission(1);    // stop transmitting
            i++;
          }
          else if((c == '|') && (i == 8)){
            String temp = "counterFlowering: " + update_String + "|";
            update_String = "";
            Wire.beginTransmission(1); // transmit to device #1
            char buffer[30];
            temp.getBytes(buffer,30);
            Wire.write(buffer);       
            Wire.endTransmission(1);    // stop transmitting
            i++;
          }
          else if((c == '|') && (i == 9)){
            String temp = "led: " + update_String + "|";
            update_String = "";
            Wire.beginTransmission(1); // transmit to device #1
            char buffer[30];
            temp.getBytes(buffer,30);
            Wire.write(buffer);       
            Wire.endTransmission(1);    // stop transmitting
            i++;
          }
          else if((c == '|') && (i == 10)){
            String temp = "flowering: " + update_String + "|";
            update_String = "";
            Wire.beginTransmission(1); // transmit to device #1
            char buffer[30];
            temp.getBytes(buffer,30);
            Wire.write(buffer);       
            Wire.endTransmission(1);    // stop transmitting
            i++;
          }
          else
            update_String += c;
        }
    }
      toggleFlag();
      interrupts();
}

void loop() {
  }

