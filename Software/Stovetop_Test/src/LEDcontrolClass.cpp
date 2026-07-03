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

#include <LEDcontrolClass.h>

// constructor.  pin is the pin number to which the LED is connected.
LEDcontrolClass::LEDcontrolClass(int pin)
{
  _pin = pin;
  _currentState = OFF;
  _flashTime = DEFAULT_FLASH_TIME;
  _lastFlashTime = 0;
}

// begin() method.  Call in setup() to initialize the pin mode and set the LED to off.
void LEDcontrolClass::begin()
{
  pinMode(_pin, OUTPUT);
  _currentState = OFF;
}

void LEDcontrolClass::on()
{
  _currentState = ON;
}

void LEDcontrolClass::off()
{
  _currentState = OFF;
}

void LEDcontrolClass::flash()
{
  _currentState = FLASH;
}

void LEDcontrolClass::setRate(int flashTime)
{
  _flashTime = flashTime;
  refresh();
}

int LEDcontrolClass::getRate()
{
  return _flashTime;
}

void LEDcontrolClass::refresh()
{

  switch (_currentState)
  {
  case OFF:
    digitalWrite(_pin, LOW);
    break;
  case ON:
    digitalWrite(_pin, HIGH);
    break;
  case FLASH:
    if (millis() - _lastFlashTime >= _flashTime)
    {
      digitalWrite(_pin, !digitalRead(_pin));
      _lastFlashTime = millis();
    }
    break;

  default: // if unspecified in enum, defualt to flash
    if (millis() - _lastFlashTime >= _flashTime)
    {
      digitalWrite(_pin, !digitalRead(_pin));
      _lastFlashTime = millis();
    }
    break;
  }
}
