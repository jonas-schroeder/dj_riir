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
int potReading = 0; 
int potState = 0;
int potStatePrev = 0;
int potPin = A0;
int potMidiState = 0;
int potMidiStatePrev = 0;
const int POT_CC = 10;
const int POT_THRESHOLD = 4;
const int POT_TIMEOUT = 300;
unsigned long potTimePrev = 0;
unsigned long potTimer = 0;

float snapMultiplier = 0.01; // default value, maybe omit
ResponsiveAnalogRead responsivePot(pin=potPin, sleepEnable=true, snapMultiplier=snapMultiplier);

void setup() {
  Serial.begin(9600);
  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  responsivePot = ResponsiveAnalogRead(0, true, snapMultiplier);
  responsivePot.setAnalogResolution(1024); // number of values, not highes value -> 1024 not 1023, default value -> maybe omit 
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




  
  potReading = analogRead(potPin);
  responsivePot.update(potReading);
  potState = responsivePot.getValue();
  potMidiState = map (potState, 0, 1023, 0, 127);

  // ignore noise
  int potVar = abs(potState - potStatePrev);
  if (potVar > POT_THRESHOLD){
    potTimePrev = millis();
  }

  potTimer = millis() - potTimePrev; 

  if (potTimer < POT_TIMEOUT){
    if (potMidiState != potMidiStatePrev){
      controlChange(0, POT_CC, potMidiState);
      MidiUSB.flush();

      Serial.println(potReading);

      Serial.println(potMidiState);
      ledVal = map(potState, 0, 1023, 0, 255);
      analogWrite(ledPin, ledVal);

      potMidiStatePrev = potMidiState;
    }
    potStatePrev = potState;
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

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}