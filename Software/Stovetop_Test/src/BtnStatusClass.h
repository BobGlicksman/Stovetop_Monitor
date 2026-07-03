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
#ifndef btncc
#define btncc

#include <Particle.h>

class BtnStatusClass
{
public:
    BtnStatusClass(int pin); // constructor.  pin is the pin number to which the button is connected.
    void begin();            // call in setup() to initialize the pin mode and set the button state to not pressed.
    bool isPressed();        // return true if the button is currently pressed, false otherwise.

private:
    int _pin;
    unsigned long _lastDebounceTime;
    const unsigned long _debounceDelay = 10; // debounce delay in milliseconds
    enum State
    {
        NOT_PRESSED,
        TENTATIVE,
        PRESSED
    };
    State _currentState;
};

#endif
