

#include <Wire.h>
#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#include <Adafruit_TSL2591.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>                    // Needed for the light sensor
#define PHrx 2                                          //define what pin rx is going to be
#define PHtx 3                                          //define what pin tx is going to be
#define ECrx 6                                          //define what pin rx is going to be
#define ECtx 7                                         //define what pin tx is going to be
#define TDSpump 8
#define FLOWERpump A0
#define PHUPpump 4
#define PHDOWNpump 5


// the value of the 'other' resistor
#define SERIESRESISTOR 1500    
 
// What pin to connect the sensor to
#define SENSORPIN A1

SoftwareSerial ECserial(ECrx, ECtx);                      //define how the soft serial port is going to work
SoftwareSerial PHserial(PHrx, PHtx);                      //define how the soft serial port is going to work

String inputstring = "";                              //a string to hold incoming data from the PC
String PHsensorstring = "";                             //a string to hold the data from the Atlas Scientific product
int avgLux;                                         // A int to hold the LUX reading from the light sensor
boolean input_string_complete = false;                //have we received all the data from the PC
boolean PHsensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
String ECsensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean ECsensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
String PHvalue;
String ECvalue;
int interval=3000;   // slace update interval
unsigned long previousMillis=0;
float reading;
byte address = 0x00;                  // Making room for the light sensor data
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);      // pass in a number for the sensor identifier (for your use later)
bool on = true;


void configureLightSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  tsl.setGain(TSL2591_GAIN_LOW);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
}

void setup() {                                        //set up the hardware
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  PHserial.begin(9600);                               //set baud rate for the software serial port to 9600
  ECserial.begin(9600);
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  PHsensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
  ECsensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
  Wire.begin(); // join i2c bus (address optional for master)
  configureLightSensor();
  //PHvalue = "00.0";
  //ECvalue = "0000";
  //reading = 0.0;
  
}

void advancedRead(void)
{
    sensor_t sensor;
    tsl.getSensor(&sensor);
    uint32_t lum = tsl.getFullLuminosity();
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;
    avgLux = tsl.calculateLux(full, ir);
    /*
    for(int i = 1; i < 10; i++)
    {
      uint32_t lum = tsl.getFullLuminosity();
      uint16_t ir, full;
      ir = lum >> 16;
      full = lum & 0xFFFF;

      Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
      Serial.print("IR: "); Serial.print(ir);  Serial.print("  ");
      Serial.print("Full: "); Serial.print(full); Serial.print("  ");
      Serial.print("Visible: "); Serial.print(full - ir); Serial.print("  ");
      Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));
      
      
      avgLux = (avgLux + tsl.calculateLux(full, ir))/2;
    }
    */
}

void print_EC_data(void) {                            //this function will pars the string  

  char sensorstring_array[30];                        //we make a char array
  char *EC;                                           //char pointer used in string parsing
  char *TDS;                                          //char pointer used in string parsing
  char *SAL;                                          //char pointer used in string parsing
  char *GRAV;                                         //char pointer used in string parsing
  float f_ec;                                         //used to hold a floating point number that is the EC
  
  ECsensorstring.toCharArray(sensorstring_array, 30);   //convert the string to a char array 
  EC = strtok(sensorstring_array, ",");               //let's pars the array at each comma
  TDS = strtok(NULL, ",");                            //let's pars the array at each comma
  SAL = strtok(NULL, ",");                            //let's pars the array at each comma
  GRAV = strtok(NULL, ",");                           //let's pars the array at each comma
  Serial.println(TDS);                                //this is the TDS value
  ECvalue=TDS; 
//f_ec= atof(EC);                                     //uncomment this line to convert the char to a float
}

void loop() {
  
  //Water Level section
  reading = analogRead(SENSORPIN);
  //convert the value to resistance
  reading = (1023 / reading)  - 1;
  reading = SERIESRESISTOR / reading;
    
  //PH section
 
  if (PHsensor_string_complete == true) { 
    PHsensorstring.remove(4);
    PHvalue = PHsensorstring;
    Serial.print(PHvalue);
    Serial.print("\n");
    PHsensorstring = "";                                //clear the string
    PHsensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    ECserial.listen();
   }

  
   if (PHserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char PHinchar = (char)PHserial.read();              //get the char we just received
    PHsensorstring += PHinchar;                           //add the char to the var called sensorstring
    if (PHinchar == '\r') {                             //if the incoming character is a <CR>
      PHsensor_string_complete = true;                  //set the flag
    }
  }
  
  
  
  //EC section
 
  if (ECsensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(ECsensorstring[0]) == false) {          //if the first character in the string is a digit
      Serial.println(ECsensorstring);                   //send that string to the PC's serial monitor
    }
    else                                              //if the first character in the string is NOT a digit
    {
      print_EC_data();                                //then call this function 
    }
    ECsensorstring = "";                                //clear the string
    ECsensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    PHserial.listen();
  }

   if (ECserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char ECinchar = (char)ECserial.read();              //get the char we just received
    ECsensorstring += ECinchar;                           //add the char to the var called sensorstring
    if (ECinchar == '\r') {                             //if the incoming character is a <CR>
      ECsensor_string_complete = true;                  //set the flag
    }
  } 

    //JOSHS BULLSHIT
   advancedRead();
    
   //Timing code
   unsigned long currentMillis = millis();
   unsigned long currentMillis2 = millis();
   
  
   // How much time has passed, accounting for rollover with subtraction!
   if ((unsigned long)(currentMillis - previousMillis) >= interval) {
      // It's time to do something!
      sendLine1ToSlave();
      sendLine2ToSlave();
      // Use the snapshot to set track time until next event
      previousMillis = currentMillis;
   }
  
  if(on){
    on = false; 
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(5000);                       // wait for a second
    digitalWrite(5, LOW);
  }
   
      

/*
  //request message from slave (device,#ofBytes)
  //we need to know how many bytes to request
  Wire.requestFrom(8,3);
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
    
  }
  delay(500);
  */
}

void sendLine1ToSlave(){

    Wire.beginTransmission(8); // transmit to device #8
    char buffer[16];
    String temp = "PH:" + PHvalue+" EC:" + ECvalue;
    temp.getBytes(buffer,16);
    Wire.write(buffer);            
    Wire.endTransmission();    // stop transmitting
  }
  
void sendLine2ToSlave(){

    Wire.beginTransmission(8); // transmit to device #8
    char buffer[16];
    Wire.write("R:");
    String temp= String(reading);
    temp.remove(4);
    temp.getBytes(buffer,16);
    Wire.write(buffer);
    
    Wire.write(" Lux:");
    String(avgLux).getBytes(buffer,16);
    Wire.write(buffer);
    Wire.endTransmission();    // stop transmitting
  }
