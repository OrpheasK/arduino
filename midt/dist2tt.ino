//Project documentation: https://www.notion.so/Midterm-Project-41dad00bbb4643a4a792ac415b368ea7
//Contains various parts of code found in https://github.com/ITPNYU/physcomp/tree/main/Labs

const int motor1Pin = 9;   // Motor driver 1 leg 1
const int motor2Pin = 10;  // Motor driver 1 leg 2
const int pwmPin = 11;     // Motor driver PWM pin

const int motor3Pin = 4;  // Motor driver 2 leg 1
const int motor4Pin = 5;  // Motor driver 2 leg 2
const int pwmPin2 = 6;    // Motor driver PWM pin

const int trigPin = 2;  //distance sensor pins
const int echoPin = 3;

int openedlim = LOW;  //limit switch indicators at the two ends of the bag (open/closed)
int closedlim = LOW;
int flclose = 0;  //process of closing/opening zipper
int flopen = 0;
int rdopenlim = 0;  //indicating which limit switch is waiting to receive signal
int rdcloselim = 1;
int distance = 0;

void setup() {
  // set the modes for the trigger pin and echo pin:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(8, INPUT);  //limit switch
  pinMode(7, INPUT);  //limit switch
  // set all the other pins you're using as outputs:
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  pinMode(pwmPin2, OUTPUT);

  // set PWM enable pin high so that motor can turn on:
  digitalWrite(pwmPin, HIGH);
  digitalWrite(pwmPin2, HIGH);

  digitalWrite(motor1Pin, LOW);  // set leg 1 of the motor driver low
  digitalWrite(motor2Pin, LOW);  // set leg 2 of the motor driver low
  digitalWrite(motor3Pin, LOW);  // set leg 1 of the motor driver low
  digitalWrite(motor4Pin, LOW);  // set leg 2 of the motor driver low

  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  //read limit switch input depending on movement
  if (rdopenlim) {
    openedlim = digitalRead(7);
  }
  if (rdcloselim) {
    closedlim = digitalRead(8);
  }
  /*
  // take the trigger pin low to start a pulse:
  digitalWrite(trigPin, LOW);
  // delay 2 microseconds:
  delayMicroseconds(2);
  // take the trigger pin high:
  digitalWrite(trigPin, HIGH);
  // delay 10 microseconds:
  delayMicroseconds(10);
  // take the trigger pin low again to complete the pulse:
  digitalWrite(trigPin, LOW);

  // listen for a pulse on the echo pin:
  long duration = pulseIn(echoPin, HIGH);
  // calculate the distance in cm.
  //Sound travels approx.0.0343 microseconds per cm.,
  // and it's going to the target and back (hence the /2):
  int distance = (duration * 0.0343) / 2;*/
  //Serial.print("Distance: ");
  getDistance();
  Serial.println(distance);
  //Serial.println(openedlim);
  //flopen = 0;


  //conditions for initializing moevement on each direction
  if ((closedlim) && (distance <= 150) && (flclose == 0)) {
    flopen = 1;
  } else if ((openedlim) && (distance == 250) && (flopen == 0)) {
    flclose = 1;
  }

  //opening bag movement
  if (flopen) {
    //if (dir==-1){delay(1000);} //delay between changes in direction
    digitalWrite(motor1Pin, LOW);   // set leg 1 of the motor driver low
    digitalWrite(motor2Pin, HIGH);  // set leg 2 of the motor driver high
    digitalWrite(motor3Pin, LOW);   // set leg 1 of the motor driver low
    digitalWrite(motor4Pin, HIGH);  // set leg 2 of the motor driver high

    //Serial.println(24);

    //closing bag movement
  } else if (flclose) {
    //if (dir==1){delay(1000);} //delay between changes in direction
    digitalWrite(motor1Pin, HIGH);  // set leg 1 of the motor driver high
    digitalWrite(motor2Pin, LOW);   // set leg 2 of the motor driver low
    digitalWrite(motor3Pin, HIGH);  // set leg 1 of the motor driver high
    digitalWrite(motor4Pin, LOW);   // set leg 2 of the motor driver low
  }
  //stopping movement when bag fully opened
  if (openedlim == HIGH) {
    digitalWrite(motor1Pin, LOW);  // set leg 1 of the motor driver low
    digitalWrite(motor2Pin, LOW);  // set leg 2 of the motor driver low
    digitalWrite(motor3Pin, LOW);  // set leg 1 of the motor driver low
    digitalWrite(motor4Pin, LOW);  // set leg 2 of the motor driver low

    flopen = 0;
    //stopping movement when bag fully closed
  } else if (closedlim == HIGH) {
    digitalWrite(motor1Pin, LOW);  // set leg 1 of the motor driver low
    digitalWrite(motor2Pin, LOW);  // set leg 2 of the motor driver low
    digitalWrite(motor3Pin, LOW);  // set leg 1 of the motor driver low
    digitalWrite(motor4Pin, LOW);  // set leg 2 of the motor driver low

    flclose = 0;
  }

  //conditions to re-initialize movement once stopped, if relevant movement is caught by the sensor
  if ((openedlim == HIGH) && (flclose == 1)) {
    openedlim = LOW;
    rdopenlim = 0;
    rdcloselim = 1;
  }
  if ((closedlim == HIGH) && (flopen == 1)) {
    closedlim = LOW;
    rdcloselim = 0;
    rdopenlim = 1;
  }
  // a short delay between readings:
  delay(10);
}

//function for smoother distance sensor readings courtesy of Kai and Mashi
//https://github.com/mashiyatz/pcomp-midterm/blob/main/trick_or_treat_box/trick_or_treat_box.ino
void getDistance() {
  /*
  Calculate the distance to whatever is in
  front of the ultrasonic sensor. 
  */

  /* 
  Trigger pin low-high-low to send a pulse and
  calculate the average distance across 10 pulses.
  */
  // int totalTime = 0;
  int sumDistance = 0;

  for (int i = 0; i < 10; i++) {
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // totalTime += pulseIn(echoPin, HIGH);
    int dist = (pulseIn(echoPin, HIGH) * 0.0343) / 2;
    if (dist > 250) dist = 250;  // remove large values for calculation purposes
    sumDistance += dist;
    //}
  }

  distance = sumDistance / 10;
}
