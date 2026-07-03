/**************************************************************************************
 * BtnStatusClass.  Library to detect and debounce a pushbutton switch closure.
 *   The isPressed() method must be called frequently in loop() in order to detect
 *   the button closure accurately.  Therefore, loop() code should be non-blocking.
 *
 * Author:  Bob Glicksma.
 * (c) 2026 by Bob Glicksman, Jim Schrempp, Team Practical Projects.  All rights reserved.
 *
 * Version 1.0. Initial release. 7/02/26
 *
 **************************************************************************************/

#include <BtnStatusClass.h>

// constructor.  pin is the pin number to which the LED is connected.
BtnStatusClass::BtnStatusClass(int pin)
{
  _pin = pin;
  _currentState = NOT_PRESSED;
  _lastDebounceTime = 0;
}

// begin() method.  Call in setup() to initialize the pin mode and set the button state to not pressed.
void BtnStatusClass::begin()
{
  pinMode(_pin, INPUT_PULLUP);
  _currentState = NOT_PRESSED;
}

// isPressed() method.  Return true if the button is currently pressed, false otherwise.
bool BtnStatusClass::isPressed()
{
  switch (_currentState)
  {
  case NOT_PRESSED:
    if (digitalRead(_pin) == LOW) // button pressed
    {
      _currentState = TENTATIVE;
      _lastDebounceTime = millis();
      return false; // button is not yet considered pressed
    }
    else // button not pressed
    {
      _currentState = NOT_PRESSED;
      return false; // button is not pressed
    }
    break;

  case TENTATIVE:
    if (digitalRead(_pin) == LOW) // button still pressed
    {
      if ((millis() - _lastDebounceTime) > _debounceDelay)
      {
        _currentState = PRESSED;
        return true; // button is now considered pressed
      }

      else // debounce time not yet elapsed
      {
        _currentState = TENTATIVE;
        return false; // button is not yet considered pressed
      }
    }
    else // button released before debounce time
    {
      _currentState = NOT_PRESSED;
      return false; // button is not pressed
    }
    break;

  case PRESSED:
    if (digitalRead(_pin) == HIGH) // button released
    {
      _currentState = NOT_PRESSED;
      return false; // button is not pressed
    }
    else // button still pressed
    {
      _currentState = PRESSED;
      return false; // button is still considered pressed
    }
    break;

  default:
    _currentState = NOT_PRESSED;
    return false; // button is not pressed
  }
}