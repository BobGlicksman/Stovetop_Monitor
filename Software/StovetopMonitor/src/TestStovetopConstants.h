/****************************************************************************************************
 * TestStovetopConstants.h  This file contains the defined constants for the Stovetop_Monitor
 *  project.  These constants are used for testing purposes only!  The times are way too short for
 *  actual use.
 *
 * Version 1.0.  7/03/26. Per the Concept document, rev 7/2/27.
 *
 * (c) 2026 Bob Glicksman, Jim Schrempp, Team Practical Projects.  All rights reserved.
 *****************************************************************************************************/

// Thresholds for state transitions.
#define WARM_UP_TH 1.5   //  Threshold for transitioning from NORM to WARM when warming up; in deg F
#define COOK_UP_TH 7.0   //  Threshold for transitioning from WARM to COOK when warming up; in deg F
#define BURN_UP_TH 130.0 //  Threshold for transitioning from COOK to BURN when warming up; in deg F

#define WARM_DN_TH 1.0   //  Threshold for transitioning from WARM to NORM when cooling down; in deg F
#define COOK_DN_TH 6.0   //  Threshold for transitioning from COOK to WARM when cooling down; in deg F
#define BURN_DN_TH 125.0 //  Threshold for transitioning from BURN to COOK when cooling down; in deg F

// timeouts for alarm
#define WARM_ALARM_TIME 1 // Minutes in WARM state before alarm
#define COOK_ALARM_TIME 1 // Minutes in COOK state before alarm
#define BURN_ALARM_TIME 1 // Minutes in BURN state before alarm
