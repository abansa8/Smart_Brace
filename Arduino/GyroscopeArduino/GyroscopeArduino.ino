/*
   Gyroscope Calculator
*/

#include <Arduino_LSM6DS3.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("IMU Uninitialized!");

    while (1);
  }
}

void loop() {

 
  float x, y, z, pitch;

      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x,y,z);
        pitch = 180 * atan2(x, sqrt(y*y + z*z))/PI;
        Serial.println(pitch);
      }
  }

  //delay(2000);
