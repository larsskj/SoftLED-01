/*
 * SoftLED-01
 * 
 * This small program is primarily meant for use in Danish model railroading.
 * 
 * It's intended to be used for a level crossing with flashing lights and 
 * provides soft turn on/off for LEDs.
 * 
 * It has a single input that switches between on and off state. This pin can 
 * be changed using a mechanical switch or something like a DCC accessory 
 * controller.
 * 
 * When off, the unorderPins are switched on to simulate a steady light in
 * the "Unorder signal" as it is called in translated Danish.
 * 
 * When on, the unorderPins are switched off and the warningsPins will 
 * provide a flashing light for red warning lights next to the road.
 * 
 * The circuit uses PWM outputs for controlling the LEDs. A standard
 * Arduino has six pins that can be used for PWM - 3, 5, 6, 9, 10, and 11. 
 * Each pin has a maximum current of 40 mA, so your LEDs cannot draw more
 * current on a single port.
 * 
 * Likewise, the Arduino has an overall maximum of 200 mA for all ports, 
 * so you cannot draw more total current on all ports combined.
 * 
 * LEDs should be connected to a positive voltage and will be connected 
 * to ground through the output pins.
 * 
 * Created May 2020 by Lars Skjærlund <lars@skjaerlund.dk>
 * 
 * The code is placed in the public domain.
 * 
 */


/*
 * User configurable pin settings. Use whatever fits your Arduino.
 */
byte inputPin = 2;
byte unorderPins[] = {10, 11};
byte warningPins[] = {3, 5, 6, 9};


/*
 * Timing values for turning LEDs on/off and flash length.
 */
unsigned int ledDn = 20;
unsigned int ledHold = 250;
unsigned int ledUp = 80;

unsigned int unorderDn = 40;
unsigned int unorderUp = 100;


/*
 * The program without any real user settings.
 */
const byte STATE_UNORDER = 0;
const byte STATE_WARNING = 1;
const byte STATE_UN2WARN = 2;
const byte STATE_WARN2UN = 3;

byte currentState;
byte desiredState;

bool inFlash = false;
unsigned int ledStart;


/*
 * Write to all unorderPins.
 */
void unorderWrite(byte value) {
  for (int i = 0; i < sizeof(unorderPins); i++) {
    analogWrite(unorderPins[i], value);
  }
}


/*
 * Write to all warningPins.
 */
void warningWrite(byte value) {
  for (int i = 0; i < sizeof(warningPins); i++) {
    analogWrite(warningPins[i], value);
  }
}


/*
 * Handle LED outputs according to time and state.
 */
void ledHandler() {
  unsigned int delta = millis() - ledStart;
  int i;

  switch (currentState) {
    case STATE_UN2WARN:
      if (delta < unorderDn * 8) {
        i = delta / unorderDn;
        unorderWrite(1 << i);
      } else {
        unorderWrite(255);
        currentState = STATE_WARNING;
      }
      break;
    case STATE_WARN2UN:
      if (delta < unorderUp * 8) {
        i = delta / unorderUp;
        unorderWrite(255 >> i);
      } else {
        unorderWrite(0);
        currentState = STATE_UNORDER;
      }
      break;
    case STATE_UNORDER:
      unorderWrite(0);
      break;
    case STATE_WARNING:
      if (delta < ledUp * 8) {
        i = delta / ledUp;
        warningWrite(255 >> i);
      } else if (delta < ledUp * 8 + ledHold) {
        warningWrite(0);
      } else if (delta < ledUp * 8 + ledHold + ledDn * 8) {
        i = (delta - ledUp * 8 - ledHold) / ledDn;
        warningWrite(1 << i);
      } else if (delta < ledUp * 8 + ledHold * 2 + ledDn * 8) {
        warningWrite(255);
      } else {
        warningWrite(255);
        inFlash = false;
      }
      break;
  }
}


/*
 * Get desired state by reading the inputPin.
 */
byte getState() {
  if (digitalRead(inputPin)) {
    return STATE_WARNING;
  } else {
    return STATE_UNORDER;
  }
}


/*
 * Standard Arduino setup() routine.
 */
void setup() {
  pinMode(inputPin, INPUT);
  
  currentState = getState();
  desiredState = getState();
}


/*
 * Standard Arduino loop() for running the program.
 */
void loop() {
  desiredState = getState();
  
  if (currentState == STATE_UNORDER && desiredState == STATE_WARNING) {
    ledStart = millis();
    currentState = STATE_UN2WARN;
  }
  
  if (currentState == STATE_WARNING && desiredState == STATE_UNORDER && !inFlash) {
    ledStart = millis();
    currentState = STATE_WARN2UN;
  }
  
  if (currentState == STATE_WARNING && desiredState == STATE_WARNING && !inFlash) {
    ledStart = millis();
    inFlash = true;
  }
  
  ledHandler();
}
