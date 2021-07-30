
7-30-2021

This sketch is a modified version of the DS3231 example sketch avaiable as part of the Adafruit RTClib library. This sketch has been modified to use the
SQW output pin of the DS3231 as an external interrupt signal connected to pin 2 of the Arduino. This sketch configures the SQW pin of the DS3231 to output
a 1Hz squarewave to trigger the Arduino to read new date and time info from the DS3231.

This repository includes the breadboard wiring diagram in jpeg and Fritzing file formats.

Useful Links:
Adafruit DS3231 Imlementation Guide: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-ds3231-precision-rtc-breakout.pdf
Adafruit RTClib library: https://github.com/adafruit/RTClib
Original Adafruit ds3231 example sketch: https://github.com/adafruit/RTClib/blob/master/examples/ds3231/ds3231.ino