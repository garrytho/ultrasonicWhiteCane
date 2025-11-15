# Ultrasonic Distance-Sensing White Cane
An Arduino-based cane for the visually impaired that uses a distance sensor and haptic feedback to give real time information about surroundings. 

## IMPORTANT:
This project is a work in progress. Iteration and modification may be necessary for reliable daily usage!

## Overview
Our project is a modified blind cane for the visually impaired that helps to reduce collisions while navigating. To accomplish this, we implemented an ultrasonic distance sensor along with a buzzer to alert the user when they are close to objects through vibration and sound. By using readily available components, 3D printed housing, and a pre-existing blind cane, we have created a widely accessible and affordable navigational aid. Our white cane is much cheaper to produce than commercially available alternatives, and is entirely open source for improvements and alterations. We hope that our resourceful design empowers more users with greater confidence and independence.

## Images and Demonstrations
<table>
  <tr>
    <td align="center">
      <img src="images/earlyPerfboard.jpg" width="250"><br>
      <em>Our first soldered perfboard being fitted into the case </em>
    </td>
    <td align="center">
      <img src="images/caseExample.jpg" width="300"><br>
      <em>The second iteration of the sensor mount </em>
    </td>
    <td align="center">
      <img src="images/prototypeTest.jpg" width="240"><br>
      <em>A fully assembled initial prototype</em>
    </td>
  </tr>
</table>

### Demo video here

## Features
-Entirely open sourced
-Much cheaper to produce as compared to commercial alternatives
-Rechargable battery via USB-C for accessibility
-Weight distribution is optimal for daily navigation

## Cost of development vs. commercial alternatives

| Device | Price per Unit | Precision | Notes |
|--------|----------------|-----------|-------|
| [WeWalk Smart Cane 2](https://floridareading.com/products/wewalk-smart-cane-2-with-voice-assistant) | $1,050 | High | Extremely expensive, but has extensive features. |
| [Saarthi Smart Aid](https://enablemart.in/shop/saarthi-smart-aid-smart-cane/) | $54.17 | Mid-High | Much more affordable, but sourced overseas and not readily available for the American market. |
| **This White Cane** | **~$51.69** | Mid | Highly portable, easy and cheap to produce, but low precision. Strong applications in low-resource environments. |







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
<p align="center">
  <img src="images/hardwareOverview.png" width="800"><br>
</p>

### Pin Assignments

**Power**:
- **+5v** - +5V from Boost Module
- **GND** - GND from Boost Module

**Distance Sensing**:
- **+5V** - VCC pin on the HC-SR04
- **GND** - GND pin on the HC-SR04
- **D5** - Trig pin on the HC-SR04
- **D11** - Echo pin on the HC-SR04

**Sound Cue**: 
- **D8** - VCC pin on the piezo buzzer
- **GND > Switch** - Toggles GND pin on the piezo buzzer (on/off)
  
**Haptic Cue**:
- **+5v** - VCC pin on vibration motor
- **D6** - Base pin on control transistor
- **GND > Transistor/Diode** - Emitter and collector lines for vibration motor control. 


### Setup Instructions
- Assemble the circuit as described above
- Connect the Arduino board to your computer via USB
- Open the provided .ino file in the Arduino IDE
- Select the appropriate port and board type (Nano)
- Upload the code to the Nano
- Disconnect from USB and power via LiPo


### Code Snippet
```cpp
//Take one distance reading (in cm)
long distance = pingDistance();

//Filter for valid readings
if (distance > 0 && distance < 120) {

  // Map distance to tone: closer = higher urgency
  int toneFreq = map(distance, 1, 120, 5000, 500);
  tone(8, toneFreq, 200);   // buzzer on pin 8

  // Vibration motor: full power at or below 60 cm
  if (distance <= 60) {
    analogWrite(vibPin, 255);   // ON
  } else {
    analogWrite(vibPin, 0);     // OFF
  }
}
```
For the full firmware, see [whiteCaneV6_Simplified_Clean.ino](whiteCaneV6_Simplified_Clean.ino)


### Hardware Limitations & Planned Improvements
| Issue                    | Description                                              | Planned Fix                                                                 |
|--------------------------|----------------------------------------------------------|------------------------------------------------------------------------------|
| Battery reliability      | 5V LIPO is vulnerable to weather interference.                  | Improve waterproofing and make the housing more robust|
| Sensor Accuracy      | The HC-SR04 alone can sometimes yield inconsistent results, especially with objects that scatter sound.                 | Integrate a second sensor (time of flight) and cross reference readings.|
| Housing       | Our current housing doesn't mechanically secure certain components in the circuitry box.                  | Add dedicated segments with mounting screws / threadded holes for securing components. |


