#include <Arduino.h>
#include "adf4351.h"
#include "BigNumber.h"
#include <Arduino_JSON.h>

#define PIN_CS 9  ///< SPI chip select pin

ADF4351  vfo(PIN_CS, SPI_MODE0, 1000000UL , MSBFIRST) ;

String incomingInfo = "";

String operation = "";
double measureTime = 0;
double stopTime = 0;
double powerFreq = 0;
double lskDemodFreq = 0;
double freq = 0;

void setup() {
  
  Serial.begin(9600) ;
  Wire.begin() ;

  vfo.pwrlevel = 3 ; ///< sets to +5 dBm output
  vfo.RD2refdouble = 0 ; ///< ref doubler off
  vfo.RD1Rdiv2 = 0 ;   ///< ref divider off
  vfo.ClkDiv = 150 ;
  vfo.BandSelClock = 200 ;
  vfo.RCounter = 1 ;  ///< R counter to 1 (no division)
  vfo.ChanStep = steps[6] ;  ///< set to 1 MHz steps

  /*!
     sets the reference frequency to 25 Mhz
  */
  if ( vfo.setrf(25000000UL) ==  0 )
    Serial.println("ref freq set to 25 Mhz") ;
  else
    Serial.println("ref freq set error") ;
    /*!
       initialize the chip
    */
    Serial.println("Initialise chip") ;
    vfo.init() ;

  vfo.enable() ;
  
  /*!
     do a freq change (433 Mhz)
  */
  delay(5000);
  Serial.println("setf() to 433 MHz") ;

  if ( vfo.setf(440000000UL) == 0 ) {
    Serial.print("setf() success freq:") ;
    Serial.println(vfo.cfreq) ;
  } else {
    Serial.println("setf() error") ;
  }

  delay(5000);
  /*!
     enable frequency output
  */
  Serial.println("Enable chip") ;
  vfo.enable() ;
  incomingInfo = "";
}

void loop()
{
  while (Serial.available() > 0) {
    incomingInfo = Serial.readString();
    JSONVar myObject = JSON.parse(incomingInfo);
    operation = myObject["operation"];

    if (operation == "change_power_frequency"){
     freq = myObject["frequency"];
     Serial.println(freq);
     if ( vfo.setf(freq) == 0 ) {
      Serial.print("setf() success freq:") ;
      Serial.println(vfo.cfreq) ;
     } 
     else {
      Serial.println("setf() error") ;
     }
    }
    else if (operation == "change_mixer_frequency"){
     freq = myObject["frequency"];
     Serial.println(freq);
     if ( vfo.setf(freq) == 0 ) {
      Serial.print("setf() success freq:") ;
      Serial.println(vfo.cfreq) ;
     } 
     else {
      Serial.println("setf() error") ;
     }
    }
    else if (operation == "stop_power"){
      stopTime = myObject["stop_time"];
      Serial.println(stopTime);
      vfo.disable() ;
      delay(stopTime);
      
    }
    else if (operation == "start_measurement"){
      measureTime = myObject["measure_time"];
      Serial.println(measureTime);
      vfo.enable() ;
      delay(measureTime);
    }
//    if (incomingInfo.indexOf("SignalOff")> -1){
//      int ind1 = incomingInfor.indexOf("SignalOff:");
//      vfo.disable() ;
//    }
//    else if (incomingInfo.indexOf("SignalOn")> -1){
//      vfo.enable() ;
//    }
    incomingInfo = "";
  }
}
