#include "Arduino.h"
#include "NewEncoder.h"

// Pins 2 and 3 should work for many processors, including Uno. See README for meaning of constructor arguments.
// Use FULL_PULSE for encoders that produce one complete quadrature pulse per detnet, such as: https://www.adafruit.com/product/377
// Use HALF_PULSE for endoders that produce one complete quadrature pulse for every two detents, such as: https://www.mouser.com/ProductDetail/alps/ec11e15244g1/?qs=YMSFtX0bdJDiV4LBO61anw==&countrycode=US&currencycode=USD
NewEncoder encoder(2, 3, -20, 20, 0, FULL_PULSE);
int16_t prevEncoderValue;

int switchPin = 10;                        // drukknop pin
int switchState = HIGH;                    // waarde van de drukknop

void setup() {
 
 pinMode (switchPin, INPUT_PULLUP);       // Stel de switchPin in als invoer met ingebouwde PULLUP weerstand
 
  NewEncoder::EncoderState state;

  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting");

  if (!encoder.begin()) {
    Serial.println("Encoder Failed to Start. Check pin assignments and available interrupts. Aborting.");
    while (1) {
      yield();
    }
  } else {
    encoder.getState(state);
    Serial.print("Encoder Successfully Started at value = ");
    prevEncoderValue = state.currentValue;
    Serial.println(prevEncoderValue);
  }
}

void loop() {
  int16_t currentValue;
  NewEncoder::EncoderState currentEncoderState;

  if (encoder.getState(currentEncoderState)) {
    Serial.print("Encoder: ");
    currentValue = currentEncoderState.currentValue;
    if (currentValue != prevEncoderValue) {
      Serial.println(currentValue);
      prevEncoderValue = currentValue;
    } else
      switch (currentEncoderState.currentClick) {
        case NewEncoder::UpClick:
          Serial.println("at upper limit.");
          break;

        case NewEncoder::DownClick:
          Serial.println("at lower limit.");
          break;

        default:
          break;
      }
  }

// DRUKKNOP
  bool switchState = digitalRead(switchPin);    // Lees de digitale waarde van de schakelaar (LOW/HIGH)
      delay (50);
  // Als de schakelaar is ingedrukt (LOW), print bericht
  if (switchState == LOW) {
      Serial.println("Schakelaar ingedrukt");

      
  } 

}
