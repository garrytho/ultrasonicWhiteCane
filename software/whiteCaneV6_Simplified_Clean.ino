int trig = 5;
int echo = 11;
int prevDistance;

int recentReadings[5];
int readingCount = -1;

long currentDistance;
int toneFreq;

//counts in-range readings that fail the ±5 cm proximity gate
int proxMisses = 0;

const int vibPin = 6;

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

    //Seed once before enforcing ±5 cm proximity
    if (prevDistance == 0) {
      prevDistance = currentDistance;   //establish anchor
      proxMisses = 0;                   //clear proximity miss streak
    }

    //enforce ±5 cm proximity gate after seeding
    if (currentDistance >= prevDistance - 5 && currentDistance <= prevDistance + 5) {
      // accepted sample
      proxMisses = 0;

      readingCount++;

      if (readingCount < 5) {
        recentReadings[readingCount] = currentDistance;
      }

      if (readingCount > 4) {
        sortAscending();

        Serial.print("Median ");
        Serial.print(recentReadings[2]);
        Serial.print("\n");

        //reset window and use median
        readingCount = -1;
        currentDistance = recentReadings[2];

        //map distance to tone
        toneFreq = map(int(currentDistance), 1, 300, 5000, 0);
        tone(8, toneFreq, 200);
      }

      //update anchor ONLY for accepted samples
      prevDistance = currentDistance;

    } else {
      //in-range but failed proximity gate
      proxMisses++;                     // FIX 3b: track consecutive proximity rejects

      // FIX 3c: simple escape—after 3 proximity misses, allow reseed
      if (proxMisses >= 3) {
        prevDistance = 0;               // next valid in-range sample will seed
        readingCount = -1;              // clear partial window
        proxMisses = 0;
      }
      // NOTE: do NOT update prevDistance here (rejects shouldn't move the anchor)
    }
  } else {
  //out-of-range or invalid: do nothing to the anchor/window here
  }



    // ---------------- VIBRATION CONTROL ----------------
  //FULL POWER if at or below 60 cm
  if (currentDistance <= 60 && currentDistance > 0) {
    analogWrite(vibPin, 255);     // turn motor ON
  } else {
    analogWrite(vibPin, 0);       // turn motor OFF
  }
  // ---------------------------------------------------
}