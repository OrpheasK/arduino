#include "MIDIUSB.h"
#include <EncoderStepCounter.h>
#include <Bounce2.h>
#include "Arduino_LSM6DS3.h"

//bounce for buttons to avoid double press registering
//5 ms debounce between each press
Bounce button1 = Bounce(21, 5);
Bounce button2 = Bounce(20, 5);
Bounce button3 = Bounce(4, 5);
Bounce button4 = Bounce(5, 5);
Bounce button5 = Bounce(12, 5);
Bounce button6 = Bounce(17, 5);
Bounce button7 = Bounce(15, 5);
Bounce button8 = Bounce(11, 5);
Bounce button9 = Bounce(10, 1);
Bounce button10 = Bounce(9, 5);
Bounce button11 = Bounce(8, 5);
Bounce button12 = Bounce(14, 5);
Bounce buttonOct = Bounce(16, 5);
//slider buttons
Bounce buttonT = Bounce(6, 5);
Bounce buttonI = Bounce(7, 5);

//MIDI variables

byte ennote = 0;  //encoder MIDI note
byte tolg1 = 0;   //handle button 1
byte tolg2 = 0;   //handle button 2
byte tolgar[5] = {0, 32, 62, 92, 127};
int arri = 0;
int ocstp = 1;
int eco = 0xB0;   //Continuous controller category for encoder and accelerometer


// encoder pins:
const int pin1 = 2;
const int pin2 = 3;
// Create encoder instance:
EncoderStepCounter encoder(pin1, pin2);

// encoder previous position:
int oldPosition = 0;

//accelerometer values
float xAcc, yAcc, zAcc;
float xGyro, yGyro, zGyro;

//accelerometer midi notes
int dm, oldm=-1;

void setup() {
  Serial.begin(9600);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(20, INPUT);
  pinMode(21, INPUT);

  // Initialize encoder
  encoder.begin();

  // start the IMU:
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    // stop here if you can't access the IMU:
    while (true)
      ;
  }
}

void loop() {
  encoder.tick();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();
  button6.update();
  button7.update();
  button8.update();
  button9.update();
  button10.update();
  button11.update();
  button12.update();
  buttonOct.update();
  buttonT.update();
  buttonI.update();

  //pitch button presses, edge/push are flipped
  if (button1.risingEdge()) {
    noteOn(0x90, 60, 0x45);  //full octave of notes from C4 to C5
  }
  if (button2.risingEdge()) {
    noteOn(0x90, 61, 0x45);
  }
  if (button3.risingEdge()) {
    noteOn(0x90, 62, 0x45);
  }
  if (button4.risingEdge()) {
    noteOn(0x90, 63, 0x45);
  }
  if (button5.risingEdge()) {
    noteOn(0x90, 64, 0x45);
  }
  if (button6.risingEdge()) {
    noteOn(0x90, 65, 0x45);
  }
  if (button7.risingEdge()) {
    noteOn(0x90, 66, 0x45);
  }
  if (button8.risingEdge()) {
    noteOn(0x90, 67, 0x45);
  }
  if (button9.risingEdge()) {
    noteOn(0x90, 68, 0x45);
  }
  if (button10.risingEdge()) {
    noteOn(0x90, 69, 0x45);
  }
  if (button11.risingEdge()) {/////////
    noteOn(0x90, 70, 0x45);
  }
  if (button12.risingEdge()) {
    noteOn(0x90, 71, 0x45);
  }
  if (buttonOct.risingEdge()) {
    noteOn(0x90, 72, 0x45);
  }

//to send double
  // if (buttonT.risingEdge()) {
  //   noteOn(eco, 9, 6);  //////////////////////////////////0xB, millis
  // }
  // if (buttonT.fallingEdge()) {
  //   noteOn(eco, 9, 0);
  // }


  //slider button presses
  // if (buttonT.risingEdge()) {
  //   tolg1 = ~tolg1;
  //   tolg1 = tolg1 & 0x7F;
  //   noteOn(eco, 0x15, tolg1);  //////////////////////////////////0xB, millis
  // }
  // if (buttonI.risingEdge()) {
  //   tolg2 = ~tolg2;
  //   tolg2 = tolg2 & 0x7F;
  //   noteOn(eco, 0x16, tolg2);
  // }

  //if t1,t2>20

  //pitch button release
  if (button1.fallingEdge()) {
    noteOn(0x90, 60, 0x00);
  }
  if (button2.fallingEdge()) {
    noteOn(0x90, 61, 0x00);
  }
  if (button3.fallingEdge()) {
    noteOn(0x90, 62, 0x00);
  }
  if (button4.fallingEdge()) {
    noteOn(0x90, 63, 0x00);
  }
  if (button5.fallingEdge()) {
    noteOn(0x90, 64, 0x00);
  }
  if (button6.fallingEdge()) {
    noteOn(0x90, 65, 0x00);
  }
  if (button7.fallingEdge()) {
    noteOn(0x90, 66, 0x00);
  }
  if (button8.fallingEdge()) {
    noteOn(0x90, 67, 0x00);
  }
  if (button9.fallingEdge()) {
    noteOn(0x90, 68, 0x00);
  }
  if (button10.fallingEdge()) {
    noteOn(0x90, 69, 0x00);
  }
  if (button11.fallingEdge()) {
    noteOn(0x90, 70, 0x00);
  }
  if (button12.fallingEdge()) {
    noteOn(0x90, 71, 0x00);
  }
  if (buttonOct.fallingEdge()) {
    noteOn(0x90, 72, 0x00);
  }

  // slider toggl
  // if (buttonT.risingEdge()) {
  //   // tolg1 = ~tolg1;
  //   if (arri<4){ arri++;
  //   // else if (arri == 4) arri = 0; use to go round the array
  //   // tolg1 = tolg1 & tolgar[arri];
  //   noteOn(eco, 0x15, tolgar[arri]);}
  // }
  // if (buttonI.risingEdge()) {
  //   // tolg2 = ~tolg2;
  //   if (arri>0){ arri--;
  //   // else if (arri == 0) arri = 3;
  //   // tolg2 = tolg2 & tolgar[arri];
  //   noteOn(eco, 0x15, tolgar[arri]);}
  // }

    if (buttonT.risingEdge()) {           //workaround
    // tolg1 = ~tolg1;
    if (arri==0) ocstep = 1;
    else if (arri==4) ocstep = -1;

    arri += ocstep;   
    noteOn(eco, 0x15, tolgar[arri]);}
  }

  // if (digitalRead(buttonT)) accact=1;
  // else accact=0;


  // read encoder position:
  int position = encoder.getPosition();

  // sliding parameter:
  if (position != oldPosition) {
    // noteOn(0xB0, contnf, param);
    noteOn(eco, 0x14, ennote);  //set in undefined function number 0x14

    Serial.println(position);

    if (position > oldPosition) {
      if (position == 127) {
        encoder.reset();
      } else {
        Serial.println("up");
        if (ennote < 127) ennote++;
      }
    }

    if (position < oldPosition) {
      if (oldPosition == -128) {//-128
        encoder.reset();
      } else if (oldPosition != 127) {
        Serial.println("down");
        if (ennote > 0) ennote--;//0
      }
    }

    oldPosition = position;
  }

  // if accelerometer is ready to be read:
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    // read accelerometer and gyrometer:
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro);
  }
  // Serial.println(xAcc);

  
  float acshift; //accelerometer value roundup using range (-0.45, +0.45)
  if (xAcc<-.45) acshift = 0.0;
  else if (xAcc>.45) acshift = 0.90;
  else acshift = xAcc + .45;

  // Serial.println(acshift);
  if ((int(acshift*100)%4) == 0) {
    int mdmp;
    mdmp = int(map(acshift*100, 0, 90.0, 0, 52));

    // Serial.println(mdmp);
    if (mdmp!=oldm){
      noteOn(eco, 0x17, mdmp);
      oldm = mdmp;
    }
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