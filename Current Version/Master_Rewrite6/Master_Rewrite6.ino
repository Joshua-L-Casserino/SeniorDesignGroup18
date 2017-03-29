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
#define FiveSeconds 5000
#define OneMinute 60000
#define FiveMinutes 300000
#define OneHour 3600000
#define LCD_Display_Time 1000
#define pH_pumpOn_Time 1000
#define TDS_pumpOn_Time 1000
boolean pH_Mix_Delay = false;
int test_pH_again = 0;
boolean TDS_Mix_Delay = false;
int test_TDS_again = 0;
boolean flag = true;
int last_Upate = 0;
SimpleTimer timer;

//  MCU Global Tracker
int start_up = 0;

//  Error Tracking
char* error_String_Array[] = {
  "pH Up solution is empty",
  "pH Down solution is empty",
  "TDS solution is empty",
  "Flowering solution is empty",
  "Water is low",
  "TDS is high",
  "System needs updating"
};

boolean error_Array[10] = {false};

//  pH Sensor Declarations
#define pH_RX 2                         // define what pin rx is going to be
#define pH_TX 3                         // define what pin tx is going to be
SoftwareSerial pH_Serial(pH_RX, pH_TX);             // define how the soft serial port is going to work
String pH_Sensor_String = "";                               // a string to hold the data from the Atlas Scientific product
boolean pH_Sensor_String_Complete = false;                  // have we received all the data from the Atlas Scientific product
String Value_pH;
double pH_High = 7.0;
double pH_Low = 6.5;


//  TDS Sensor Declarations
#define TDS_RX 6                        // define what pin rx is going to be
#define TDS_TX 7                        // define what pin tx is going to be
SoftwareSerial TDS_Serial(TDS_RX, TDS_TX);            // define how the soft serial port is going to work
String TDS_Sensor_String = "";                              // a string to hold the data from the Atlas Scientific product
boolean TDS_Sensor_String_Complete = false;                 // have we received all the data from the Atlas Scientific product
String Value_TDS = "";
int TDS_High = 1500;
int TDS_Low = 800;

//  Light Sensor Declarations
int avgLux = 0;                                               // A int to hold the LUX reading from the light sensor
byte address = 0x00;                                // Making room for the light sensor data
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);            // pass in a number for the sensor identifier (for your use later)
int visible = 0;

//  Dimmer Declarations
#define dimmer_pin 10
int Light_Increment = 0; 

//  Water Level Sensor Declarations
#define SENSORPIN A1
#define SERIESRESISTOR 560
String waterPercentage = ""; 
double reading;
double Water_Low;

//  Pump Declarations
#define Pump_TDS 8
#define Pump_Flower A0
#define Pump_pH_Up 4
#define Pump_pH_Down 5
int Pump_Counter_TDS = 300;
int Pump_Counter_Flower = 300;
int Pump_Counter_pH_Up = 300;
int Pump_Counter_pH_Down = 300;

//  System Initial Set up Function
void setup() {                                            // set up the hardware
  
    // Pin declaration
  pinMode (Pump_TDS, OUTPUT);
  pinMode (Pump_Flower, OUTPUT);
  pinMode (Pump_pH_Up, OUTPUT);
  pinMode (Pump_pH_Down, OUTPUT);
  pinMode (dimmer_pin, OUTPUT);
  
  Serial.begin(9600);                                     // set baud rate for the hardware serial port_0 to 9600
  pH_Serial.begin(9600);                                  // set baud rate for the software serial port to 9600
  TDS_Serial.begin(9600);
  input_String.reserve(10);                               // set aside some bytes for receiving data from the PC
  pH_Sensor_String.reserve(30);                             // set aside some bytes for receiving data from Atlas Scientific product
  TDS_Sensor_String.reserve(30);                          // set aside some bytes for receiving data from Atlas Scientific product
  Wire.begin();                       // join i2c bus (address optional for master)
  tsl.setGain(TSL2591_GAIN_LOW);                // Sets the gain of the light sensor
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);         // shortest integration time (bright light)
  Wire.onReceive(Receiving_Data);
  Wire.onRequest(Requesting_Data);
  SPI.begin();

  digitalPotWrite(Light_Increment);

  pH_Serial.listen();
  String temp = "";
  send_to_LCD("Home Hydroponics", "Group 18");
  delay(2000);
  for(int i = 0; i < 34; i++){
    send_to_LCD("Loading System", temp);
    water_Level_Sensor();
    Light_Sensor();
    if(pH_Listen_Flag)
      pH_Serial.listen();
      else
        TDS_Serial.listen();
    pH_Sensor();
    TDS_Sensor();
    if(temp.length() < 16)
      temp += ".";
      else
        temp = ""; 
  }
  LCD_Display();

  //  Set the timers for each subsystem
  timer.setInterval(FiveSeconds, pH_Sensor);
  timer.setInterval(FiveSeconds, TDS_Sensor);
  timer.setInterval(FiveSeconds, water_Level_Sensor);
  timer.setInterval(3000, Light_Sensor);
  timer.setInterval(30000, Light_Dimmer);
  timer.setInterval(FiveSeconds, LCD_Display);
  timer.setInterval(FiveMinutes, update_Database);
  timer.setInterval(OneMinute, error_Reporting);
  timer.setInterval(OneMinute, minuteCounter);
}

// Receiving data  Wire.onReceive(receiveEvent);
void Receiving_Data(){
  // I'm planning for the data to come in this format "TDSHigh|TDSLow|pHHigh|pHLow|counterTDS|counterPHUp|counterPHDown|counterFlowering|led|flowering|"
  input_String = " ";
  int temp;

  if(Wire.available() > 0){
    input_String = Wire.readStringUntil('|');
    TDS_High = input_String.toInt();
    
    input_String = Wire.readStringUntil('|');
    TDS_Low = input_String.toInt();
    
    input_String = Wire.readStringUntil('|');
    pH_High = input_String.toFloat();
    
    input_String = Wire.readStringUntil('|');
    pH_Low = input_String.toFloat();
    
    input_String = Wire.readStringUntil('|');
    Pump_Counter_TDS = input_String.toInt();

    input_String = Wire.readStringUntil('|');
    Pump_Counter_pH_Up = input_String.toInt();

    input_String = Wire.readStringUntil('|');  
    Pump_Counter_pH_Down = input_String.toInt();

    input_String = Wire.readStringUntil('|'); 
    Pump_Counter_Flower = input_String.toInt();

    input_String = Wire.readStringUntil('|');
    temp = input_String.toInt();
    if(temp == 0)
      Lighting_On = false;
      else
        Lighting_On = true;
    
    input_String = Wire.readStringUntil('|');
    temp = input_String.toInt();
    if(temp == 0)
      Flowering = false;
      else
        Flowering = true;
  
  last_Upate = 0;
  error_Array[6] = false;
  }
}

void minuteCounter(){
  test_pH_again++;
  test_TDS_again++;
  last_Upate++;
}

void Requesting_Data(){
  Wire.write("update");
}

//  Light Sensor Function
void Light_Sensor(){
  if(Lighting_On){
    for(int i = 0; i < 10; i++){
      sensor_t sensor;
      tsl.getSensor(&sensor);
      uint32_t lum = tsl.getFullLuminosity();
      uint16_t ir, full;
      ir = lum >> 16;
      full = lum & 0xFFFF;
      if(avgLux = 0){
        avgLux = tsl.calculateLux(full, ir);
        visible = (int)lum;
      }
        else{
          avgLux = (avgLux + tsl.calculateLux(full, ir))/2;
          visible = (visible + (int)lum)/2;
      }
    }
  }
}

//  Dimmer Functions
void Light_Dimmer(){
  if(Lighting_On){
    if(visible > 10500 && avgLux > 30000 && Light_Increment > 10)
      Light_Increment -= 10;
      else if(avgLux > 31000 && Light_Increment > 10)
        Light_Increment -= 10;
      else if(avgLux < 30000 && Light_Increment < 115)
        Light_Increment += 10;

    digitalPotWrite(Light_Increment);
    avgLux = 0;
  }
}

int digitalPotWrite(int value){
  digitalWrite(dimmer_pin, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(dimmer_pin, HIGH);
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
  Serial.print("TDS: ");
  Serial.println(TDS);                                    //  this is the TDS value
  Value_TDS = TDS; 
}

void TDS_Sensor(){
  if(!pH_Listen_Flag){
      if (TDS_Serial.available() > 0) {                         // if we see that the Atlas Scientific product has sent a character
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
}

//  Water Level Sensor Function
void water_Level_Sensor(){
  float avgRead;
  for(int i = 0; i < 10; i++){
    reading = analogRead(SENSORPIN);
    // convert the value to resistance
    reading = (1023 / reading)  - 1;
    reading = SERIESRESISTOR / reading;
    avgRead = (avgRead + reading)/2;
  }
  float tempOne = (1570 - avgRead)/ 973;
  int tempTwo = tempOne * 100;
  waterPercentage = String(tempTwo);
  
  if(tempTwo < 20)
    error_Array[4] = true;
  else
    error_Array[4] = false;
    
}

//  pH Sensor Function
void pH_Sensor(){
  if(pH_Listen_Flag){
      if (pH_Serial.available() > 0) {                //if we see that the Atlas Scientific product has sent a character
        char pH_inchar = (char)pH_Serial.read();            //get the char we just received
        pH_Sensor_String += pH_inchar;                      //add the char to the var called sensorstring
        if (pH_inchar == '\r')                              //if the incoming character is a <CR>
      pH_Sensor_String_Complete = true;               //set the flag
      }
    if (pH_Sensor_String_Complete == true) {
    if (isdigit(pH_Sensor_String[0])){
      pH_Sensor_String.remove(3);
      Value_pH = pH_Sensor_String;
      pH_Check();
    }
    
    pH_Sensor_String = "";                              //clear the string
    pH_Sensor_String_Complete = false;                  //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    pH_Listen_Flag = false;
  }
  }
}

void loop() {
  if(pH_Listen_Flag)
    pH_Serial.listen();
    else
      TDS_Serial.listen();
  if(test_pH_again > 1)
    pH_Mix_Delay = false;
  if(test_TDS_again > 1)
    TDS_Mix_Delay = false;
  if(last_Upate > 60)
    error_Array[6] = true;
    
  timer.run();
}
  
void LCD_Display(){
  if(flag)
    send_to_LCD("pH: " + Value_pH, "Tank " + waterPercentage + "% full");
    else
      send_to_LCD("TDS: " + Value_TDS, "Lux: " + String(avgLux));
      
  toggle();
}

void toggle(){
  flag = !flag;
}
  
void send_to_LCD(String msgOne, String msgTwo){
  Wire.beginTransmission(8); // transmit to device #8
  char buffer[17];
  msgOne.getBytes(buffer,17);
  Wire.write(buffer);            
  Wire.endTransmission();    // stop transmitting
  Wire.beginTransmission(8); // transmit to device #8
  msgTwo.getBytes(buffer,17);
  Wire.write(buffer);            
  Wire.endTransmission();
}
  
  //  Turning on a pump
void pump_On(int pin, int time){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);        // turn the pump on
  delay(time);              // wait for 1 second
  digitalWrite(pin, LOW);       // turn the pump off
}

void update_Database(){
  String database_String = "Upload|PH: " + Value_pH + "|TDS: " + Value_TDS + "|LUX: " + String(avgLux) +  "|WATERLEVEL: " + waterPercentage + "|";
  send_to_LCD(database_String,"");
  delay(100);
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

    pump_On(Pump_pH_Up, pH_pumpOn_Time);
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

      pump_On(Pump_pH_Down, pH_pumpOn_Time);
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

      pump_On(Pump_TDS, TDS_pumpOn_Time/2);
      Pump_Counter_TDS--;
      pump_On(Pump_Flower, TDS_pumpOn_Time/2);
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

      pump_On(Pump_TDS, TDS_pumpOn_Time);
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
