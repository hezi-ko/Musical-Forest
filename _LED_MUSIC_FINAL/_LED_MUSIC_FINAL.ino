#include "MIDIUSB.h"
#include <FastLED.h>

// LED Variables

#define NUM_LEDS 30
#define BRIGHTNESS 64

CRGB leds0[NUM_LEDS];
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS];
CRGB leds5[NUM_LEDS];

CRGB colors[4] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow};
bool isInitialized[6] = {false, false, false, false, false, false};

// Distance Sensor Variables

const int trigPins[6] = {7, 5, 6, 2, 3, 4};
const int echoPins[6] = {13, 11, 12, 8, 9, 10};

const byte midiNotes[6][3] = {
  {60, 61, 62}, // Sensor 1
  {63, 64, 65}, // Sensor 2
  {66, 67, 68}, // Sensor 3
  {69, 70, 71}, // Sensor 4
  {72, 73, 74}, // Sensor 5
  {75, 76, 77}  // Sensor 6
};

unsigned long lastTriggerTime[6] = {0, 0, 0, 0, 0, 0};
int currentLayer[6] = {-1, -1, -1, -1, -1, -1};

const unsigned long triggerCooldown = 3000;   // 3 seconds to prevent rapid retriggering
const unsigned long layerTimeout = 15000;     // 15 seconds of inactivity to auto-mute

bool hasStarted = false;

void setup() {

  Serial.begin(115200);

  for (int i = 0; i < 6; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.addLeds<WS2812B, A0, GRB>(leds0, NUM_LEDS);

  fill_solid(leds0, NUM_LEDS, CRGB::Black);
  FastLED.addLeds<WS2812B, A1, GRB>(leds1, NUM_LEDS);

  fill_solid(leds1, NUM_LEDS, CRGB::Black);
  FastLED.addLeds<WS2812B, A2, GRB>(leds2, NUM_LEDS);

  fill_solid(leds2, NUM_LEDS, CRGB::Black);
  FastLED.addLeds<WS2812B, A3, GRB>(leds3, NUM_LEDS);

  fill_solid(leds3, NUM_LEDS, CRGB::Black);
  FastLED.addLeds<WS2812B, A4, GRB>(leds4, NUM_LEDS);

  fill_solid(leds4, NUM_LEDS, CRGB::Black);
  FastLED.addLeds<WS2812B, A5, GRB>(leds5, NUM_LEDS);

  fill_solid(leds5, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

long getDistance0() {
  digitalWrite(2, LOW);
  delayMicroseconds(2);
  digitalWrite(2, HIGH);
  delayMicroseconds(10);
  digitalWrite(2, LOW);
  long duration = pulseIn(8, HIGH, 30000);
  return duration * 0.034 / 2;
}

long getDistance1() {
  digitalWrite(3, LOW);
  delayMicroseconds(2);
  digitalWrite(3, HIGH);
  delayMicroseconds(10);
  digitalWrite(3, LOW);
  long duration = pulseIn(9, HIGH, 30000);
  return duration * 0.034 / 2;
}
long getDistance2() {
  digitalWrite(4, LOW);
  delayMicroseconds(2);
  digitalWrite(4, HIGH);
  delayMicroseconds(10);
  digitalWrite(4, LOW);
  long duration = pulseIn(10, HIGH, 30000);
  return duration * 0.034 / 2;
}
long getDistance3() {
  digitalWrite(5, LOW);
  delayMicroseconds(2);
  digitalWrite(5, HIGH);
  delayMicroseconds(10);
  digitalWrite(5, LOW);
  long duration = pulseIn(11, HIGH, 30000);
  return duration * 0.034 / 2;
}
long getDistance4() {
  digitalWrite(6, LOW);
  delayMicroseconds(2);
  digitalWrite(6, HIGH);
  delayMicroseconds(10);
  digitalWrite(6, LOW);
  long duration = pulseIn(12, HIGH, 30000);
  return duration * 0.034 / 2;
}
long getDistance5() {
  digitalWrite(7, LOW);
  delayMicroseconds(2);
  digitalWrite(7, HIGH);
  delayMicroseconds(10);
  digitalWrite(7, LOW);
  long duration = pulseIn(13, HIGH, 30000);
  return duration * 0.034 / 2;
}

void sendClipLaunch(byte noteNumber) {
  midiEventPacket_t noteOn = {0x09, 0x90, noteNumber, 127};
  midiEventPacket_t noteOff = {0x08, 0x80, noteNumber, 0};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
  delay(100);
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

float readDistance(int sensorIndex) {
  digitalWrite(trigPins[sensorIndex], LOW);
  delayMicroseconds(2);
  digitalWrite(trigPins[sensorIndex], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPins[sensorIndex], LOW);

  long duration = pulseIn(echoPins[sensorIndex], HIGH, 30000);
  return duration > 0 ? (duration * 0.0343) / 2.0 : -1;
}

int detectLayer(float distance) {
  if (distance > 2 && distance <= 25) return 0;
  if (distance > 25 && distance <= 50) return 1;
  if (distance > 50 && distance <= 70) return 2;
  return -1;
}

void loop() {

  long distance0 = getDistance0();
  if (distance0 >= 70) {
    fill_solid(leds0, NUM_LEDS, CRGB::Black);
    isInitialized[0] = false;
  } else {
    if (!isInitialized[0]) {
      for (int j = 0; j < 12; j++) leds0[j] = colors[(j / 3) % 4];
      for (int j = 12; j < 30; j++) leds0[j] = CRGB::Black;
      isInitialized[0] = true;
    } else {
      CRGB last = leds0[11];
      for (int j = 11; j > 0; j--) leds0[j] = leds0[j - 1];
      leds0[0] = last;
    }
    for (int j = 12; j < 18; j++) leds0[j] = (distance0 < 50) ? CRGB(255,140,0) : CRGB::Black;
    for (int j = 18; j < 24; j++) leds0[j] = (distance0 < 30) ? CRGB(255,50,0) : CRGB::Black;
    for (int j = 24; j < 30; j++) leds0[j] = (distance0 < 20) ? CRGB::Red : CRGB::Black;
  }
  long distance1 = getDistance1();
  if (distance1 >= 70) {
    fill_solid(leds1, NUM_LEDS, CRGB::Black);
    isInitialized[1] = false;
  } else {
    if (!isInitialized[1]) {
      for (int j = 0; j < 12; j++) leds1[j] = colors[(j / 3) % 4];
      for (int j = 12; j < 30; j++) leds1[j] = CRGB::Black;
      isInitialized[1] = true;
    } else {
      CRGB last = leds1[11];
      for (int j = 11; j > 0; j--) leds1[j] = leds1[j - 1];
      leds1[0] = last;
    }
    for (int j = 12; j < 18; j++) leds1[j] = (distance1 < 50) ? CRGB(255,140,0) : CRGB::Black;
    for (int j = 18; j < 24; j++) leds1[j] = (distance1 < 30) ? CRGB(255,50,0) : CRGB::Black;
    for (int j = 24; j < 30; j++) leds1[j] = (distance1 < 20) ? CRGB::Red : CRGB::Black;
  }
  long distance2 = getDistance2();
  if (distance2 >= 70) {
    fill_solid(leds2, NUM_LEDS, CRGB::Black);
    isInitialized[2] = false;
  } else {
    if (!isInitialized[2]) {
      for (int j = 0; j < 12; j++) leds2[j] = colors[(j / 3) % 4];
      for (int j = 12; j < 30; j++) leds2[j] = CRGB::Black;
      isInitialized[2] = true;
    } else {
      CRGB last = leds2[11];
      for (int j = 11; j > 0; j--) leds2[j] = leds2[j - 1];
      leds2[0] = last;
    }
    for (int j = 12; j < 18; j++) leds2[j] = (distance2 < 50) ? CRGB(255,140,0) : CRGB::Black;
    for (int j = 18; j < 24; j++) leds2[j] = (distance2 < 30) ? CRGB(255,50,0) : CRGB::Black;
    for (int j = 24; j < 30; j++) leds2[j] = (distance2 < 20) ? CRGB::Red : CRGB::Black;
  }
  long distance3 = getDistance3();
  if (distance3 >= 70) {
    fill_solid(leds3, NUM_LEDS, CRGB::Black);
    isInitialized[3] = false;
  } else {
    if (!isInitialized[3]) {
      for (int j = 0; j < 12; j++) leds3[j] = colors[(j / 3) % 4];
      for (int j = 12; j < 30; j++) leds3[j] = CRGB::Black;
      isInitialized[3] = true;
    } else {
      CRGB last = leds3[11];
      for (int j = 11; j > 0; j--) leds3[j] = leds3[j - 1];
      leds3[0] = last;
    }
    for (int j = 12; j < 18; j++) leds3[j] = (distance3 < 50) ? CRGB(255,140,0) : CRGB::Black;
    for (int j = 18; j < 24; j++) leds3[j] = (distance3 < 30) ? CRGB(255,50,0) : CRGB::Black;
    for (int j = 24; j < 30; j++) leds3[j] = (distance3 < 20) ? CRGB::Red : CRGB::Black;
  }
  long distance4 = getDistance4();
  if (distance4 >= 70) {
    fill_solid(leds4, NUM_LEDS, CRGB::Black);
    isInitialized[4] = false;
  } else {
    if (!isInitialized[4]) {
      for (int j = 0; j < 12; j++) leds4[j] = colors[(j / 3) % 4];
      for (int j = 12; j < 30; j++) leds4[j] = CRGB::Black;
      isInitialized[4] = true;
    } else {
      CRGB last = leds4[11];
      for (int j = 11; j > 0; j--) leds4[j] = leds4[j - 1];
      leds4[0] = last;
    }
    for (int j = 12; j < 18; j++) leds4[j] = (distance4 < 50) ? CRGB(255,140,0) : CRGB::Black;
    for (int j = 18; j < 24; j++) leds4[j] = (distance4 < 30) ? CRGB(255,50,0) : CRGB::Black;
    for (int j = 24; j < 30; j++) leds4[j] = (distance4 < 20) ? CRGB::Red : CRGB::Black;
  }
  long distance5 = getDistance5();
  if (distance5 >= 70) {
    fill_solid(leds5, NUM_LEDS, CRGB::Black);
    isInitialized[5] = false;
  } else {
    if (!isInitialized[5]) {
      for (int j = 0; j < 12; j++) leds5[j] = colors[(j / 3) % 4];
      for (int j = 12; j < 30; j++) leds5[j] = CRGB::Black;
      isInitialized[5] = true;
    } else {
      CRGB last = leds5[11];
      for (int j = 11; j > 0; j--) leds5[j] = leds5[j - 1];
      leds5[0] = last;
    }
    for (int j = 12; j < 18; j++) leds5[j] = (distance5 < 50) ? CRGB(255,140,0) : CRGB::Black;
    for (int j = 18; j < 24; j++) leds5[j] = (distance5 < 30) ? CRGB(255,50,0) : CRGB::Black;
    for (int j = 24; j < 30; j++) leds5[j] = (distance5 < 20) ? CRGB::Red : CRGB::Black;
  }
  FastLED.show();
  // delay(50);

  for (int i = 0; i < 6; i++) {
    float distance = readDistance(i);
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(" Distance: ");
    Serial.println(distance);

    int detectedLayer = detectLayer(distance);

    if (!hasStarted && detectedLayer != -1) {
      sendClipLaunch(80); // Start song or initial playback
      hasStarted = true;
      delay(300);
    }

    if (hasStarted) {
      if (detectedLayer != -1 && detectedLayer != currentLayer[i] && millis() - lastTriggerTime[i] > triggerCooldown) {
        // New layer detected — trigger
        if (currentLayer[i] != -1) {
          sendClipLaunch(midiNotes[i][currentLayer[i]]); // Mute old
        }

        delay(20);
        sendClipLaunch(midiNotes[i][detectedLayer]); // Play new

        currentLayer[i] = detectedLayer;
        lastTriggerTime[i] = millis();
      }

      // Timeout reached — auto-mute current layer
      if (currentLayer[i] != -1 && millis() - lastTriggerTime[i] > layerTimeout) {
        Serial.print("Sensor ");
        Serial.print(i + 1);
        Serial.println(" timed out. Muting...");

        sendClipLaunch(midiNotes[i][currentLayer[i]]);
        currentLayer[i] = -1;  // Reset state
      }
    }

    delay(50);
  }

  Serial.println("-----------------------------------");
  delay(100);
}
