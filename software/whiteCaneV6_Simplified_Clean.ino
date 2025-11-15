//Pin definitions
int trig = 5;
int echo = 11;
int prevDistance;
int vibPin = 6;

//variabes for filtering window
int recentReadings[5];
int readingCount = -1;

//distance logging
long currentDistance;

//buzzer control
int toneFreq;

//valid readings, but fail the 5cm similarity gate
int proxMisses = 0;

//vibration control
int vibPin = 6;

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(vibPin, OUTPUT);
  analogWrite(vibPin, 0);
}

//bubble sort
void sortAscending() {
  const int n = sizeof(recentReadings) / sizeof(recentReadings[0]);
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
    if (!swapped) break;
  }
}

//returns the distance of an object in cm in accordance with the HC-SR04 datasheet
long pingDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  long distance = duration / 58;
  return distance;
}

void loop() {
  //update the current distance
  currentDistance = pingDistance();
  delayMicroseconds(60); //as per datasheet

  //general filter: valid sensor range
  if (currentDistance < 120 && currentDistance > 0) {

    //setup for enforcing 5 cm proximity
    if (prevDistance == 0) {
      prevDistance = currentDistance;
      proxMisses = 0;                   
    }

    //enforce cm proximity gate after setup
    if (currentDistance >= prevDistance - 5 && currentDistance <= prevDistance + 5) {
      //this passes
      proxMisses = 0;
      readingCount++;

      //fill the array slot by slot
      if (readingCount < 5) {
        recentReadings[readingCount] = currentDistance;
      }

      //if the array full. sort and take the median. 
      if (readingCount > 4) {
        sortAscending();

        Serial.print("Median ");
        Serial.print(recentReadings[2]);
        Serial.print("\n");

        readingCount = -1;
        currentDistance = recentReadings[2];

        //map distance to tone
        toneFreq = map(int(currentDistance), 1, 300, 5000, 0);
        tone(8, toneFreq, 200);
      }

      //update prevDistance ONLY for accepted samples
      prevDistance = currentDistance;

    } else {
      //in-range but failed proximity gate
      proxMisses++;          

      //simple escape—after 3 proximity misses to reset the prevDistance. Stops the thread from getting locked. 
      if (proxMisses >= 3) {
        prevDistance = 0;               // next valid in-range sample will seed
        readingCount = -1;              // clear partial window
        proxMisses = 0;
      }

    }
  } else {
  //invalid: do nothing here
  }



    // ---------------- VIBRATION CONTROL ----------------
  //FULL POWER if at or below 60 cm
  if (currentDistance <= 60 && currentDistance > 0) {
    analogWrite(vibPin, 255);     //motor on
  } else {
    analogWrite(vibPin, 0);       //motor off
  }
  // ---------------------------------------------------

}
