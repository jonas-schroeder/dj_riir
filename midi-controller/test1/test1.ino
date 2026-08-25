#include <MIDIUSB.h>

int buttonPin = 2;
int ledPin = 13;

int buttonState = 0;
int buttonStatePrev = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceTimer = 0;
const int debounceDelay = 10;

void setup() {
  Serial.begin(9600);
  
  pinMode( buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);

  // debouncing
  if (millis() - lastDebounceTime > debounceDelay){
    // button state changes
    if (buttonState != buttonStatePrev) {
      lastDebounceTime = millis();

      // pressed
      if(buttonState == LOW){
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
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

