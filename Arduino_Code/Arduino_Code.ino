#include <Arduino.h>
#include "adf4351.h"

#define PIN_CS 9  ///< SPI chip select pin

ADF4351  vfo(PIN_CS, SPI_MODE0, 1000000UL , MSBFIRST) ;

void setup() {
  // put your setup code here, to run once:
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
  
}

void loop()
{

//  /*!
//     change the step value to 100kHz
//     do a set of 5 channel steps change every 5 seconds
//     1,000.100 Mhz
//     1,000.200 Mhz
//     ...
//     1,000.500 Mhz
//
//     print out the PLL values
//
//  */
//  unsigned long newf ;
//  vfo.ChanStep = steps[4] ; ///< change to 100 kHz
//
//  for (int i = 0; i < 5; i++) {
//    delay(5000);
//    newf = vfo.cfreq + vfo.ChanStep ;
//
//    if ( vfo.setf(newf) == 0 ) {
//      Serial.print("setf() success freq:") ;
//      Serial.println(vfo.cfreq) ;
//    } else {
//      Serial.println("setf() error") ;
//    }
//
//    Serial.println(vfo.cfreq) ;
//    Serial.print("PLL INT:");
//    Serial.println(vfo.N_Int);
//    Serial.print("PLL FRAC:");
//    Serial.println(vfo.Frac);
//    Serial.print("PLL MOD:");
//    Serial.println(vfo.Mod);
//    Serial.print("PLL PFD:");
//    Serial.println(vfo.PFDFreq);
//    Serial.print("PLL output divider:");
//    Serial.println(vfo.outdiv);
//    Serial.print("PLL prescaler:");
//    Serial.println(vfo.Prescaler);
//    Serial.println("");
//  }

  /*!
     disable the output for 5 seconds
     and then enable it.
  */
  delay(10000);
  Serial.println("disable");
  vfo.disable() ;
  delay(5000);
  Serial.println("enable");
  vfo.enable() ;

  delay(5000) ;
}
