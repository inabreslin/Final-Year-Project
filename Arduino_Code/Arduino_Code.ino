#include <Arduino.h>
#include "adf4351.h"

#define PIN_CS 9  ///< SPI chip select pin

ADF4351  vfo(PIN_CS, SPI_MODE0, 1000000UL , MSBFIRST) ;

String incomingInfo = "";

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
    if (incomingInfo.indexOf("SignalOff")> -1){
      vfo.disable() ;
      incomingInfo = "";
    }
    
    if (incomingInfo.indexOf("SignalOn")> -1){
      vfo.enable() ;
      incomingInfo = "";
    }
    incomingInfo = "";
  }
}
