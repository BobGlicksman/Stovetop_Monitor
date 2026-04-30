/*
 Stovetop Test:  Program to collect stovetop IR temperature data and log the data to the
  Serial monitor.

  An MLX90614 IR temerature sensor is connected to a Particle Photon 2 using the I2C bus.
  The Adarfruit MLX90614 library is used to read data from the sensor.

  The Photon2 and the sensor are mounted above and to the side of a stove.  The sensor is pointed
  down at the stovetop so that its FOV covers the entire stovetop.  Individual burners will be turned
  on and off at various heat levels and the data recorded until the sensor readings stabilize.  Each burner
  will be tested with and without a pot or pan on the burner.  The intent of this testing is to determine
  if the sensor can be used to detect when a burner is left on after the usual cooking time of a burner
  with that heat setting.  If the burner is on too long, it may be assumed that it has been unintentionally
  left on and unattended and an alarm should be generated.

  Author:  Bob Glicksman
  Date: 4/30/26
  Version 1.0 - Initial release for testing

  (c) 2026 Bob Glicksman, Jim Schrempp, Team Practical Projects.
  All rights reserved.

*/

// Include Particle Device OS APIs
#include "Particle.h"

// Include MLX90614 library
#include <Adafruit_MLX90614.h>

// define interval for data recording
#define RECORDING_INTERVAL 15000 // take data every 15 seconds

// instantiate the sensor
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Global variables
unsigned long loggingTimeMillis;

// Function to log data to the serial monitor
void printData(float time)
{
  Serial.print("Time = ");
  Serial.print(time);
  Serial.print(" minutes\t");
  Serial.print("Ambient = ");
  Serial.print(mlx.readAmbientTempF());
  Serial.print("*F\tObject = ");
  Serial.print(mlx.readObjectTempF());
  Serial.println("*F");

} // end of printData()

void setup()
{
  Serial.begin(9600);

  delay(5000); // wait for console opening// wait to open a serial monitor
  Serial.println("Stovetop Sensor test");
  mlx.begin();
  Serial.println("Ready for testing ...");
  loggingTimeMillis = millis();

} // end of setup()

void loop()
{
  unsigned long intervalMills;
  static float intervalMinutes = 0.0f;

  intervalMills = millis() - loggingTimeMillis;

  if (intervalMills >= RECORDING_INTERVAL)
  {
    intervalMinutes += ((float)(intervalMills) / 60000.0f);
    printData(intervalMinutes);

    loggingTimeMillis = millis();
  }

} // end of loop()
