# arduino_sensor2MIDI
Arduino sketch for measuring intensity of light (or anything else!) across a number of sensors and outputting MIDI control 
change messages. Values are read and 'smoothed' with an exponential weighted moving average filter (inspiration from 
https://makeabilitylab.github.io/physcomp/advancedio/smoothing-input.html) to avoid 'noisy' readings. Before the filter is 
applied, the range of sensor values are constrained between a lower and upper limit. This is usefull for using the device 
in enviroments with different levels of ambient light or adjusting for time of day (uncommenting the for loop near the bottom 
of the sketch & plotting the raw sensor values, can be usfull for judging this).

The smoothed sensor values sent via MIDI control change messages can be used for controlling parameters in a software application 
such as Max/MSP or PureData. The sketch makes use of the MidiUSB library (https://www.arduino.cc/reference/en/libraries/midiusb/) 
to acheive this.
