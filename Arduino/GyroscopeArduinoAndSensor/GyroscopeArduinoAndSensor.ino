/*
   Gyroscope Calculator Between Sensor And embedded Arduino
*/

#include <Arduino_LSM6DS3.h>

#include<Wire.h>

const int MPU_addr=0x68; int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

int minVal=265; int maxVal=402; 

double x; double y; double z;


void setup() {
  
  Wire.begin(); 
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x6B); Wire.write(0); 
  Wire.endTransmission(true); 
  
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("IMU Uninitialized!");

    while (1);
  }
}

void loop() {
  
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x3B); 
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU_addr,14,true); 
  AcX=Wire.read()<<8|Wire.read(); 
  AcY=Wire.read()<<8|Wire.read(); 
  AcZ=Wire.read()<<8|Wire.read();

  //Sensor
  int xAng = map(AcX,minVal,maxVal,-90,90); 
  int yAng = map(AcY,minVal,maxVal,-90,90); 
  int zAng = map(AcZ,minVal,maxVal,-90,90);


x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); 
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI); 
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);


Serial.print("Sensor AngleX = "); Serial.println(x);
Serial.print("Sensor AngleY = "); Serial.println(y);
Serial.print("Sensor AngleZ = "); Serial.println(z);


Serial.println("-----------------------------------------"); 

      
  //Arduino
  
  float xArd, yArd, zArd, pitch;

      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(xArd,yArd,zArd);
        pitch = 180 * atan2(xArd, sqrt(yArd*yArd + zArd*zArd))/PI;
        Serial.println(pitch);
      }

  delay(2000);
}
