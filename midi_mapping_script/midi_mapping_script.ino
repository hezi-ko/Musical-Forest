#include "MIDIUSB.h"

void sendClipLaunch(byte noteNumber) {
  midiEventPacket_t noteOn = {0x09, 0x90, noteNumber, 127};
  midiEventPacket_t noteOff = {0x08, 0x80, noteNumber, 0};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
  delay(100);
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

void sendControlChange(byte ccNumber, byte ccValue) {
  // 0x0B = USB-MIDI “Event” packet, 0xB0 = Control Change on channel 1
  midiEventPacket_t cc = {0x0B, 0xB0, ccNumber, ccValue};
  MidiUSB.sendMIDI(cc);
  MidiUSB.flush();
}


void setup() {

  Serial.begin(115200);

  Serial.println("Go to Ableton");

  delay(4000);

  // delay(4000);
  // sendClipLaunch(80);

  // BENDY

  // delay(4000);
  // sendControlChange(114, 63);

  // delay(6000);
  // sendControlChange(115, 63);

  // delay(4000);
  // sendControlChange(112, 63);
  // delay(4000);
  // sendClipLaunch(125);
  // delay(4000);
  // sendClipLaunch(126);
  // delay(4000);
  // sendClipLaunch(127);


  // MUSIC

  delay(4000);
  sendClipLaunch(60);
  delay(4000);
  sendClipLaunch(61);
  delay(4000);
  sendClipLaunch(62);

  delay(4000);
  sendClipLaunch(63);
  delay(4000);
  sendClipLaunch(64);
  delay(4000);
  sendClipLaunch(65);

  delay(4000);
  sendClipLaunch(66);
  delay(4000);
  sendClipLaunch(67);
  delay(4000);
  sendClipLaunch(68);

  delay(4000);
  sendClipLaunch(69);
  delay(4000);
  sendClipLaunch(70);
  delay(4000);
  sendClipLaunch(71);

  delay(4000);
  sendClipLaunch(72);
  delay(4000);
  sendClipLaunch(73);
  delay(4000);
  sendClipLaunch(74);

  delay(4000);
  sendClipLaunch(75);
  delay(4000);
  sendClipLaunch(76);
  delay(4000);
  sendClipLaunch(77);

  Serial.println("MAPPING COMPLETE");


}

void loop() {

}
