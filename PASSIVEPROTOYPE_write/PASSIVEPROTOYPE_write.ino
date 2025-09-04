/*
 * MIDIUSB_test.ino
 *
 * Created: 4/6/2015 10:47:08 AM
 * Author: gurbrinder grewal
 * Modified by Arduino LLC (2015)
 */ 

#include "MIDIUSB.h"

const int trigPin = 9;
const int echoPin = 10;

bool played = true;

float duration, distance;

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;

  Serial.print("Distance: ");
  Serial.println(distance);

 if(distance < 20) {

    Serial.print("Distance: ");
    Serial.println(distance);

    // D Minor Chord

    Serial.println("Sending note on");
    noteOn(1, 50, 64);   
    noteOn(1, 53, 64);   
    noteOn(1, 57, 64);   
    MidiUSB.flush();
    delay(1500);
    Serial.println("Sending note off");
    noteOff(1, 50, 64);  
    noteOff(1, 53, 64);  
    noteOff(1, 57, 64);  
    MidiUSB.flush();
    delay(500);

    // F Major Chord

    Serial.println("Sending note on");
    noteOn(1, 53, 64);   
    noteOn(1, 57, 64);   
    noteOn(1, 60, 64);   
    MidiUSB.flush();
    delay(1500);
    Serial.println("Sending note off");
    noteOff(1, 53, 64);  
    noteOff(1, 57, 64);  
    noteOff(1, 60, 64);  
    MidiUSB.flush();
    delay(500);

    // C Major Chord

    Serial.println("Sending note on");
    noteOn(1, 48, 64);   
    noteOn(1, 52, 64);   
    noteOn(1, 55, 64);   
    MidiUSB.flush();
    delay(1500);
    Serial.println("Sending note off");
    noteOff(1, 48, 64);  
    noteOff(1, 52, 64);  
    noteOff(1, 55, 64);  
    MidiUSB.flush();
    delay(500);

    // G Major Chord

    Serial.println("Sending note on");
    noteOn(1, 43, 64);   
    noteOn(1, 47, 64);   
    noteOn(1, 50, 64);   
    MidiUSB.flush();
    delay(1500);
    Serial.println("Sending note off");
    noteOff(1, 43, 64);  
    noteOff(1, 47, 64);  
    noteOff(1, 50, 64);  
    MidiUSB.flush();
    delay(500);

    played = true;
  }
  else if(distance > 20 && distance < 30 && played == true) {

    Serial.print("Distance: ");
    Serial.println(distance);

    // C Major Chord

    Serial.println("Sending note on");
    noteOn(0, 48, 64);   
    noteOn(0, 52, 64);   
    noteOn(0, 55, 64);   
    MidiUSB.flush();
    delay(1500);
    Serial.println("Sending note off");
    noteOff(0, 48, 64);  
    noteOff(0, 52, 64);  
    noteOff(0, 55, 64);  
    MidiUSB.flush();
    delay(500);

    // G Major Chord

    Serial.println("Sending note on");
    noteOn(0, 43, 64);   
    noteOn(0, 47, 64);   
    noteOn(0, 50, 64);   
    MidiUSB.flush();
    delay(1500);
    Serial.println("Sending note off");
    noteOff(0, 43, 64);  
    noteOff(0, 47, 64);  
    noteOff(0, 50, 64);  
    MidiUSB.flush();
    delay(500);

    // A Minor Chord

    Serial.println("Sending note on");
    noteOn(0, 45, 64);   
    noteOn(0, 48, 64);   
    noteOn(0, 52, 64);   
    MidiUSB.flush();
    delay(1500);
    Serial.println("Sending note off");
    noteOff(0, 45, 64);  
    noteOff(0, 48, 64);  
    noteOff(0, 52, 64);  
    MidiUSB.flush();
    delay(500);

    // F Major Chord

    Serial.println("Sending note on");
    noteOn(0, 41, 64);   
    noteOn(0, 45, 64);   
    noteOn(0, 48, 64);   
    MidiUSB.flush();
    delay(1500);
    Serial.println("Sending note off");
    noteOff(0, 41, 64);  
    noteOff(0, 45, 64);  
    noteOff(0, 48, 64);  
    MidiUSB.flush();
    delay(500);

    played = false;

  }

  distance = 0;
  delay(30);
  // controlChange(0, 10, 65); // Set the value of controller 10 on channel 0 to 65
}