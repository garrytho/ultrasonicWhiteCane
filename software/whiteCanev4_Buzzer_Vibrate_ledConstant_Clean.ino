#include <Adafruit_NeoPixel.h>


//pin definitions for the distance sensor
int trig = 9;
int echo = 10;

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
    if (currentDistance >= prevDistance - 5 && currentDistance <= prevDistance + 5) {
      readingCount++;
      
      //we passed filtering. reset the miss Count
      missCount = 0;

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


        //BUZZER CONTROLS:
        //map the distance (in cm) to a buzzer tone. Higher is be shriller
        toneFreq = map(int(currentDistance), 1, 300, 5000, 0);
        tone(3, toneFreq, 200);
      
        //VIBRATION CONTROLS:
        //decide the distance range and then vibrate the motor accordingly
        switch(getDistanceRange(currentDistance)){
          case 1: vibrationIntensity = 255; break;
          case 2: vibrationIntensity = 155; break;
          case 3: vibrationIntensity = 55;  break;
          case 4: vibrationIntensity = 0;   break;
        }

        //only analogWrite if the intensity has actually updated
          if(prevVibrationIntensity != vibrationIntensity){
            analogWrite(vibPin, vibrationIntensity);
          }

          prevVibrationIntensity = vibrationIntensity;
      }
    }
  } else {
    missCount++;

    //turn the motor off if we've missed too many times.
    if(missCount >= 5){
      vibrationIntensity = 0;
      analogWrite(vibPin, vibrationIntensity);
      Serial.println("Too many misses! Stopped the motor.");
    }

  }

  // update the prev distance to the current distance
  prevDistance = currentDistance;
}