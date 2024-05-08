#include "MIDIUSB.h"
#include "EWMA.h"

const int numSensors = 4; //number of sensors to read from
const int sampleRate = 50; //set rate to read sensors in ms

const int sensorMin = 950; //minimum sensor input value (0 by default)
const int sensorMax = 1023; //maximum sensor input value (1023 by default)

const float ewmaCoeff = 0.2; //set coefficient for exponential weighted moving average

const int sensorPins[numSensors] = {A0, A1, A2, A3}; //pins to read from
int rawValues[numSensors] = {0, 0, 0, 0}; //array to store raw sensor readings

EWMA smoothing[numSensors]; //array to contain EWMA objects
int smoothedValues[numSensors] = {0, 0, 0, 0}; //array to store smoothed sensor values

const int controllerNums[numSensors] = {20 , 21, 22, 23}; //set controller numbers
int midiValues[numSensors] = {0, 0, 0, 0}; //array to store MIDI CC values

void controlChange(byte channel, byte control, byte value) {

  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value}; //create message
  MidiUSB.sendMIDI(event); //send MIDI event!

}

void setup() {

  Serial.begin(9600); //set the data rate in Bauds for serial data transmission

  for(int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT); //setup input sensors
    smoothing[i] = EWMA(analogRead(sensorPins[i]), ewmaCoeff); //setup EWMA objects, passing initial sensor reading & coefficient
  }

}

void loop() {

  for(int i = 0; i < numSensors; i++) {
    rawValues[i] = analogRead(sensorPins[i]); //read sensors
    smoothedValues[i] = smoothing[i].process(constrain(rawValues[i], sensorMin, sensorMax)); //'smoothed' sensor values
    midiValues[i] = map(smoothedValues[i], sensorMin, sensorMax, 0, 127); //map sensor values to 0 - 127
    controlChange(0, controllerNums[i], midiValues[i]); //send MIDI event!
    MidiUSB.flush(); //wait for all messages to actually be sent
  }

  //uncomment below to plot all sensor raw input values, usefull when setting sensorMin & sensorMax!

  // for(int i = 0; i < numSensors; i++) {
  //   Serial.print("raw_val_");
  //   Serial.print(String(i));
  //   Serial.print(":");
  //   if(i < numSensors - 1) {
  //     Serial.print(rawValues[i]);
  //     Serial.print(", ");
  //   }
  //   else {Serial.println(rawValues[i]);}
  // }

  delay(sampleRate); //take sensor reading every 50ms

}
