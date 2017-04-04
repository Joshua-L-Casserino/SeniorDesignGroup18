#include <Wire.h>

/*
 *  Simple HTTP get webclient test
 */

#include <ESP8266WiFi.h>

const char* ssid     = "JOSH 4051";
const char* password = "48!V263k";
const char* host = "group18.net16.net";  //db host

String equipmentID = "1234";

String TDS = "";
String PH = "";
String LUX = "";
String WATERLEVEL = "";
String dump = "";
String Pump_Counter_TDS = "";
String Pump_Counter_Flower = "";
String Pump_Counter_pH_Up = "";
String Pump_Counter_pH_Down = "";


void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(2,OUTPUT);
  pinMode(0,OUTPUT);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(2,HIGH);
    delay(500);
    digitalWrite(2,LOW);
  }

}


void loop() {
  if(Serial.available() > 0){
    while(Serial.available() > 0){
      String line = Serial.readStringUntil('|');
      
      if(line.startsWith("PH: "))
        PH = line.substring(4);
        
      if(line.startsWith("TDS: "))
        TDS = line.substring(5);
        
      if(line.startsWith("LUX: "))
        LUX = line.substring(5);
        
      if(line.startsWith("WATERLEVEL: "))
        WATERLEVEL = line.substring(12);
  
      if(line.startsWith("counterTDS: "))
        Pump_Counter_TDS = line.substring(12);
        
      if(line.startsWith("counterPHup: "))
        Pump_Counter_pH_Up = line.substring(13);
  
      if(line.startsWith("counterPHdown: "))
        Pump_Counter_pH_Down = line.substring(15);
  
      if(line.startsWith("counterFlowering: "))
        Pump_Counter_Flower = line.substring(18);
    }
    
      // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        for(int i = 0; i < 3; i++)
        {
          digitalWrite(0,HIGH);
          delay(300);
          digitalWrite(0,LOW);
          delay(300);
        }
        return;
      }
    
    // We now create a URI for the request
    String url = "/upload.php";
    String postInfo = "equipmentID=" + equipmentID + "&PH=" + PH + "&TDS=" + TDS + "&LUX=" + LUX + "&WATERLEVEL=" + WATERLEVEL + "&counterTDS=" + Pump_Counter_TDS + "&counterPHup=" + Pump_Counter_pH_Up + "&counterPHdown=" + Pump_Counter_pH_Down + "&counterFlowering=" + Pump_Counter_Flower;
    
    // This will send the request to the server
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "User-Agent: Arduino/1.0\r\n"+
                 "Connection: close\r\n"+
                 "Content-Type: application/x-www-form-urlencoded\r\n"+
                 "Content-Length: "+ postInfo.length() +"\r\n" +
                 "\r\n"+
                 postInfo);
  
    delay(1000);
      
      // parse webpage response here
      while(client.available() > 0){
        String line = client.readStringUntil('$');
        String line2 = client.readStringUntil('$');
        if(line2.length() > 10){
          Serial.println(line2);
        }
      }
    }
}



