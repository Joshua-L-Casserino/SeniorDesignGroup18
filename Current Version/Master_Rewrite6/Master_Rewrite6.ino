#include <Adafruit_TSL2591.h>
#include <Adafruit_Sensor.h>
#include <SimpleTimer.h>
#include <SoftwareSerial.h>                                 // we have to include the SoftwareSerial library, or else we can't use it
#include <SPI.h>                                  // Needed for the light sensor
#include <Wire.h>

//  Input Data Declarations
String input_String = "";                                   // a string to hold incoming data from the PC
boolean input_string_complete = false;                    // have we received all the data from the PC
boolean Flowering = false;
boolean Lighting_On = true;
boolean pH_Listen_Flag = true;


//  MCU Time Management Declarations
#define LCD_Display_Time 1000
#define pH_pumpOn_Time 1000
#define TDS_pumpOn_Time 1000
boolean pH_Mix_Delay = true;
int test_pH_again = 0;
boolean TDS_Mix_Delay = true;
int test_TDS_again = 0;
boolean flag = true;
int last_Upate = 0;
SimpleTimer timer;

//  MCU Global Tracker
int start_up = 0;

//  Error Tracking
char* error_String_Array[] = {
  "Solution 1 empty", // pH up
  "Solution 2 empty", // pH Down
  "Solution 3 empty", // TDS
  "Solution 4 empty", // Flowering
  "Water is low",
  "TDS is high",
  "Check WiFi"
};

boolean error_Array[10] = {false};

//  pH Sensor Declarations
SoftwareSerial pH_Serial(2, 3);             // define how the soft serial port is going to work
String pH_Sensor_String = "";                               // a string to hold the data from the Atlas Scientific product
boolean pH_Sensor_String_Complete = false;                  // have we received all the data from the Atlas Scientific product
String Value_pH = " ";
double pH_High = 7.0;
double pH_Low = 6.5;


//  TDS Sensor Declarations
SoftwareSerial TDS_Serial(6, 7);            // define how the soft serial port is going to work
String TDS_Sensor_String = "";                              // a string to hold the data from the Atlas Scientific product
boolean TDS_Sensor_String_Complete = false;                 // have we received all the data from the Atlas Scientific product
String Value_TDS = " ";
int TDS_High = 1500;
int TDS_Low = 800;

//  Light Sensor Declarations
int avgLux = 0;                                     // A int to hold the LUX reading from the light sensor
byte address = 0x00;                                // Making room for the light sensor data
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);      // pass in a number for the sensor identifier (for your use later)
int visible = 0;
boolean lightChecked = true;

//  Dimmer Declarations
int Light_Increment = 0; 

//  Water Level Sensor Declarations
String waterPercentage = " ";
double reading;
double Water_Low;

//  Pump Declarations
int Pump_Counter_TDS = 300;
int Pump_Counter_Flower = 300;
int Pump_Counter_pH_Up = 300;
int Pump_Counter_pH_Down = 300;

//  System Initial Set up Function
void setup() {                                            // set up the hardware
  
    // Pin declaration
  pinMode (8, OUTPUT);
  pinMode (A0, OUTPUT);
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (10, OUTPUT);
  
  Serial.begin(9600);                                     // set baud rate for the hardware serial port_0 to 9600
  pH_Serial.begin(9600);                                  // set baud rate for the software serial port to 9600
  TDS_Serial.begin(9600);
  input_String.reserve(10);                               // set aside some bytes for receiving data from the PC
  pH_Sensor_String.reserve(30);                             // set aside some bytes for receiving data from Atlas Scientific product
  TDS_Sensor_String.reserve(30);                          // set aside some bytes for receiving data from Atlas Scientific product
  Wire.begin(1);                       // join i2c bus (address optional for master)
  tsl.setGain(TSL2591_GAIN_LOW);                // Sets the gain of the light sensor
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);         // shortest integration time (bright light)
  Wire.onReceive(Receiving_Data);
  SPI.begin();

  digitalPotWrite(Light_Increment);
  delay(2000);
  send_to_LCD("Home Hydroponics", "Loading System");

  while((Value_pH == " ") || (Value_TDS == " ") || (waterPercentage == " ") || (avgLux == 0)){
    if(pH_Listen_Flag)
      pH_Serial.listen();
      else
        TDS_Serial.listen();
    Light_Sensor();
    water_Level_Sensor();
    sensorToggle();
  }
  LCD_Display();
  
  //  Set the timers for each subsystem
  //  Min to Milli
  //  1      60000
  //  5      300000
  //  10     600000
  //  30     1800000
  //  60     3600000
  
  timer.setInterval(10000, LCD_Display);
  timer.setInterval(30000, sensorToggle);
  timer.setInterval(60000, minuteCounter);
  timer.setInterval(300000, Light_Sensor);
  timer.setInterval(300000, water_Level_Sensor);
  timer.setInterval(30000, error_Reporting);
  timer.setInterval(60000, update_Database);
}

void loop() {
  if(pH_Listen_Flag)
    pH_Serial.listen();
    else
      TDS_Serial.listen();
      
  if(test_pH_again >= 1)
    pH_Mix_Delay = false;
    
  if(test_TDS_again >= 1)
    TDS_Mix_Delay = false;
    
  if(last_Upate > 60)
    error_Array[6] = true;
    
  timer.run();
}

// Receiving data  Wire.onReceive(receiveEvent);
void Receiving_Data(int howMany){
  noInterrupts();
  // I'm planning for the data to come in this format "TDSHigh|TDSLow|pHHigh|pHLow|counterTDS|counterPHUp|counterPHDown|counterFlowering|led|flowering|"
  input_String = "";
  int temp;

  while(Wire.available() > 0){
    String temp2 = "";
    input_String = Wire.readStringUntil('|');

    if(input_String.startsWith("TDS High: ")){
      temp2 = input_String.substring(10);
      TDS_High = temp2.toInt();
      Serial.println("TDS High: " + String(TDS_High));
    }

    if(input_String.startsWith("TDS Low: ")){
      temp2 = input_String.substring(9);
      TDS_Low = temp2.toInt();
      Serial.println("TDS Low: " + String(TDS_Low));
    }

    if(input_String.startsWith("pH High: ")){
      temp2 = input_String.substring(9);
      pH_High = temp2.toFloat();
      Serial.println("pH High: " + String(pH_High));
    }

    if(input_String.startsWith("pH Low: ")){
      temp2 = input_String.substring(8);
      pH_Low = temp2.toFloat();
      Serial.println("pH Low: " + String(pH_Low));
    }

    if(input_String.startsWith("counterTDS: ")){
      temp2 = input_String.substring(12);
      Pump_Counter_TDS = temp2.toInt();
      Serial.println("counterTDS: " + String(Pump_Counter_TDS));
    }

    if(input_String.startsWith("counterPHup: ")){
      temp2 = input_String.substring(13);
      Pump_Counter_pH_Up = temp2.toInt();
      Serial.println("counterPHup: " + String(Pump_Counter_pH_Up));
    }    

    if(input_String.startsWith("counterPHdown: ")){
      temp2 = input_String.substring(15);
      Pump_Counter_pH_Down = temp2.toInt();
      Serial.println("counterPHdown: " + String(Pump_Counter_pH_Down));
    }

    if(input_String.startsWith("counterFlowering: ")){
      temp2 = input_String.substring(18);
      Pump_Counter_Flower = temp2.toInt();
      Serial.println("counterPHdown: " + String(Pump_Counter_Flower));
    }

    if(input_String.startsWith("led: ")){
      temp2 = input_String.substring(5);
      temp = temp2.toInt();
      if(temp == 0)
        Lighting_On = false;
        else
          Lighting_On = true;
      Serial.println("Lights On: " + String(Lighting_On));
    }

    if(input_String.startsWith("flowering: ")){
      temp2 = input_String.substring(11);
      temp = temp2.toInt();
      if(temp == 0)
        Flowering = false;
        else
          Flowering = true;
      Serial.println("Flowering: " + String(Flowering));
    }
  
  last_Upate = 0;
  error_Array[6] = false;
  }
  interrupts();
}

void sensorToggle(){
  if(pH_Listen_Flag)
      pH_Sensor();
      else
        TDS_Sensor();
}

void minuteCounter(){
  test_pH_again++;
  test_TDS_again++;
  last_Upate++;
}

void Requesting_Data(){
  Wire.write("wow");
}

//  Light Sensor Function
void Light_Sensor(){
  if(Lighting_On){
    for(int i = 0; i < 100; i++){
      sensor_t sensor;
      tsl.getSensor(&sensor);
      uint32_t lum = tsl.getFullLuminosity();
      uint16_t ir, full;
      ir = lum >> 16;
      full = lum & 0xFFFF;
      int lux = tsl.calculateLux(full, ir);
      if(lightChecked && (lux <= 100000)){
        avgLux = lux;
        visible = (int)lum;
        lightChecked = false;
      }
        else if(lux <= 100000){
          avgLux = (avgLux + lux)/2;
          visible = (visible + (int)lum)/2;
      }
    }
    Light_Dimmer();
  }
}

//  Dimmer Functions
void Light_Dimmer(){
    if(visible > 10500 && avgLux > 30000 && Light_Increment > 10)
      Light_Increment -= 10;
      else if(avgLux > 31000 && Light_Increment > 10)
        Light_Increment -= 10;
      else if(avgLux < 30000 && Light_Increment < 115)
        Light_Increment += 10;

    digitalPotWrite(Light_Increment);
    lightChecked = true;
}

int digitalPotWrite(int value){
  digitalWrite(10, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(10, HIGH);
}

//  TDS Sensor Functions
void print_TDS_Data(){                                 //  this function will pars the string 
  char sensorstring_array[30];                            //  we make a char array
  char *EC;                                               //  char pointer used in string parsing
  char *TDS;                                              //  char pointer used in string parsing
  char *SAL;                                              //  char pointer used in string parsing
  char *GRAV;                                             //  char pointer used in string parsing

  TDS_Sensor_String.toCharArray(sensorstring_array, 30);    //  convert the string to a char array 
  EC = strtok(sensorstring_array, ",");                   //  let's pars the array at each comma
  TDS = strtok(NULL, ",");                                //  let's pars the array at each comma
  SAL = strtok(NULL, ",");                                //  let's pars the array at each comma
  GRAV = strtok(NULL, ",");                               //  let's pars the array at each comma
  Value_TDS = TDS; 
}

void TDS_Sensor(){
    while(TDS_Serial.available() > 0) {                         // if we see that the Atlas Scientific product has sent a character
      char TDS_inchar = (char)TDS_Serial.read();            // get the char we just received
      TDS_Sensor_String += TDS_inchar;                      // add the char to the var called sensorstring
      if (TDS_inchar == '\r')                               // if the incoming character is a <CR>
    TDS_Sensor_String_Complete = true;                // set the flag
    }

    if (TDS_Sensor_String_Complete == true) {         // if a string from the Atlas Scientific product has been received in its entirety
      if (isdigit(TDS_Sensor_String[0]))           // if the first character in the string is a digit
        print_TDS_Data();                             // then call this function
      TDS_Sensor_String = "";                               // clear the string
      TDS_Sensor_String_Complete = false;                   // reset the flag used to tell if we have received a completed string from the Atlas Scientific product
      pH_Listen_Flag = true;
      TDS_Check();
    }
}

//  Water Level Sensor Function
void water_Level_Sensor(){
  float avgRead = 0;
  for(int i = 0; i < 10; i++){
    reading = analogRead(A1);
    // convert the value to resistance
    reading = (1023 / reading)  - 1;
    reading = 560 / reading;
    avgRead = (avgRead + reading)/2;
  }
  float tempOne = (1570 - avgRead)/ 973;
  int tempTwo = (tempOne * 100);
  waterPercentage = String(tempTwo);
  
  if(tempTwo < 20)
    error_Array[4] = true;
  else
    error_Array[4] = false;
    
}

//  pH Sensor Function
void pH_Sensor(){
      while(pH_Serial.available() > 0) {                //if we see that the Atlas Scientific product has sent a character
        char pH_inchar = (char)pH_Serial.read();            //get the char we just received
        pH_Sensor_String += pH_inchar;                      //add the char to the var called sensorstring
        if (pH_inchar == '\r')                              //if the incoming character is a <CR>
        pH_Sensor_String_Complete = true;               //set the flag
      }
    if (pH_Sensor_String_Complete == true) {
    if (isdigit(pH_Sensor_String[0])){
      pH_Sensor_String.remove(4);
      Value_pH = pH_Sensor_String;
      pH_Check();
    }
    
    pH_Sensor_String = "";                              //clear the string
    pH_Sensor_String_Complete = false;                  //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    pH_Listen_Flag = false;
  }
}
  
void LCD_Display(){
  if(flag){
    send_to_LCD("pH: " + Value_pH, "Tank " + waterPercentage + "% full");
    Wire.flush();
  }
    else{
      send_to_LCD("TDS: " + Value_TDS, "Lux: " + String(avgLux));
      Wire.flush();
    }
      
  toggle_Flag();
}

void toggle_Flag(){
  flag = !flag;
}
  
void send_to_LCD(String msgOne, String msgTwo){
  Wire.beginTransmission(8); // transmit to device #8
  char buffer[17];
  msgOne.getBytes(buffer,17);
  Wire.write(buffer);            
  Wire.endTransmission();    // stop transmitting
  delay(500);
  Wire.beginTransmission(8); // transmit to device #8
  msgTwo.getBytes(buffer,17);
  Wire.write(buffer);            
  Wire.endTransmission();
  delay(500);
}
  
  //  Turning on a pump
void pump_On(int pin, int time){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);        // turn the pump on
  delay(time);              // wait for 1 second
  digitalWrite(pin, LOW);       // turn the pump off
}

void update_Database(){
  send_to_LCD("Updating DB"," ");
  delay(2000);
  send_to_LCD("Upload", " ");
  
}

void pH_Check(){
  if(!pH_Mix_Delay){
    double temp_pH = Value_pH.toFloat();
    
    if((pH_Low > temp_pH) && (Pump_Counter_pH_Up > 0)){
      send_to_LCD("System Setting","pH is low");
      delay(LCD_Display_Time);
      for(int i = 3; i > 0; i--){
        String temp = String(i);
        send_to_LCD("Turning on pump","in " + temp + " seconds");
        delay(1000);
      }

    pump_On(4, pH_pumpOn_Time);
    Pump_Counter_pH_Up--;
    pH_Mix_Delay = true;
    test_pH_again = 0;
    error_Array[0] = false;
    }

    else if((pH_High < temp_pH) && (Pump_Counter_pH_Down > 0)){
      send_to_LCD("System Setting","pH is high");
      delay(LCD_Display_Time);
      for(int i = 3; i > 0; i--){
        String temp = String(i);
        send_to_LCD("Turning on pump","in " + temp + " seconds");
        delay(1000);
      }

      pump_On(5, pH_pumpOn_Time);
      Pump_Counter_pH_Down--;
      pH_Mix_Delay = true;
      test_pH_again = 0;
      error_Array[1] = false;
    }

    else if((Pump_Counter_pH_Up == 0) && (Pump_Counter_pH_Down == 0)){
      error_Array[0] = true;
      error_Array[1] = true;
    }

    else if(Pump_Counter_pH_Up == 0)
      error_Array[0] = true;

    else if(Pump_Counter_pH_Down == 0)
      error_Array[1] = true;
  }
}

void TDS_Check(){
  if(!TDS_Mix_Delay){
    int temp_TDS = Value_TDS.toInt();
    if((TDS_Low > temp_TDS) && Flowering && ((Pump_Counter_TDS > 0) && (Pump_Counter_Flower > 0))){
      send_to_LCD("System Setting","TDS is low");
      delay(LCD_Display_Time);
      for(int i = 3; i > 0; i--){
        String temp = String(i);
        send_to_LCD("Turning on pumps","in " + temp + " seconds");
        delay(1000);
      }
      pump_On(8, TDS_pumpOn_Time/2);
      Pump_Counter_TDS--;

      send_to_LCD("System Setting","Adding Nutrients");
      delay(LCD_Display_Time);
      for(int i = 3; i > 0; i--){
        String temp = String(i);
        send_to_LCD("Turning on pumps","in " + temp + " seconds");
        delay(1000);
      }
      pump_On(A0, TDS_pumpOn_Time/2);
      Pump_Counter_Flower--;
      
      test_TDS_again = 0;
      TDS_Mix_Delay = true;
      error_Array[2] = false;
      error_Array[3] = false;
    }
    else if((TDS_Low > temp_TDS) && (Pump_Counter_TDS > 0)){
      send_to_LCD("System Setting","TDS is low");
      delay(LCD_Display_Time);
      for(int i = 3; i > 0; i--){
        String temp = String(i);
        send_to_LCD("Turning on pump","in " + temp + " seconds");
        delay(LCD_Display_Time);
      }

      pump_On(8, TDS_pumpOn_Time);
      Pump_Counter_TDS -= 2;
      test_TDS_again = 0;
      TDS_Mix_Delay = true;
      error_Array[2] = false;
    }

    else if((Pump_Counter_TDS == 0) && (Pump_Counter_Flower == 0)){
      error_Array[2] = true;
      error_Array[3] = true;
    }

    else if(Pump_Counter_TDS == 0)
      error_Array[2] = true;

    else if(Pump_Counter_Flower == 0)
      error_Array[3] = true;

    else if(TDS_High > temp_TDS)
      error_Array[5] = true;
  }
}

void error_Reporting(){
  for(int i = 0; i < 10; i++){
    if(error_Array[i] == true){
    String temp = error_String_Array[i];
    if(temp.length() > 16){
      String temp2 = "";
      while(temp.length() > 16){
        temp2 = temp.substring(0, 17);
        temp.remove(0, 16);
        send_to_LCD("System Warning!", temp2);
        delay(LCD_Display_Time);
      }
      send_to_LCD("System Warning!", temp);
      delay(LCD_Display_Time);
    }
    else{
      send_to_LCD("System Warning!", error_String_Array[i]);
      delay(LCD_Display_Time);
    }
    }
  }
}


