#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <I2C_Anything.h>

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
String db_TDS_High;
String db_TDS_Low;
String db_pH_High;
String db_pH_Low;
String db_Counter_TDS_High;
String db_Counter_TDS_Low;
String db_Counter_pH_High;
String db_Counter_pH_Low;
String db_LED;
String db_Flowering;


SoftwareSerial softSerial(2, 3);

void setup() {
  softSerial.begin(9600);
  Serial.begin(9600);
  lcd.begin(16,2);
  Wire.begin(8);                    // join i2c bus with address #8
  Wire.onReceive(LCD_Update);       // register event
  Wire.onRequest(Requesting_Data);
}

void Requesting_Data(int howMany){
  if((howMany == 1) && (gotSomeThing))
    Wire.write(1);
  gotSomeThing = false;

  
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
      }
      
      toggleFlag();
      gotSomeThing = true;
}

void loop() {
  /*if(softSerial.available()){
    noInterrupts();
    String temp = "";
    String update_String = "";
    int i = 1;
    while(softSerial.available() > 0){
      char c = softSerial.read();
      
      if((c == '|') && (i == 1)){
        db_TDS_High = "TDS High: " + update_String + "|";
        update_String = "";
        i++;
      }
      else if((c == '|') && (i == 2)){
        db_TDS_Low = "TDS Low: " + update_String + "|";
        update_String = "";
        i++;
      }
      else if((c == '|') && (i == 3)){
        db_pH_High = "pH High: " + update_String + "|";
        update_String = "";
        i++;
      }
      else if((c == '|') && (i == 4)){
        db_pH_Low = "pH Low: " + update_String + "|";
        update_String = "";
        i++;
      }
      else if((c == '|') && (i == 5)){
        db_Counter_TDS = "counterTDS: " + update_String + "|";
        update_String = "";
        i++;
      }
      else if((c == '|') && (i == 6)){
        db_Counter_Flowering = "counterPHup: " + update_String + "|";
        update_String = "";
        i++;
      }
      else if((c == '|') && (i == 7)){
        db_Counter_pH_High = "counterPHdown: " + update_String + "|";
        update_String = "";
        i++;
      }
      else if((c == '|') && (i == 8)){
        db_Counter_pH_Low = "counterFlowering: " + update_String + "|";
        update_String = "";
        i++;
      }
      else if((c == '|') && (i == 9)){
        db_LED = "led: " + update_String + "|";
        update_String = "";
        i++;
      }
      else if((c == '|') && (i == 10)){
        db_Flowering = "flowering: " + update_String + "|";
        update_String = "";
        i++;
      }
      else
        update_String += c;
    }
    interrupts();
  }*/
}

