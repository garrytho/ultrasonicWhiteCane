# Ultrasonic Distance-Sensing White Cane
An Arduino-based cane for the visually impaired that uses a distance sensor and haptic feedback to give real time information about surroundings. 

## IMPORTANT:
This project is a work in progress. Iteration and modification may be necessary for reliable daily usage!

## Overview
This project aims to improve 

## Images and Demonstrations


### Demo video here

## Features
-Entirely open sourced
-Much cheaper to produce as compared to commercial alternatives

## Cost of development VS. commercial CO detectors
| Device | Price per Unit | Precision | Notes |
|--------|----------------|-----------|-------|
| [Standard Wall Mount Detector](https://garrytho.short.gy/nCqUUw) | ~$21.51 | Mid–High | Not portable. Requires constant power supply. |


## Use Cases and Applications


# BILL OF MATERIALS

| # | Item               | Purpose           | Qty. | Cost/Unit Part | Cost |
|---|--------------------|------------------|------|----------------|------|
| 1 | HC-SR04            | Ultrasonic Sensor | 1    | $4             | $4   |
| 2 | Arduino Nano       | Microcontroller   | 1    | $5             | $5   |
| 3 | JYCL0610R2540      | Vibration Motor   | 3    | $2             | $6   |
| 4 | MakerFocus 3.7V 3000mAh Lipo | LiPo Batteries | 2 | $9 | $18 |
| 5 | MT3608             | Step Up           | 1    | $1             | $1   |
| 6 | TP4056             | Charge Protector  | 1    | $2             | $2   |
| 7 | Cane               | Cane              | 1    | $15            | $15  |
**Total:** $51

## Circuit Diagram


## Firmware Overview


### Pin Assignments


### Setup Instructions
- Assemble the circuit as described above
- Connect the Arduino board to your computer via USB
- Open the provided .ino file in the Arduino IDE
- Select the appropriate port and board type (Nano)
- Upload the code to the Nano
- Disconnect from USB and power via 5v LIPO

### Behavior table


### Code Snippet

For the full firmware, see 


## Development process and challenges
### Initial Vision: 

### Power Struggles

### Perfboarding

### Firmware Logic

### Hardware Limitations & Planned Improvements
| Issue                    | Description                                              | Planned Fix                                                                 |
|--------------------------|----------------------------------------------------------|------------------------------------------------------------------------------|
| Battery reliability      | 5V LIPO is vulnerable to weather interference.                  | Improve waterproofing and make the housing more robust|


