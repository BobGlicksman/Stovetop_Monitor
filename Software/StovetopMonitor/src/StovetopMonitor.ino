/******************************************************************************************************
 StovetopMonitor:  This program monitors the infrared and ambient temperature of a stovetop
   in order to determine if the stovetop has been left on for too long and generates an alarm
   if so.  The program uses an MLX90614 IR temperature sensor to read the stovetop temperature
   from a distance.  It also measures the ambient temperature of the sensor.

   Experimental data was gathered in the spring of 2026 to determine the thresholds for the stovetop
   and ambient temperature readings that could indicate if one or more burners were tuned on for
   WARMing food, for COOKing food, or left open on high heat (BURNing).  Times in each state that e
   exceed normal operational times are also experimentally determined.  When a pre-determined time
   in any state exceed the time threshold for that state, the ALARM state is entered and the user is
   notified (both locally and remotely).  See the Concept Document for details:

    https://docs.google.com/document/d/18Uzl63lqLXmCHiYM6owl-rcvX6FZUQaka_lKGS37vmU/edit?usp=drive_link

    A Particle Photon 2 is used as the IoT processor for this project.  The Team practical
    Projects RFID Station pcb is used for this project; see:

    https://github.com/TeamPracticalProjects/MN_ACL/tree/master/Hardware/PC_board

    A Photon 2 is used in place of the depricated Argon and the pinouts are converted accordingly.
    The MLX90614 IR temerature sensor is connected to a Particle Photon 2 using the I2C bus.  A
    commercially available MLX90614 breakout board is used on this project:

    https://www.amazon.com/dp/B0B63K5V7T?ref=ppx_yo2ov_dt_b_fed_asin_title

    This breakout board houses the MLX90614 sensor, I2C bos pullup resistors, and a low droptout
    voltage regualator (so that it can rune from either 5 volts or 3.3 volts).  The sensor breakout board
    is connected to the Photon 2 using the I2C bus.  The Adafruit MLX90614 library is used to read
    data from the sensor.

    The pcb provides connections to the following components that are used to indicate the status
    of the monitored stovetop:

    - A green LED
    - A yellow LED
    - A red backlit pushbutton switch, containing the red LED and a button to manually reset an alarm.
    - A piezo buzzer

    Other components supported by the pcb are not used for this project:

    - 16x2 LCD display and its contrast pot
    - I2C bus pullup resistors (the MLX90614 breakout board has its own pullup resistors)

  Author:  Bob Glicksman
  Date: 7/03/26

  Version 1.0.  7/03/26. Initial release for testing

  (c) 2026 Bob Glicksman, Jim Schrempp, Team Practical Projects.
  All rights reserved.

********************************************************************************************************/

#define VER "1.0"

// Include Particle Device OS APIs
#include <Particle.h>

// Include MLX90614 library
#include <Adafruit_MLX90614.h>

// include the TPP LED and Button libraries
#include "LEDcontrolClass.h"
#include "BtnStatusClass.h"

// Include the constants for the Stovetop_Monitor project
#include "TestStovetopConstants.h" // used for TESTING ONLY
// #include "StovetopConstants.h"  // constants for the electric stovetop

// Photon 2 pin definitions for this project
#define GREEN_LED D4
#define YELLOW_LED S4
#define RED_LED S3
#define BUZZER D2
#define ALARM_RESET D16

// define interval for data recording
#define RECORDING_INTERVAL 15000 // take data every 15 seconds

// instantiate the sensor
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// instantiate the LEDs and pushbutton
LEDcontrolClass greenLED(GREEN_LED);
LEDcontrolClass yellowLED(YELLOW_LED);
LEDcontrolClass redLED(RED_LED);
LEDcontrolClass buzzer(BUZZER);
BtnStatusClass resetBtn(ALARM_RESET);

// Global variables
//  Cloud variables
int timeInState = 0;               // in minutes
double irTemperature = 0.0;        // in degrees F
double ambientTemperature = 0.0;   // in degrees F
String currentState = "Undefined"; // "Normal", "Warming", "Cooking", "Burning", "Alarm"
String version = VER;

//  Other globals
unsigned long loggingTimeMillis; // beginning millis() time for data sampling
enum State
{ // states of the system
    NORM,
    WARM,
    COOK,
    BURN,
    ALARM
};

State systemState; // The current state of the system - not the string to publish to the cloud!

/************************************************* */
// Functions

// Cloud function to reset the system to the NORM state
void resetAlarm()
{
    systemState = NORM;
} // end of resetAlarm()

// Function to refresh all of the LEDs and buzzer
void refreshAll()
{
    greenLED.refresh();
    yellowLED.refresh();
    redLED.refresh();
    buzzer.refresh();
} // end of refreshAll()

// Function to log data to the serial monitor
void printData(float time)
{
    Serial.print("Time = ");
    Serial.print(time);
    Serial.print(" minutes\t");
    Serial.print("Ambient = ");
    Serial.print(ambientTemperature);
    Serial.print("*F\tObject = ");
    Serial.print(irTemperature);
    Serial.println("*F");

} // end of printData()

/******************************************************************* */
void setup()
{
    // initializations
    Serial.begin(9600);
    greenLED.begin();
    yellowLED.begin();
    redLED.begin();
    buzzer.begin();
    mlx.begin();

    // set flashing rates for this program
    greenLED.setRate(200);
    yellowLED.setRate(200);
    redLED.setRate(200);
    buzzer.setRate(200);

    // turn on the green LED until Photon 2 connected to the Particle cloud
    greenLED.on();
    yellowLED.off();
    redLED.off();
    buzzer.off();
    refreshAll();

    waitUntil(Particle.connected);

    // turn on all LEDs and turn the buzzer off - selftest
    greenLED.on();
    yellowLED.on();
    redLED.on();
    buzzer.off();
    refreshAll();

    // wait a few seconds to open the serial monitor
    delay(2000);

    // set the initial state of the system to NORM
    systemState = NORM;
    currentState = "Normal";
    Serial.println("Stovetop Monitor Data.");
    Serial.println("Ready ....");
    loggingTimeMillis = millis();

} // end of setup()

void loop()
{
    // determine if it is time to sample new IR sensor data
    unsigned long intervalMills;
    static float intervalMinutes = 0.0f;

    intervalMills = millis() - loggingTimeMillis;

    if (intervalMills >= RECORDING_INTERVAL)
    {
        intervalMinutes += ((float)(intervalMills) / 60000.0f);

        // update global variables
        irTemperature = mlx.readObjectTempF();
        ambientTemperature = mlx.readAmbientTempF();

        printData(intervalMinutes);

        loggingTimeMillis = millis();
    }

    // state machine for system behaviour

    // update time in state

    // update current state as string

    // refresh LEDs and buzzer.

} // end of loop()
