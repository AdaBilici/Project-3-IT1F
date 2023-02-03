#include <Servo.h>

// wheel movement and sensors
int wheelAForeward = 10;
int wheelABackward = 13;
int wheelBBackward = 12;
int wheelBForeward = 11;
int wheelBSensor = 5;
int wheelASensor = 7;

// echo sensor
int distanceTrig = 8;
int distanceEcho = 9;

//line sensors
int LS1 = A0;
int LS2 = A1;
int LS3 = A2;
int LS4 = A3;
int LS5 = A4;
int LS6 = A5;
int LS7 = A6;
int LS8 = A7;

// gripper
int gripperPin = 6;
Servo gripper;

// yellow led (for testing)
int Lled = 3;

long duration, distance;
void setup() {
  // put your setup code here, to run once:
pinMode(wheelABackward, OUTPUT);
pinMode(wheelAForeward, OUTPUT);
pinMode(wheelBBackward, OUTPUT);
pinMode(wheelBForeward, OUTPUT);
pinMode(wheelASensor, INPUT);
pinMode(wheelBSensor, INPUT);
pinMode(distanceTrig, OUTPUT);
pinMode(distanceEcho, INPUT);

pinMode(LS1, INPUT);
pinMode(LS2, INPUT);
pinMode(LS3, INPUT);
pinMode(LS4, INPUT);
pinMode(LS5, INPUT);
pinMode(LS6, INPUT);
pinMode(LS7, INPUT);
pinMode(LS8, INPUT);

gripper.attach(gripperPin);
pinMode(Lled, OUTPUT);
Serial.begin(9600);


}

void loop() {

}

// grab an object using a grabber, or move the grabber to the specified degrees using the optional argument
void grab(int degrees = 0){
  gripper.write(degrees);  
}

// moves the robot foreward, not practical, to be used for testing
void simpleForeward(){
  analogWrite(wheelAForeward, 255);
  analogWrite(wheelBForeward, 255);
}

// reads distances from the echo sensor
int getDistance(){
  digitalWrite(distanceTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(distanceTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(distanceTrig, LOW);

  duration = pulseIn(distanceEcho, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}
