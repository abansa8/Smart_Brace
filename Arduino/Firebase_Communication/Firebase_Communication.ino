
//Example shows how to connect to Firebase RTDB and perform basic operation for set, get, push and update data to database

//Required WiFiNINA Library for Arduino from https://github.com/arduino-libraries/WiFiNINA

#include "Firebase_Arduino_WiFiNINA.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include "time.h"

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

  Serial.begin(115200);
  delay(100);
  Serial.println();

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

  //

  //Provide the autntication data
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);

  String path = "/UNO_WiFi_REV2_Test";
  String jsonStr;

  Serial.println("-----------------------------------");
  Serial.println("----------Begin Set Test-----------");
  Serial.println("-----------------------------------");
  Serial.println();

  for (uint8_t i = 0; i < 10; i++)
  {

    if (Firebase.setInt(firebaseData, path + "/Int/Data" + (i + 1), (i + 1) * 10))
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
    else
    {
      Serial.println("----------Can't set data--------");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("--------------------------------");
      Serial.println();
    }
  }

  Serial.println("-----------------------------------");
  Serial.println("----------Begin Get Test-----------");
  Serial.println("-----------------------------------");
  Serial.println();

  for (uint8_t i = 0; i < 10; i++)
  {

    if (Firebase.getInt(firebaseData, path + "/Int/Data" + (i + 1)))
    {
      Serial.println("----------Get result-----------");
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
    else
    {
      Serial.println("----------Can't get data--------");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("--------------------------------");
      Serial.println();
    }
  }

  Serial.println("-----------------------------------");
  Serial.println("----------Begin Push Test----------");
  Serial.println("-----------------------------------");
  Serial.println();

  for (uint8_t i = 0; i < 5; i++)
  {

    if (Firebase.pushInt(firebaseData, path + "/Push/Int", (i + 1)))
    {
      Serial.println("----------Push result-----------");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.print("PUSH NAME: ");
      Serial.println(firebaseData.pushName());
      Serial.println("--------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("----------Can't push data--------");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("--------------------------------");
      Serial.println();
    }
  }

  for (uint8_t i = 5; i < 10; i++)
  {

    jsonStr = "{\"Data" + String(i + 1) + "\":" + String(i + 1) + "}";

    if (Firebase.pushJSON(firebaseData, path + "/Push/Int", jsonStr))
    {
      Serial.println("----------Push result-----------");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.print("PUSH NAME: ");
      Serial.println(firebaseData.pushName());
      Serial.println("--------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("----------Can't push data--------");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("--------------------------------");
      Serial.println();
    }
  }

  Serial.println("-----------------------------------");
  Serial.println("---------Begin Update Test----------");
  Serial.println("-----------------------------------");
  Serial.println();

  for (uint8_t i = 0; i < 5; i++)
  {

    jsonStr = "{\"Data" + String(i + 1) + "\":" + String(i + 5.5) + "}";

    if (Firebase.updateNode(firebaseData, path + "/Int", jsonStr))
    {
      Serial.println("----------Update result-----------");
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
    else
    {
      Serial.println("----------Can't Update data--------");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("--------------------------------");
      Serial.println();
    }
  }
}

void loop()
{

while (!timeClient.update()){
  timeClient.forceUpdate();
}
  formattedDate = timeClient.getFormattedTime();
  Serial.println(formattedDate);
}
