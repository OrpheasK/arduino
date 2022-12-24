//Contains various pieces of code found in https://tigoe.github.io/SoundExamples/midi.html
//
//accelerometer using LIS3DH library example by Kevin Townsend
const int PIEZO_PIN = A0;  // Piezo output
#include "MIDIUSB.h"
#include "Wire.h"
#include "Adafruit_LIS3DH.h"
int amv;
Adafruit_LIS3DH accelerometer = Adafruit_LIS3DH();
// Variables:
byte note = 0;  // The MIDI note value to be played
int ptcho, prevptcho;
int distrd, freqrd, qrd, ndrd;
int pta;
int pv, mv, mpv, dv, fv, qv, ndv, ev;
int prevpv, prevdv, prevfv, prevqv, prevndv, prevev;
int tm = -1, tl = -1;
int pr1 = 1, pr2 = 1;
int p1, p2;

void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT);
  pinMode(4, INPUT);

  //initialize pitch values
  prevptcho = analogRead(A7);
  pv = 52;
  p1 = 52;
  p2 = 52;

  //initialize slider
  ndrd = analogRead(A6);
  prevndv = map(ndrd, 16, 875, 0, 127);

  //initialize accelerometer
  prevev = 126;

  prevdv = 0;
  prevfv = 0;
  prevqv = 0;

  while (!Serial)
    ;
  if (!accelerometer.begin(0x18)) {
    Serial.println("Couldn't start accelerometer. Check wiring.");
    while (true)
      ;  // stop here and do nothing
  }
  accelerometer.setRange(LIS3DH_RANGE_8_G);  // 2, 4, 8 or 16 G
}

void loop() {
  // Read Piezo ADC value in, and convert it to a voltage
  int piezoADC = analogRead(PIEZO_PIN);
  float piezoV = piezoADC / 1023.0 * 5.0;

  //reading of effects values
  distrd = analogRead(A1);  //distortion, ptn 1
  dv = map(distrd, 0, 1023, 0, 127);
  freqrd = analogRead(A2);  //filter frequency value, ptn 2
  fv = map(freqrd, 0, 1023, 0, 127);
  qrd = analogRead(A3);  //filter Q value, ptn 3
  qv = map(qrd, 0, 1023, 0, 127);

  float avs;
  for (int i = 0; i < 20; i++) {
    ndrd = analogRead(A6);  //note duration, slider average value
    if (ndrd >= 16 && ndrd <= 875) {
      mv = map(ndrd, 16, 875, 0, 127);
      avs += mv;
    }
  }

  ndv = int(round(avs / 20.0));

  //reading of pitch value, ptn 4
  // only latest addded note can change at one time
  int avp=0;
  for (int j = 0; j < 20; j++) {
    ptcho = 1023 - analogRead(A7);
    mpv = map(ptcho, 0, 1023, 40, 64);//limited pitch mapping
    avp += mpv;
  }
  //ndv = map(ndrd, 0, 1023, 0, 127);
  pv = int(round(avp / 20.0));

  //reading of expression pedal value, accelerometer average
  float accs = 0.0;
  int nea, amv;
  accelerometer.read();

  for (int i = 0; i < 20; i++) {
    nea = -floor(100 * convertReading(accelerometer.y));
    if (nea >= 25 && nea <= 90) {
      amv = map(nea, 25, 90, 0, 127);
      accs += amv;
    }
  }
  accs = round(accs / 20.0);
  ev = int(accs);


  //sending effects values in different channels

  if (abs(dv - prevdv) > 4) {
    noteOn(0x91, dv, 0x45);
    prevdv = dv;
  }
  // delay(50);
  if (abs(fv - prevfv) > 2){
    noteOn(0x92, fv, 0x45);
    prevfv = fv;
  }
  // //delay(50);
  if (abs(qv - prevqv) > 2){
    noteOn(0x93, qv, 0x45);
    prevqv = qv;
  }
  // delay(50);

  if (abs(ndv-prevndv)<50 && abs(ndv-prevndv)>2){
    if (ndv>=120) noteOn(0x94, 127, 0x45);
    else noteOn(0x94, ndv, 0x45);
    prevndv = ndv;
  }
  // delay(50);

  if (abs(ev-prevev)>3){
    noteOn(0x95, ev, 0x45);
    prevev = ev;
  }
  //delay(50);


  //triggering of note or chord
  if (piezoV > 0.7) {
    if (digitalRead(4)) {  //toggle switch pos
      //3-note chord
      Serial.println("3 notes");
      noteOn(0x90, p1, 0x45);
      noteOn(0x90, p2, 0x45);
      noteOn(0x90, pv, 0x45);
      delay(100);
    } else if (!digitalRead(3)) {
      //2-note chord
      Serial.println("2 notes");
      noteOn(0x90, p1, 0x45);
      noteOn(0x90, pv, 0x45);
      delay(100);
      p2 = pv;
    } else {  //single note
      //Serial.println(piezoV);  // Print the voltage.
      noteOn(0x90, pv, 0x45);
      delay(100);
      p1 = pv;
    }
  }
  //delay(100);


  //for manual mapping of knobs
  if (digitalRead(3) && digitalRead(4)) {
    pta = analogRead(A5);
    mv = map(pta, 0, 1023, 0, 127);
    // noteOn(0x94, mv, 0x45);
    delay(100);
    Serial.println(23);
  }
}

// plays a MIDI note. Doesn't check to see that
// cmd is greater than 127, or that data values are less than 127:
void noteOn(byte cmd, byte data1, byte data2) {
  /* First parameter is the event type (top 4 bits of the command byte).
      Second parameter is command byte combined with the channel.
      Third parameter is the first data byte
      Fourth parameter second data byte, if there is one:
  */
  midiEventPacket_t midiMsg = { cmd >> 4, cmd, data1, data2 };
  MidiUSB.sendMIDI(midiMsg);

  //prints the values in the serial monitor so we can see what note we're playing
  Serial.print("cmd: ");
  Serial.print(cmd);
  Serial.print(", data1: ");
  Serial.print(data1);
  Serial.print(", data2: ");
  Serial.println(data2);
}

// convert reading to a floating point number in G's:
float convertReading(int reading) {
  float divisor = 2 << (13 - accelerometer.getRange());
  float result = (float)reading / divisor;
  return result;
}
