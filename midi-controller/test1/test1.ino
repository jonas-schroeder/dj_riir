#include <MIDIUSB.h>
#include <ResponsiveAnalogRead.h>

int ledPin = 13;
byte ledVal = 0;
// button
int buttonPin = 2;
int buttonState = 0;
int buttonStatePrev = 0;

// debouncing button
unsigned long lastDebounceTime = 0;
unsigned long debounceTimer = 0;
const int debounceDelay = 10;

// potentiometer
int potPin = A0;
const int POT_CC = 10;
int midiVal = 0;
int midiValPrev = 0;

ResponsiveAnalogRead responsivePot(potPin, true);

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  responsivePot.setAnalogResolution(1024);  // number of values, not highes value -> 1024 not 1023, default value -> maybe omit
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);

  // debouncing
  if (millis() - lastDebounceTime > debounceDelay) {
    // button state changes
    if (buttonState != buttonStatePrev) {
      lastDebounceTime = millis();

      // pressed
      if (buttonState == LOW) {
        Serial.println("PRESSED");
        noteOn(0, 38, 127);
        MidiUSB.flush();
      } else {
        Serial.println("RELEASED");
        noteOff(0, 38, 0);
        MidiUSB.flush();
      }
      buttonStatePrev = buttonState;
    }
  }





  responsivePot.update();

  // if the responsive value has change, print out 'changed'
  if (responsivePot.hasChanged()) {
    int potValue = responsivePot.getValue();
    midiVal = map(potValue, 0, 1023, 0, 127);

    if (midiVal != midiValPrev) {
      <Serial.println(potValue);
      Serial.println(midiVal);


      controlChange(0, POT_CC, midiVal);
      MidiUSB.flush();
      ledVal = map(potValue, 0, 1023, 0, 255);

      analogWrite(ledPin, ledVal);

      midiValPrev = midiVal;
    }
  }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}