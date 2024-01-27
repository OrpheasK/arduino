const int speakerPin = 9;  // the pin number for the speaker
int notePlay = 0;
float frequency;


void setup() {
  Serial.begin(9600);

  // make the two buttons inputs and the speaker pin an output:
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  //button 1 pushed
  if (digitalRead(2) == HIGH) {
    frequency = 220;
    notePlay = 1;
  //button 2 pushed
  }else if (digitalRead(3) == HIGH) {
    frequency = 440;
    notePlay = 1;
  } else {
    digitalWrite(speakerPin, LOW);
    notePlay = 0;
  }

  //if a button is pushed begin the duty cycle alternating based on the FSR analog input
  if (notePlay == 1) {
    int period = (1 / frequency) * 1000000;
    int analogValue = analogRead(A0);  // read the pot value
    int timbre = map(analogValue, 0, 950, 50, 900);
    float multiplier = timbre / 1000.0;
    // turn the speaker on:
    digitalWrite(speakerPin, HIGH);
    // delay half the period:
    delayMicroseconds(period * multiplier);
    // turn the speaker off:
    digitalWrite(speakerPin, LOW);
    // delay half the period:
    delayMicroseconds(period * (1 - multiplier));
    Serial.println(multiplier);
  }
}
