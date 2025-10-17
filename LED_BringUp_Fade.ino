#include <Adafruit_NeoPixel.h>

const int ledPin = 2;
const int ledCount = 26;

int stripBrightness = 0;
int fadeAmount = 5;

  Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
}

void loop() {
  for (int i = 0; i < ledCount; i++) {
    strip.setPixelColor(i, strip.Color(stripBrightness, 0, 0));
  }
  strip.show();
  delay(30);

  stripBrightness += fadeAmount;

if (stripBrightness <= 0 || stripBrightness >= 255) {
  fadeAmount = -fadeAmount;
}

} 