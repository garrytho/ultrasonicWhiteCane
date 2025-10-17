int trig = 9;
int echo = 10;
int prevDistance;

int recentReadings[5];
int readingCount = -1;

long currentDistance;
int toneFreq;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
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

void loop() {
  // put your main code here, to run repeatedly:

  //update the current distance
  currentDistance = pingDistance();
  delayMicroseconds(60); //as per datasheet

  // a general filter. This sensor will not read past 300cm. Also, account for a base state of 0
  if (currentDistance < 300 && currentDistance > 0) {

    // a more advanced filter. The sensor data must be within 5 cm of the previous one to count
    if (currentDistance >= prevDistance - 5 && currentDistance <= prevDistance + 5) {
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
        Serial.print("\n");

        //resetting the pointer for the next array iteration and update currentDistance to median
        readingCount = -1;
        currentDistance = recentReadings[2];

        //map the distance (in cm) to a buzzer tone. Higher should be shriller
        toneFreq = map(int(currentDistance), 1, 300, 5000, 0);
        tone(3, toneFreq, 200);
      }
    }
  }

  // update the prev distance to the current distance
  prevDistance = currentDistance;
}
