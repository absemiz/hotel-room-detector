# Hotel Room Detector


## Problem Statement

In hotels, visitors often leave their room cards in the power slot, 
causing air conditioners and other electrical systems to run unnecessarily when the room is unoccupied. 
This leads to energy waste and higher operational costs.

## Solution

Develop a smart room detection and automation system that uses sensors to detect human presence and door activity. 
The system will automatically cut off the power supply if no human presence is detected, 
even if the room card remains in the slot.

## Constrains

* The device must operate on battery power to ensure reliability during outages and minimize wiring needs.

## System Design

### Components Required

#### 1. ESP32 Microcontroller (x2)

* One ESP32 for handling the door sensor.
* Second ESP32 for handling the PIR sensor.

#### 2. HC-SR501 PIR Motion Sensor

* Detects human presence based on infrared radiation.

#### 3. MC-38 Wired Door Sensor Magnetic Switch

* Detects door open/close status.

#### 4. Relay Module

* Controls the power supply to the room.

#### 5. Battery Pack (x2)

* Provides power to ESP32 microcontrollers.

#### 6. Wi-Fi Network or 433Mhz RF Reciver/Transmitter

* Enables communication between the two ESP32 modules.

## System Architecture

### 1. Detection Units

* PIR sensor checks for human presence.
* Door sensor checks the door status.
* Communicate ESP32 modules each other and control unit.

### 2. Control Unit

* Receives sensor data from ESP32 modules.
* Controls a relay to cut or restore power to the room based on sensor inputs.