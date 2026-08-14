# Stovetop_Monitor.
## Overview.
The Stovetop Monitor project provides standoff, non-invasive monitoring of a stovetop with algorithms that 
determine if one or more burners are left on unintentionally.  An infrared (IR) temperature sensor is used to detect, 
categorize and track the IR radiation from the stovetop.  

## Purpose.
A stovetop burner that is accidentally left on presents a fire hazard.  Busy/distracted people, as well as elderly people 
who are becoming forgetful, can accidentally leave a stovetop burner on, unattended.  In the worst case, this can lead to 
a fire hazard and burn down the house as well as risking lives.  In less than the worst case, it can lead to damage to 
the stove and/or cookware.

## Solution.
The Stovetop Monitor uses a Melexis MLX90614 IR temperature sensor to continuously monitor both the ambient air temperature 
and the IR temperature of the stovetop.  A custom designed 3D printed enclosure houses the sensor and associated electronics.
The electronics mount underneath a cabinet that is above and off to the side of the stovetop.  The enclosure mounts the IR
temperature sensor so that it looks down at the stovetop at a 45 degree angle.  The entire stovetop is within the sensor's
field of view.

A Particle Photon 2 WiFi enabled microcontroller reads and processes sensor data.  The algorithm attempts to categorize what
is happening on the stovetop as Normal (burners are off), Warming (one or more burners on low), Cooking (one or more burners
on medium - high), or Burning (unusually high IR temperature reading).  Each of these stovetop states has an associated timeout,
beyond which it is assumed that the stovetop has been left on unattended.  After a timeout, a visual and audible alarm sounds.  A
remote alarm on a smartphone may also be configured using the Particle Cloud, Particle Webhooks and Pushover app capabilities.
An Android smartphone app is also included with this project, allowing a user to remotely monitor the status of their stovetop
an the ability to remotely reset any alarm in process.

See the "Stovetop_Monitor_Overview_and_Theory_of_Operation" document in this Repository for further details.

## Contents of this Repository.
### Top Level.
- "README.md": this document.
- "Terms_of_Use_License_and_Disclaimer":  You must agree to the contents of this document in order to use the material
    in this Repository.
- "Stovetop_Monitor_Overview_and_Theory_of_Operation":  This document provides an overview of the project.  It contains
    the project requirements, architecture, and theory of operation.
### Documents Folder.
- "Stovetop_Monitor_Build_and_Install_Manual":  Instructions for building and installing the Stovetop Monitor hardware
    and software.
- "Stovetop_Monitor_User_Manual":  Instructions for using the Stovetop Monitor, both locally and remotely.
### Hardware Folder.
- 3D CAD .stl files for the 3D printed enclosure and top mounting bracket.
- Fusion 360 .f3d file so modifying the enclosure design in Fusion 360 CAD software.
- MLX90614 IR temperature sensor datasheet.
- "A NOTE ABOUT PIEZO BUZZERS.txt": Options for the piezo buzzer that provides the audible alarm capability of the Stovetop Monitor.
### Software Folder.
#### AI2_App.
MIT App Inventor 2 source code and .apk installation file for the ANdroid App that is part of this project.
#### StovetopMonitor.
Source code, libraries and helper files for the Photon 2 microcontroller.
#### Webhook.
JSON representation of the Particle Webhook for sending remote alarms using Pushover.

