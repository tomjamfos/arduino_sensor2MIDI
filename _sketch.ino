/*
Arduino sketch for measuring intensity of light (or anything else!) across a number of sensors and outputting MIDI control 
change messages. Values are read and 'smoothed' with an exponential weighted moving average filter (inspiration from 
https://makeabilitylab.github.io/physcomp/advancedio/smoothing-input.html) to avoid 'noisy' readings. Before the filter is 
applied, the range of sensor values are constrained between a lower and upper limit. This is usefull for using the device 
in enviroments with different levels of ambient light or adjusting for time of day (uncommenting the for loop near the bottom 
of the sketch & plotting the raw sensor values, can be usfull for judging this).

The smoothed sensor values sent via MIDI control change messages can be used for controlling parameters in a software application 
such as Max/MSP or PureData. The sketch makes use of the MidiUSB library (https://www.arduino.cc/reference/en/libraries/midiusb/) 
to acheive this.
*/
#include "MIDIUSB.h"
#include "EWMA.h"

const int numSensors = 4; //number of sensors to read from
const int sampleRate = 50; //set rate to read sensors

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