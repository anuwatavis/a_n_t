#define solenoid 4
#define limitSwitch 5
#define encoderA 6
#define encoderB 7
#include <DMXSerial.h>
int counter = 0;
byte valueCount = 0;
byte stateValue = 0;
int aState;
int aLastState;
void setup() {
  delay(20000);
  DMXSerial.init(DMXController);
  pinMode(solenoid, INPUT_PULLUP);
  pinMode(limitSwitch, INPUT_PULLUP);
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  aLastState = digitalRead(encoderA);
  
}

void loop() {
  aState = digitalRead(encoderA);
  if (aState != aLastState) {
    if (digitalRead(encoderB) != aState) {
      counter ++;
    } else {
      counter --;
    }
  }
  aLastState = aState;
  valueCount = byte(counter);
  DMXSerial.write(1, valueCount);

  if (digitalRead(solenoid) == 1) { //solenoid ON
    stateValue = 16;
    DMXSerial.write(2, stateValue);
    DMXSerial.write(3, stateValue);
  } else { // solenoid OFF
    stateValue = 32;
    DMXSerial.write(2, stateValue);
    DMXSerial.write(3, stateValue);
  }
  if (digitalRead(limitSwitch) == 0) {
    stateValue = 128;
    DMXSerial.write(2, stateValue);
    DMXSerial.write(3, stateValue);
  }
  /*
  if (counter < 0) {
    stateValue = 64;
    DMXSerial.write(2, stateValue);
    DMXSerial.write(3, stateValue);
  }
  */
  
  DMXSerial.write(5, 11);
  DMXSerial.write(6, 12);
  DMXSerial.write(7, 13);
  delayMicroseconds(2000);
}
