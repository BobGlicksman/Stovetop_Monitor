/***************************************************
  TBased upon the Adafruit library example for the MLX90614 Temp Sensor

  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1748
  ----> https://www.adafruit.com/products/1749

  These sensors use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  Modified by: Bob Glicksman; 4/26/26
  - added 5 second delay in setup() to allow time to open a serial monitor.
  - changed the loop() delay to 2 seconds for less distracting reporting.
  - added 10 millisecond delays between the sensor reading to see if spurious readings are eliminated (they aren't).

 ****************************************************/

#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup()
{
  Serial.begin(9600);

  delay(5000); // wait for console opening// wait to open a serial monitor

  Serial.println("Adafruit MLX90614 test");

  mlx.begin();
}

void loop()
{
  Serial.print("Ambient = ");
  Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = ");
  delay(10);

  Serial.print(mlx.readObjectTempC());
  Serial.println("*C");
  delay(10);

  Serial.print("Ambient = ");
  Serial.print(mlx.readAmbientTempF());
  Serial.print("*F\tObject = ");
  delay(10);

  Serial.print(mlx.readObjectTempF());
  Serial.println("*F");

  Serial.println();
  delay(2000);
}
