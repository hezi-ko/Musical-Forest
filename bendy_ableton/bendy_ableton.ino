#include "MIDIUSB.h"

// Bend sensors will be mapped to the following MIDI notes: 100, 102, 103, 104, 105, 106
const byte ccNumbers[6] = {122, 123, 124, 125, 126, 127};

const float VCC = 5.0;
const float R_DIV = 10000.0;

const int flexPins[6] = {A0, A1, A2, A3, A4, A5};

// Resistance calibration for each sensor (6 total)
const float neutralResistance[6] = {81000.0, 44000.0, 40000.0, 63000.0, 40000.0, 76000.0};
const float minResistance[6]     = {60000.0, 30000.0, 21000.0, 34000.0, 26000.0, 35000.0};
const float maxResistance[6]     = {168000.0, 90000.0, 130000.0, 140000.0, 135000.0, 185000.0};

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 6; i++) {
    pinMode(flexPins[i], INPUT);
  }
}

float readAverageFlex(int pin, int samples = 10) {
  long total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(pin);
    delay(5);
  }

  // int average = (float)total / samples;

  // Serial.println("PIN: " + pin);
  // Serial.println("AVERAGE: " + average);

  return (float)total / samples;
}

int resistanceToCC(float Rflex, float Rmin, float Rmax) {
  // clamp to calibration bounds
  Rflex = constrain(Rflex, Rmin, Rmax);
  // map linearly to 0–127
  return map(Rflex, Rmin, Rmax, 0, 127);
}

void loop() {
  Serial.println("_________");

  for (int i = 0; i < 6; i++) {
    float ADCflex = readAverageFlex(flexPins[i]);
    float Vflex = ADCflex * VCC / 1023.0;
    float Rflex = R_DIV * (VCC / Vflex - 1.0);


    // in loop():
    int ccValue = resistanceToCC(
      Rflex,
      minResistance[i],
      maxResistance[i]
    );


    // Map each sensor to a different CC number (e.g., CC 127 down to CC 122)
    byte ccNumber = ccNumbers[i];;
    midiEventPacket_t cc = {0x0B, 0xB0, ccNumber, ccValue};
    MidiUSB.sendMIDI(cc);

    // Debug output
    // Serial.print("Sensor ");
    // Serial.print(i + 1);
    // Serial.print(" → CC ");
    // Serial.print(ccNumbers[i]);
    // Serial.print(" = ");
    // Serial.println(ccValue);

    // Debug print
    // Serial.print("Sensor ");
    // Serial.println(i + 1);
    // Serial.print("ADC: "); Serial.println((int)ADCflex);
    // Serial.print("Resistance: "); Serial.print(Rflex, 2); Serial.println(" ohms");
    // Serial.print("Angle: "); Serial.print(angle, 1); Serial.println(" deg");
    // Serial.print("CC: "); Serial.print(ccNumbers[i]); Serial.print(" → Value: "); Serial.println(ccValue);
    // Serial.println();

      Serial.print("Sensor ");  
      Serial.println(i + 1);
      Serial.print("  ADC: ");       
      Serial.println((int)ADCflex);
      Serial.print("  Resistance: ");
      Serial.print(Rflex, 2);
      Serial.println(" ohms");
      Serial.print("  CC: ");        
      Serial.print(ccNumber);
      Serial.print(" → Value: ");    
      Serial.println(ccValue);
      Serial.println();
  }

  MidiUSB.flush();
  Serial.println("_________");
  delay(200); // update rate
}

