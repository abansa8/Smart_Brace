
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

#define MOD_COUNTER 800
#define MAX_RANGE 45
#define MIN_RANGE -45

//Global Variables
const int MPU_addr=0x68; int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int minVal=265; int maxVal=402; 
int x; int y; int z;
int Counter;
int id= 46;
float CalcAngle;

//WRIST
#define WRIST_MOTION__DEADBAND 70
#define WRIST_MOTION_TRIGGER 3
bool streaching = false; int stageOneComplete = 0; int wristMotion = 0;
int streachCounter = 0;

//BODY
#define BODY_MOTION_DEADBAND
int bodyMotion = 0;

//VIBRATIONS
int vibPin = 2;
int vibTrigger = 0; int vibWait = 0;
#define VIB_TRIGGER_COUNTER 5
#define VIB_WAIT_COUNTER 3

//FIREBASE
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


   if (!IMU.begin()) {
    Serial.println("IMU Uninitialized!");

    while (1);
  }
  
  delay(100);

  //Vibration motor OUTPUT
  pinMode(vibPin, OUTPUT);

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
}

void loop()
{
  //Print and Tracking Current Time 
  while (!timeClient.update())
  {
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

      
  //Arduino Gyro Readings
  
  float xArd, yArd, zArd, pitch;

 if (IMU.accelerationAvailable()) {
  IMU.readAcceleration(xArd,yArd,zArd);

  pitch = 180 * atan2(xArd, sqrt(yArd*yArd + zArd*zArd))/PI;
 }
  //Calculated Angle Difference
  CalcAngle = y - pitch - 10;

  if(Counter > MOD_COUNTER)
  {
    //Reset Counter
    id++;
    Counter = 0;

  if(CalcAngle > 180)
  {
    CalcAngle = CalcAngle - 360;
  }
       
    //Set Data
    Serial.println("-----------------------------------");
    Serial.println("------------Begin Send-------------");
    Serial.println("-----------------------------------");
    Serial.println();

      if (Firebase.setInt(firebaseData, path + "/" + String(id), CalcAngle))
      {
        Serial.println("----------Set Angle-----------");
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
  
      //Updating vibration motor settings
      vibrationHeartbeat();

      //Updating WristMotion
      updateWristMotion();
  }
  else
  {
    Counter++;
  }


}

void updateWristMotion()
{
  Serial.println("In Wrist");
  if((CalcAngle > WRIST_MOTION_TRIGGER || CalcAngle < -WRIST_MOTION_TRIGGER) && wristMotion <= 10)
  {
    streachCounter++;
  }

  if(streachCounter >= WRIST_MOTION_TRIGGER)
  {
    streachCounter = 0;
    wristMotion++;
  }

  //Update FireBase
  if (Firebase.setInt(firebaseData, "/wrist", wristMotion))
      {
        Serial.println("----------Set WristMotion--------");
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

void vibrationHeartbeat()
{
  if(vibTrigger < VIB_TRIGGER_COUNTER)
  {
    digitalWrite(vibPin, LOW);
    
    if (Firebase.setInt(firebaseData, "/vibration", 0))
    {
      Serial.println("----------Set Vibration--------");
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
    
    if(CalcAngle < MIN_RANGE || CalcAngle > MAX_RANGE)
    {
      vibTrigger++;
      vibWait = 0;
    }
    else
    {
      vibTrigger = 0;
    }
  }
  else
  {
    digitalWrite(vibPin, HIGH);
    if (Firebase.setInt(firebaseData, "/vibration", 1))
    {
      Serial.println("----------Set Vibration--------");
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
    if(vibWait >= VIB_WAIT_COUNTER)
    {
      vibTrigger = 0;
      vibWait = 0;
    }
    else
    {
      vibWait++;
    }
  }
  
}
