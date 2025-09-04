#include "MIDIUSB.h"

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
  delay(200);
}
