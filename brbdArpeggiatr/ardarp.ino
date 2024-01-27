//consists of code found in https://github.com/tigoe/SoundExamples
#include "pitches.h"
//initial three notes loaded
int note1 = 55;  //G3 MIDI
int note2 = 59;  //B3 MIDI
int note3 = 62;  //D4 MIDI

void setup() {
  //Serial.begin(9600);

  //3 note pushbuttons, 2 transposing pushbuttons
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  int analogValue = analogRead(A0);  // read the pot value
  // to calculate the note duration, take one second
  // divided by the note type.
  //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  int divider = map(analogValue, 0, 1023, 4, 32);  //limit values between quarter and 32nd notes
  int noteDuration = 1000 / divider;

  //note combination to be arpeggiated
  if (digitalRead(2) && digitalRead(3) && digitalRead(4)) {
    int arr[] = { freqForm(note1), freqForm(note2), freqForm(note3), freqForm(note1 + 12), freqForm(note2 + 12), freqForm(note3 + 12) };
    arpeggiate(arr, 6, noteDuration);
  } else if (digitalRead(2) && digitalRead(3)) {
    int arr[] = { freqForm(note1), freqForm(note2), freqForm(note1 + 12), freqForm(note2 + 12) };
    arpeggiate(arr, 4, noteDuration);
  } else if (digitalRead(2) && digitalRead(4)) {
    int arr[] = { freqForm(note1), freqForm(note3), freqForm(note1 + 12), freqForm(note3 + 12) };
    arpeggiate(arr, 4, noteDuration);
  } else if (digitalRead(3) && digitalRead(4)) {
    int arr[] = { freqForm(note2), freqForm(note3), freqForm(note2 + 12), freqForm(note3 + 12) };
    arpeggiate(arr, 4, noteDuration);
  } else if (digitalRead(2) == HIGH) {
    int arr[] = { freqForm(note1), freqForm(note1 + 12) };
    arpeggiate(arr, 2, noteDuration);
  } else if (digitalRead(3) == HIGH) {
    int arr[] = { freqForm(note2), freqForm(note2 + 12) };
    arpeggiate(arr, 2, noteDuration);
  } else if (digitalRead(4) == HIGH) {
    int arr[] = { freqForm(note3), freqForm(note3 + 12) };
    arpeggiate(arr, 2, noteDuration);
  }

  //if transposition button pressed along with note button, move assigned note one semitone up
  if (digitalRead(6) && digitalRead(2)) note1++;
  if (digitalRead(6) && digitalRead(3)) note2++;
  if (digitalRead(6) && digitalRead(4)) note3++;
  
  //the same for one semitone down
  if (digitalRead(7) && digitalRead(2)) note1--;
  if (digitalRead(7) && digitalRead(3)) note2--;
  if (digitalRead(7) && digitalRead(4)) note3--;
}

//plays notes in array in sequence
void arpeggiate(int noteArray[], int length, int duration) {
  for (int i = 0; i < length; i++) {
    tone(8, noteArray[i], duration);
    delay(duration + 20);
  }
}

float freqForm(int noteValue) {
  float frequency = 440 * pow(2, ((noteValue - 69) / 12.0));
  return frequency;
}
