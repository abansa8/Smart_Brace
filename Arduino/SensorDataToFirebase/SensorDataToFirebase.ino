
//Example shows how to connect to Firebase RTDB and perform basic operation for set, get, push and update data to database
//Required WiFiNINA Library for Arduino from https://github.com/arduino-libraries/WiFiNINA


//Include Libraries
#include "Firebase_Arduino_WiFiNINA.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include "time.h"
#include <Arduino_LSM6DS3.h>
#include<Wire.h>


//Global Variables
const int MPU_addr=0x68; int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int minVal=265; int maxVal=402; 
int x; int y; int z;
int Counter;

String path = "/GyroReadings";
String jsonStr;

//Authentications
#define FIREBASE_HOST "esp32test-acf49.firebaseio.com"
#define FIREBASE_AUTH "uXprbUT4LSXNBSk5N5qsnnJPxYoMBtqVZfzryfhk"
#define WIFI_SSID "SM-G955W7932"
#define WIFI_PASSWORD "aryamanbansal"

//Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//Variables to hold date and time
String formattedDate;
String dayStamp;
String timeStamp;

//Define Firebase data object
FirebaseData firebaseData;

void setup()
{
  //Start Gyro Transmission
  Wire.begin(); 
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x6B); Wire.write(0); 
  Wire.endTransmission(true); 

  Serial.begin(115200);
  delay(100);

  Serial.println();

  //Establish Wi-Fi Connection
  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Init the NTPClient
  timeClient.begin();
  timeClient.setTimeOffset(-18000); //May have to change offset - depends on time zone


  //Fill the authentication data
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);


 
//      //Pushing Data
//  Serial.println("-----------------------------------");
//  Serial.println("----------Begin Push Test----------");
//  Serial.println("-----------------------------------");
//  Serial.println();
//
//  for (uint8_t i = 0; i < 5; i++)
//  {
//
//    if (Firebase.pushInt(firebaseData, path + "/Push/Int", (i + 1)))
//    {
//      Serial.println("----------Push result-----------");
//      Serial.println("PATH: " + firebaseData.dataPath());
//      Serial.print("PUSH NAME: ");
//      Serial.println(firebaseData.pushName());
//      Serial.println("--------------------------------");
//      Serial.println();
//    }
//    else
//    {
//      Serial.println("----------Can't push data--------");
//      Serial.println("REASON: " + firebaseData.errorReason());
//      Serial.println("--------------------------------");
//      Serial.println();
//    }
//  }

//
//
//  for (uint8_t i = 5; i < 10; i++)
//  {
//
//    jsonStr = "{\"Data" + String(i + 1) + "\":" + String(i + 1) + "}";
//
//    if (Firebase.pushJSON(firebaseData, path + "/Push/Int", jsonStr))
//    {
//      Serial.println("----------Push result-----------");
//      Serial.println("PATH: " + firebaseData.dataPath());
//      Serial.print("PUSH NAME: ");
//      Serial.println(firebaseData.pushName());
//      Serial.println("--------------------------------");
//      Serial.println();
//    }
//    else
//    {
//      Serial.println("----------Can't push data--------");
//      Serial.println("REASON: " + firebaseData.errorReason());
//      Serial.println("--------------------------------");
//      Serial.println();
//    }
//  }


////Update Data
//  Serial.println("-----------------------------------");
//  Serial.println("---------Begin Update Test----------");
//  Serial.println("-----------------------------------");
//  Serial.println();
//
//  for (uint8_t i = 0; i < 5; i++)
//  {
//
//    jsonStr = "{\"Data" + String(i + 1) + "\":" + String(i + 5.5) + "}";
//
//    if (Firebase.updateNode(firebaseData, path + "/Int", jsonStr))
//    {
//      Serial.println("----------Update result-----------");
//      Serial.println("PATH: " + firebaseData.dataPath());
//      Serial.println("TYPE: " + firebaseData.dataType());
//      Serial.print("VALUE: ");
//      if (firebaseData.dataType() == "int")
//        Serial.println(firebaseData.intData());
//      else if (firebaseData.dataType() == "float")
//        Serial.println(firebaseData.floatData());
//      else if (firebaseData.dataType() == "boolean")
//        Serial.println(firebaseData.boolData() == 1 ? "true" : "false");
//      else if (firebaseData.dataType() == "string")
//        Serial.println(firebaseData.stringData());
//      else if (firebaseData.dataType() == "json")
//        Serial.println(firebaseData.jsonData());
//      Serial.println("--------------------------------");
//      Serial.println();
//    }
//    else
//    {
//      Serial.println("----------Can't Update data--------");
//      Serial.println("REASON: " + firebaseData.errorReason());
//      Serial.println("--------------------------------");
//      Serial.println();
//    }
//  }
}

void loop()
{

//Print and Tracking Current Time 
while (!timeClient.update()){
  timeClient.forceUpdate();
}
  formattedDate = timeClient.getFormattedTime();
  
  //Serial.println(formattedDate);


//Gyro Sensor Readings
Wire.beginTransmission(MPU_addr); 
  Wire.write(0x3B); 
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU_addr,14,true); 
  AcX=Wire.read()<<8|Wire.read(); 
  AcY=Wire.read()<<8|Wire.read(); 
  AcZ=Wire.read()<<8|Wire.read();

  int xAng = map(AcX,minVal,maxVal,-90,90); 
  int yAng = map(AcY,minVal,maxVal,-90,90); 
  int zAng = map(AcZ,minVal,maxVal,-90,90);


  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); 
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI); 
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);


  //Serial.print("Sensor AngleX = "); Serial.println(x);
  //Serial.print("Sensor AngleY = "); Serial.println(y);
  //Serial.print("Sensor AngleZ = "); Serial.println(z);


      
  //Arduino Gyro Readings
  
  float xArd, yArd, zArd, pitch;

      //if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(xArd,yArd,zArd);
        pitch = 180 * atan2(xArd, sqrt(yArd*yArd + zArd*zArd))/PI;
        
        //Serial.println(pitch);

        //Calculated Angle Difference
      float CalcAngle = y - pitch;

//Set Data

  Serial.println("-----------------------------------");
  Serial.println("----------Begin Set Test-----------");
  Serial.println("-----------------------------------");
  Serial.println();

    if (Firebase.setInt(firebaseData, path + "/Int/Time: " + formattedDate, y))
    {
      Serial.println("----------Set result-----------");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "int")
        Serial.println(firebaseData.intData());
      else if (firebaseData.dataType() == "float")
        Serial.println(firebaseData.floatData());
      else if (firebaseData.dataType() == "boolean")
        Serial.println(firebaseData.boolData() == 1 ? "true" : "false");
      else if (firebaseData.dataType() == "string")
        Serial.println(firebaseData.stringData());
      else if (firebaseData.dataType() == "json")
        Serial.println(firebaseData.jsonData());
      Serial.println("--------------------------------");
      Serial.println();
    }


}
