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
  Date: 7/06/26

  Version 0.9.9  7/06/26. Preliminary release for testingWork in progress.

  (c) 2026 Bob Glicksman, Jim Schrempp, Team Practical Projects.
  All rights reserved.

********************************************************************************************************/

#define VER "0.9.9"

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
double timeInState = 0.0;          // in minutes
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
int resetAlarm(String noString)
{
    systemState = NORM;
    return systemState;

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
void printData(float time, double ir, double amb)
{
    Serial.print("Time = ");
    Serial.print(time);
    Serial.print(" minutes\t");
    Serial.print("Ambient = ");
    Serial.print(amb);
    Serial.print("*F\tObject = ");
    Serial.print(ir);
    Serial.println("*F");

} // end of printData()

/******************************************************************* */
void setup()
{
    // declare cloud variables and functions
    Particle.variable("Time in current state (minutes)", timeInState);
    Particle.variable("IR Temperature (deg F)", irTemperature);
    Particle.variable("Ambient temperature (deg F)", ambientTemperature);
    Particle.variable("Current State", currentState);
    Particle.variable("Version", version);

    Particle.function("Reset", resetAlarm);

    // initializations
    Serial.begin(9600);
    greenLED.begin();
    yellowLED.begin();
    redLED.begin();
    buzzer.begin();
    resetBtn.begin();
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

    static unsigned long intervalMills;     // time interval for the next sensor reading
    static float intervalMinutes = 0.0f;    // time interval converted to minutes
    static double newIRtemp;                // IR value from the sensor - unfiltered
    static double newAmbtemp;               // Ambient value from the sensor - unfiltered
    static unsigned long baseTimeInState;   // millis() value when entering a new state
    static unsigned long millisTimeInState; // current time in the current state, in milliseconds

    intervalMills = millis() - loggingTimeMillis;

    // determine if it is time to sample new IR sensor data
    if (intervalMills >= RECORDING_INTERVAL)
    {
        intervalMinutes += ((float)(intervalMills) / 60000.0f);

        // read out the sensor and update global variables
        newIRtemp = mlx.readObjectTempF();
        newAmbtemp = mlx.readAmbientTempF();
        printData(intervalMinutes, newIRtemp, newAmbtemp); // print out the original data, even the errors

        //  test that readings are valid; if not valid, skip the update
        if ((newIRtemp < 1000.0) && (newAmbtemp < 1000.0))
        {
            irTemperature = newIRtemp;
            ambientTemperature = newAmbtemp;
        }

        loggingTimeMillis = millis();
    }

    // state machine for system behaviour
    switch (systemState)
    {
    case (NORM):

        // set normal state indicators
        greenLED.on();
        yellowLED.off();
        redLED.off();
        buzzer.off();

        // don't need to update time in state for NORMal state

        if ((irTemperature - ambientTemperature) >= WARM_UP_TH)
        {
            systemState = WARM;
            baseTimeInState = millis(); // initialize the time in the new state
        }
        else
        {
            // stay in the NORMal state
            systemState = NORM;
        }

        break;

    case (WARM):

        // set the warming state indicators
        greenLED.off();
        yellowLED.on();
        redLED.off();
        buzzer.off();

        // update the time in state
        millisTimeInState = millis() - baseTimeInState;
        timeInState = (double)(millisTimeInState) / 60000.0;

        if (timeInState >= WARM_ALARM_TIME)
        { // enter ALARM state
            Particle.publish("stovetopAlarm", "Alarm from WARM", PRIVATE);
            systemState = ALARM;
            baseTimeInState = millis(); // initialize the time in the new state
        }
        else
        { // no alarm, monitor the temperature to exit the state
            if ((irTemperature - ambientTemperature) >= COOK_UP_TH)
            { // enter the COOK state
                systemState = COOK;
                baseTimeInState = millis();
            }
            else
            {
                if ((irTemperature - ambientTemperature) <= WARM_DN_TH)
                { // back to the NORM state
                    systemState = NORM;
                    baseTimeInState = millis();
                }
                else
                { // we remain in the current state
                    systemState = WARM;
                }
            }
        }

        break;

    case (COOK):
        // set the cooking state indicators
        greenLED.off();
        yellowLED.flash();
        redLED.off();
        buzzer.off();

        // update the time in state
        millisTimeInState = millis() - baseTimeInState;
        timeInState = (double)(millisTimeInState) / 60000.0;

        if (timeInState >= COOK_ALARM_TIME)
        { // enter ALARM state
            Particle.publish("stovetopAlarm", "Alarm from COOK", PRIVATE);
            systemState = ALARM;
            baseTimeInState = millis(); // initialize the time in the new state
        }
        else
        { // no alarm, monitor the temperature to exit the state
            if ((irTemperature) >= BURN_UP_TH)
            { // enter the BURN state
                systemState = BURN;
                baseTimeInState = millis();
            }
            else
            {
                if ((irTemperature - ambientTemperature) <= COOK_DN_TH)
                { // back to the WARM state
                    systemState = WARM;
                    baseTimeInState = millis();
                }
                else
                { // we remain in the current state
                    systemState = COOK;
                }
            }
        }
        break;

    case (BURN):
        // set the burning state indicators
        greenLED.off();
        yellowLED.off();
        redLED.on();
        buzzer.off();

        // update the time in state
        millisTimeInState = millis() - baseTimeInState;
        timeInState = (double)(millisTimeInState) / 60000.0;

        if (timeInState >= BURN_ALARM_TIME)
        { // enter ALARM state
            Particle.publish("stovetopAlarm", "Alarm from BURN", PRIVATE);
            systemState = ALARM;
            baseTimeInState = millis(); // initialize the time in the new state
        }
        else
        { // no alarm, monitor the temperature to exit the state

            if ((irTemperature) <= BURN_DN_TH)
            { // back to the COOK state
                systemState = WARM;
                baseTimeInState = millis();
            }
            else
            { // we remain in the current state
                systemState = BURN;
            }
        }
        break;

    case (ALARM):
        // set the alarming state indicators
        greenLED.off();
        yellowLED.off();
        redLED.flash();
        buzzer.flash();

        // update the time in state
        millisTimeInState = millis() - baseTimeInState;
        timeInState = (double)(millisTimeInState) / 60000.0;

        // test for alarm reset button pressed
        if (resetBtn.isPressed() == true)
        {
            resetAlarm("noString");
        }
        else
        {
            systemState = ALARM;
        }
        break;

    default:                    // should never get here
        resetAlarm("noString"); // just return to the NORMal state
        break;
    }

    // update time in state

    // update current state as string
    switch (systemState)
    {
    case (NORM):
        currentState = "NORMAL";
        break;

    case (WARM):
        currentState = "WARMING";
        break;

    case (COOK):
        currentState = "COOKING";
        break;

    case (BURN):
        currentState = "BURNING";
        break;

    case (ALARM):
        currentState = "ALARM";
        break;

    default:
        currentState = "UNDEFINED";
        break;
    }

    // refresh LEDs and buzzer.
    refreshAll();

} // end of loop()
