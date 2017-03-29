#include <Wire.h>
#include <ESP8266WiFi.h>
#include <SimpleTimer.h>


// wifi info
const char* ssid     = "JOSH 4051";
const char* password = "48!V263k";
const char* host = "group18.net16.net";  //db host

String equipmentID = "1234";

void setup() {
  // Use WiFiClient class to create TCP connections
  Serial.begin(9600);
  delay(100);
  pinMode(2,OUTPUT);
  pinMode(0,OUTPUT);
  
  //  We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(2,HIGH);
    delay(500);
    digitalWrite(2,LOW);
  }
}

void database_Upload(){
  if(Serial.available() > 0){
    WiFiClient client;
    String TDS = "";
    String PH = "";
    String LUX = "";
    String WATERLEVEL = "";
    boolean update_MCU = false;
/*
    // parse webpage response here
    while(client.available()){
    String line = client.readStringUntil('\r');
    }
*/
    while(Serial.available()>0){
      String line = Serial.readStringUntil('|');

      if(line.startsWith("PH: "))
        PH = line.substring(4);
      
      if(line.startsWith("TDS: "))
        TDS = line.substring(5);
      
      if(line.startsWith("LUX: "))
        LUX = line.substring(5);
      
      if(line.startsWith("WATERLEVEL: "))
        WATERLEVEL = line.substring(12);
    }

    boolean update_Database = ((PH.length() > 0) && (TDS.length() > 0) && (LUX.length() > 0) && (WATERLEVEL.length() > 0));

    if(update_Database){
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        for(int i = 0; i < 3; i++ ){
          digitalWrite(0,HIGH);
          delay(300);
          digitalWrite(0,LOW);
          delay(300);
        }
      return;
      } 

      // We now create a URI for the request
      String url = "/upload.php";
      String postInfo = "equipmentID=" + equipmentID + "&PH=" + PH + "&TDS=" + TDS + "&LUX=" + LUX + "&WATERLEVEL=" + WATERLEVEL;

      // This will send the request to the server
      client.print(String("POST ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" + 
      "User-Agent: Arduino/1.0\r\n" +
      "Connection: close\r\n" +
      "Content-Type: application/x-www-form-urlencoded\r\n" +
      "Content-Length: " + postInfo.length() + "\r\n" +
      "\r\n" + postInfo);

      delay(1000);

      if (!client.connect(host, httpPort)) {
        for(int i = 0; i < 3; i++ ){
          digitalWrite(0,HIGH);
          delay(300);
          digitalWrite(0,LOW);
          delay(300);
        }
        
        return;
      } 

      // We now create a URI for the request
      url = "/MCU_Upload.php";
      postInfo = "equipmentID=" + equipmentID;

      // This will send the request to the server
      client.print(String("POST ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" + 
      "User-Agent: Arduino/1.0\r\n" +
      "Connection: close\r\n" +
      "Content-Type: application/x-www-form-urlencoded\r\n" +
      "Content-Length: " + postInfo.length() + "\r\n" +
      "\r\n" + postInfo);

      delay(1000);

      // parse webpage response here
      if(client.available()){
        String line = client.readStringUntil('\r');
        //Serial.println(line);
        Serial.print(line);
      }
    } 
  }
}

void loop() {
  database_Upload();
}
