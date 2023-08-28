/*
  Using the BNO08x IMU

  This example shows how to use the stability classifier:
  Are we on a table, stationary, stable, or moving?

  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Originally written by Nathan Seidle @ SparkFun Electronics, December 28th, 2017

  Adjusted by Pete Lewis @ SparkFun Electronics, June 2023 to incorporate the
  CEVA Sensor Hub Driver, found here:
  https://github.com/ceva-dsp/sh2

  Also, utilizing code from the Adafruit BNO08x Arduino Library by Bryan Siepert
  for Adafruit Industries. Found here:
  https://github.com/adafruit/Adafruit_BNO08x

  Also, utilizing I2C and SPI read/write functions and code from the Adafruit
  BusIO library found here:
  https://github.com/adafruit/Adafruit_BusIO

  Hardware Connections:
  Plug the sensor into IoT RedBoard via QWIIC cable.
  Serial.print it out at 115200 baud to serial monitor.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/22857
*/

#include <Wire.h>

#include "SparkFun_BNO08x_Arduino_Library.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_BNO08x
BNO08x myIMU;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO08x Read Example");

  Wire.begin();

  if (myIMU.begin() == false) {
    Serial.println("BNO08x not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    while (1)
      ;
  }
  Serial.println("BNO08x found!");

  // Wire.setClock(400000); //Increase I2C data rate to 400kHz

  setReports();

  Serial.println("Reading events");
  delay(100);
}

// Here is where you define the sensor outputs you want to receive
void setReports(void) {
  Serial.println("Setting desired reports");

  if (myIMU.enableStabilityClassifier() == true) {
    Serial.println(F("Stability Classifier enabled"));
  } else {
    Serial.println("Could not enable Stability Classifier");
  }
}

void loop() {
  delay(10);

  if (myIMU.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }

  // Has a new event come in on the Sensor Hub Bus?
  if (myIMU.getSensorEvent() == true) {

    // is it the correct sensor data we want?
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_STABILITY_CLASSIFIER) {

    byte classification = myIMU.getStabilityClassifier();

    if(classification == STABILITY_CLASSIFIER_UNKNOWN) Serial.print(F("Unknown classification"));
    else if(classification == STABILITY_CLASSIFIER_ON_TABLE) Serial.print(F("On table"));
    else if(classification == STABILITY_CLASSIFIER_STATIONARY) Serial.print(F("Stationary"));
    else if(classification == STABILITY_CLASSIFIER_STABLE) Serial.print(F("Stable"));
    else if(classification == STABILITY_CLASSIFIER_MOTION) Serial.print(F("Motion"));
    else if(classification == STABILITY_CLASSIFIER_RESERVED) Serial.print(F("[Reserved]"));

    Serial.println();
    }
  }
}
