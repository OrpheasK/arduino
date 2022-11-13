//Project documentation: https://www.notion.so/Midterm-Project-41dad00bbb4643a4a792ac415b368ea7
//Contains various parts of code found in https://github.com/ITPNYU/physcomp/tree/main/Labs
const int trigPin = 2;
const int echoPin = 3;


void setup() {
  // set the modes for the trigger pin and echo pin:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // initialize serial communication:
  Serial.begin(9600);
}

 
void loop() {
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
  int distance = (duration * 0.0343) / 2;
  // remap the dist value to fit in 1 byte:
  //int mappedDist = map(distance, 0, 2000, 0, 255); 
  // print it out the serial port:
  Serial.println(distance);                             
  // slight delay to stabilize the ADC:
  delay(10);                                            
}
