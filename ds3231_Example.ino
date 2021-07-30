
/*
This sketch is a modified version of the DS3231 example sketch avaiable as part of the Adafruit RTClib library. This sketch has been modified to use the SQW output pin of the DS3231 as an external
interrupt signal connected to pin 2 of the Arduino. This sketch configures the SQW pin of the DS3231 to output a 1Hz squarewave to trigger the Arduino to read new date and time info from the DS3231.

Link to the Adafruit DS3231 Imlementation Guide: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-ds3231-precision-rtc-breakout.pdf
Link to the Adafruit RTClib library: https://github.com/adafruit/RTClib
Link to the original Adafruit ds3231 example sketch: https://github.com/adafruit/RTClib/blob/master/examples/ds3231/ds3231.ino
*/

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

const byte SQWinput = 2; //Arduino pin used to monitor the SQW 1Hz output from the DS3231.  Pin 2 is an "interrupt-capable" pin on the Arduino Uno.  Note that different Arduino
                         //boards have specific pins which are inerrupt capable. Refer to www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/ for a complete list.

volatile int EDGE; //A variable to store when a falling 1Hz clock edge has occured on the SQW pin of the DS3231.
                   //It's recommended to make a variable volatile if it is to be shared with an Interrupt Service Routine (ISR)

//Setup the Real Time Clock, DS3231
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  Serial.begin(57600); //Make sure to set the serial monitor to 57600 baud to properly read the time and date transmissions
 
#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  
  //Configure SQW pin on the DS3231 to output a 1Hz squarewave to Arduino pin 2 (SQWinput) for timing
  rtc.writeSqwPinMode(DS3231_SquareWave1Hz);
  
  pinMode(SQWinput, INPUT); //Configure the SQWinput pin as an INPUT to monitor the SQW pin of the DS3231.
  digitalWrite (SQWinput, HIGH); //Enable the internal pull-up resistor, since the SQW pin on the DS3231 is an Open Drain output.
  attachInterrupt(digitalPinToInterrupt(SQWinput), Isr, FALLING); //Configure SQWinput (pin 2 of the Arduino) for use by the Interrupt Service Routine (Isr)
  EDGE = 1; //Initialize EDGE equal to 1. The Interrupt Service Routine will make EDGE equal to zero when triggered by a falling clock edge on SQW
}

//Interrupt Service Routine - This routine is performed when a falling edge on the 1Hz SQW clock from the RTC is detected
void Isr () {
    EDGE = 0; //A falling edge was detected on the SQWinput pin.  Now set EDGE equal to 0.
}

void loop (){
  //Test if EDGE has been made equal to zero by the Interrrupt Service Routine(ISR).  If it has, then update the time displayed on the clock
  if (EDGE == 0){
    displayTime ();
    EDGE = 1; //Now set EDGE equal to 1 after returning from the displayTime function.  The time will not be updated again until another
              //falling clock edge is detected on the SQWinput pin of the Arduino.
  }
}

void displayTime () {
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days, 12 hours, 30 minutes, 6 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));

    Serial.print(" now + 7d + 12h + 30m + 6s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();

    Serial.print("Temperature: ");
    Serial.print(rtc.getTemperature());
    Serial.println(" C");

    Serial.println();
    //delay(3000);
}
