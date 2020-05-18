byte unorderPins[] = {10, 11};
byte warningPins[] = {3, 5, 6, 9};

unsigned int ledDn = 20;
unsigned int ledHold = 250;
unsigned int ledUp = 80;

unsigned int unorderDn = 40;
unsigned int unorderUp = 100;

const byte STATE_UNORDER = 0;
const byte STATE_WARNING = 1;
const byte STATE_UN2WARN = 2;
const byte STATE_WARN2UN = 3;

byte currentState;
byte desiredState;

bool inFlash = false;
unsigned int ledStart;


void unorderWrite(byte value) {
  for (int i = 0; i < sizeof(unorderPins); i++) {
    analogWrite(unorderPins[i], value);
  }
}


void warningWrite(byte value) {
  for (int i = 0; i < sizeof(warningPins); i++) {
    analogWrite(warningPins[i], value);
  }
}


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


byte getState() {
  if (digitalRead(2)) {
    return STATE_WARNING;
  } else {
    return STATE_UNORDER;
  }
}


void setup() {
  pinMode(2, INPUT);
  
  currentState = getState();
  desiredState = getState();
}


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
