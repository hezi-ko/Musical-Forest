#include "MIDIUSB.h"

// Bend sensors will be mapped to the following MIDI notes: 100, 102, 103, 104, 105, 106
// const byte ccNumbers[6] = {122, 123, 124, 125, 126, 127};

const float VCC = 5.0;
const float R_DIV = 10000.0;

// 模拟口连接
const int flexPins[6] = {A0, A1, A2, A3, A4, A5};

// 每个 bend sensor 的中性、最小和最大电阻值
const float neutralResistance[6] = {821707.31, 477142.87, 625403.75, 40920.85, 103163.72, 49824.56};
const float minResistance[6] = {335608.09, 270000.0, 30.0};
const float maxResistance[6] = {2367049.25, 629375.00, 1237561.00, 79815.63, 108265.90, 110352.95};


void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; i++) {
    pinMode(flexPins[i], INPUT);
  }
}

float readAverageFlex(int pin, int samples = 10) {
  long total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(pin);
    delay(5);
  }
  return (float)total / samples;
}

int resistanceToCC(float Rflex, float Rmin, float Rmax) {
  Rflex = constrain(Rflex, Rmin, Rmax);
  return map(Rflex, Rmin, Rmax, 0, 127);
}

// map from neutral…max into 0…127
int resistanceToCC_abs(float Rflex, float Rneutral, float Rmax) {
  if (Rflex <= Rneutral) return 0;
  Rflex = constrain(Rflex, Rneutral, Rmax);
  return map(Rflex, Rneutral, Rmax, 0, 127);
}

// int resistanceToCC_abs(
//   float Rflex,
//   float Rneutral,
//   float Rmin,
//   float Rmax
// ) {
//   // how far you are from “flat” in either direction
//   float distIn  = abs(Rflex - Rneutral);
//   // the furthest possible distance
//   float maxDist = max(Rneutral - Rmin, Rmax - Rneutral);
//   distIn = constrain(distIn, 0, maxDist);
//   // map 0…maxDist to 0…127
//   return map(distIn, 0, maxDist, 0, 127);
// }

void loop() {

  for (int i = 0; i < 6; i++) {

  int sensorIndex = i; // Using only Sensor 0 for MIDI CC 127
  float ADCflex = readAverageFlex(flexPins[sensorIndex]);
  float Vflex = ADCflex * VCC / 1023.0;
  float Rflex = R_DIV * (VCC / Vflex - 1.0);

  float angle = 0.0;
  if (Rflex >= neutralResistance[sensorIndex]) {
    angle = (Rflex - neutralResistance[sensorIndex]) * (90.0 / (maxResistance[sensorIndex] - neutralResistance[sensorIndex]));
  } else {
    angle = (Rflex - neutralResistance[sensorIndex]) * (90.0 / (neutralResistance[sensorIndex] - minResistance[sensorIndex]));
  }

  // Clamp angle
  if (angle > 90) angle = 90;
  if (angle < -90) angle = -90;

  // Map bend angle (-90 to 90) to MIDI CC range (0–127)
  // int midiValue = map(angle, -90, 90, 0, 127);
  // midiValue = constrain(midiValue, 0, 127); // Ensure within MIDI limits

  int midiValue = resistanceToCC_abs(
    Rflex,
    neutralResistance[i],
    maxResistance[i]
  );

  Serial.print("Sensor ");
  Serial.print(i + 1);
  Serial.print(" - ADC: ");
  Serial.print((int)ADCflex);
  Serial.print(", Resistance: ");
  Serial.print(Rflex, 2);
  Serial.print(" ohms, Bend: ");
  Serial.print(angle, 2);
  Serial.println(" degrees");
  Serial.print("Midi value");
  Serial.println(midiValue);

  // Send MIDI CC message
  // midiEventPacket_t cc = {0x0B, 0xB0, 127, midiValue}; // 0xB0 = MIDI CC on channel 1
  // MidiUSB.sendMIDI(cc);
  // MidiUSB.flush();

  if (i == 1) {
  // send CC127 for sensor 2 only
    midiEventPacket_t cc = {0x0B, 0xB0, 110, (byte)midiValue};
    MidiUSB.sendMIDI(cc);
    MidiUSB.flush();
  } else if (i == 0) {

    midiEventPacket_t cc = {0x0B, 0xB0, 111, (byte)midiValue};
    MidiUSB.sendMIDI(cc);
    MidiUSB.flush();
  } else if (i == 2) {
    midiEventPacket_t cc = {0x0B, 0xB0, 112, (byte)midiValue};
    MidiUSB.sendMIDI(cc);
    MidiUSB.flush();
  }  else if (i == 3) {

    midiEventPacket_t cc = {0x0B, 0xB0, 113, (byte)midiValue};
    MidiUSB.sendMIDI(cc);
    MidiUSB.flush();
  } else if (i == 4) {
    midiEventPacket_t cc = {0x0B, 0xB0, 114, (byte)midiValue};
    MidiUSB.sendMIDI(cc);
    MidiUSB.flush();
  } else if (i == 5) {
    midiEventPacket_t cc = {0x0B, 0xB0, 115, (byte)midiValue};
    MidiUSB.sendMIDI(cc);
    MidiUSB.flush();
  } else {
    Serial.println("Do Nothing");
  }

  }

  Serial.println("------------");
  delay(50); // adjust for responsiveness vs. MIDI spam
}