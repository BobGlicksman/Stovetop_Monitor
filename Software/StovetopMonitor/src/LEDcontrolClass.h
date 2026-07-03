/**************************************************************************************
 * LEDcontrolClass.  Library to control an LED or any device that can be turned on and
 *   off using digitalWrite(), e.g. LED, buzzer, etc.  Methods allow objects of this
 *   class to be turned on, tuend off, or flashed at a specifed rate.
 *
 *   the refresh() method must be called frequently in loop() in order to allow
 *   flashing to occur accurately.  Therefore, loop() code should be non-blocking.
 *
 * Author:  Bob Glicksma.
 * (c) 2026 by Bob Glicksman, Jim Schrempp, Team Practical Projects.  All rights reserved.
 *
 * Version 1.0. Initial release. 7/01/26
 *
 **************************************************************************************/
#ifndef ledcc
#define ledcc

#include <Particle.h>

class LEDcontrolClass
{
public:
    LEDcontrolClass(int pin);    // constructor.  pin is the pin number to which the LED is connected.
    void begin();                // call in setup() to initialize the pin mode and set the LED to off.
    void on();                   // turn the device on
    void off();                  // turn the device off
    void flash();                // cause the device to flash at the rate specified by _flashTime
    void setRate(int flashTime); // set the flash rate in milliseconds.  Default is DEFAULT_FLASH_TIME.
    void refresh();              // must be called frequently from loop() min order to flash the device.

    int getRate(); // return the current flash rate in milliseconds

private:
    enum State
    {
        OFF,
        ON,
        FLASH
    };
    const int DEFAULT_FLASH_TIME = 300; // default on and off time in milliseconds
    State _currentState;
    int _pin;
    int _flashTime;
    unsigned long _lastFlashTime;
};

#endif
