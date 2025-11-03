#include <Adafruit_NeoPixel.h>


//pin definitions for the distance sensor
int trig = 5;
int echo = 11;

//pin definition for the vibration motor
int vibPin = 6;

//pin definition for the led WS2812 strip
int ledPin = 2;

//variables for distance sensor filtering and tracking
long currentDistance;
int prevDistance;
int recentReadings[5];
int readingCount = -1;

//the amount of failed sensor values allowed before we turn off vibration
int missCount = 0;
int missThreshold = 5;

//variables for buzzer
int buzzPin = 8;
int toneFreq;

//variables for vibration motor
int prevVibrationIntensity = 0;
int vibrationIntensity;

//variables for led control
int ledBrightness = 255;
int ledCount = 26;
Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //pinMode assignments
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(vibPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  //beginning the LED strip as a constant red LED.
  strip.begin();
  strip.show();
  strip.setBrightness(ledBrightness);
  strip.fill(strip.Color(255, 0, 0));
  strip.show();
}

//a bubble sort that sorts the global array currentReadings[] ascending
void sortAscending() {

  //specify the end component 
  const int n = sizeof(recentReadings) / sizeof(recentReadings[0]);

  //nested loops. standard bubble sort
  for (int i = 0; i < n - 1; ++i) {
    bool swapped = false;
    for (int j = 0; j < n - 1 - i; ++j) {
      if (recentReadings[j] > recentReadings[j + 1]) {
        int t = recentReadings[j];
        recentReadings[j] = recentReadings[j + 1];
        recentReadings[j + 1] = t;
        swapped = true;
      }
    }
    if (!swapped) break; //exit if already sorted
  }
}

long pingDistance() {
  // the ping sequence for our distance sensor, as per the datasheet
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // this acts as a "stopwatch". tells you how long it took the ultrasonic wave to return.
  long duration = pulseIn(echo, HIGH);

  // converting that distance to a cm value, as per the datasheet
  long distance = duration / 58;

  // return the value we found
  return distance;
}

//converts the current distance in centimeters to one of three ranges.
int getDistanceRange(int cm){

  
  //the closest state. will be used to issue the harshest warning
  if(cm < 45){
    return 1;
  }

  //the second closest state. Moderate warning
  if(cm < 75){
    return 2;
  }

  //the farthest state
  if(cm < 100){
    return 3;
  }

  //a null state. Stop the buzzer
  else{
    return 0;
  }
}


void loop() {
  // put your main code here, to run repeatedly:

  //update the current distance
  currentDistance = pingDistance();
  delayMicroseconds(60); //as per datasheet

  // a general filter. This sensor will not read past 150cm. Also, account for a base state of 0
  if (currentDistance < 150 && currentDistance > 0) {

    // a more advanced filter. The sensor data must be within 5 cm of the previous one to count
    // Skip filter if prevDistance is 0 (startup or after bad readings)
    if (prevDistance == 0 || (currentDistance >= prevDistance - 5 && currentDistance <= prevDistance + 5)) {
      readingCount++;
      

      // using the array to log the value...
      if (readingCount < 5) {  // cap at four so no index out of bounds
        recentReadings[readingCount] = currentDistance;
      }

      // only output a tone if the reading count has filled up!
      if (readingCount > 4) {
        sortAscending();

        Serial.print("Median ");
        Serial.print(recentReadings[2]);
        Serial.print("  Intensity: ");
        Serial.print(vibrationIntensity);
        Serial.print("\n");

        //resetting the pointer for the next array iteration and update currentDistance to median
        readingCount = -1;
        currentDistance = recentReadings[2];
        // update the prev distance to the current distance
        prevDistance = currentDistance;


        //BUZZER CONTROLS:
        //map the distance (in cm) to a buzzer tone. Higher is be shriller
        toneFreq = map(int(currentDistance), 1, 300, 5000, 0);
        tone(buzzPin, toneFreq, 200);
      
        //VIBRATION CONTROLS:
        //decide the distance range and then vibrate the motor accordingly
        switch(getDistanceRange(currentDistance)){
          case 1: vibrationIntensity = 255; break;
          case 2: vibrationIntensity = 155; break;
          case 3: vibrationIntensity = 55;  break;
          case 0: vibrationIntensity = 0;   break;
        }

        //only analogWrite if the intensity has actually updated
          if(prevVibrationIntensity != vibrationIntensity){
            analogWrite(vibPin, vibrationIntensity);
          }

          prevVibrationIntensity = vibrationIntensity;
      }
    }
  } else {
    //bad reading, reset so next good reading is accepted
    prevDistance = 0;
    readingCount = -1;
    
    //turn off motor when no valid object detected
    vibrationIntensity = 0;
    analogWrite(vibPin, vibrationIntensity);
    prevVibrationIntensity = 0;
  }

  
}