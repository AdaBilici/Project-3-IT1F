//=============[LIBRARIES]=============
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel
#include <Servo.h> // https://github.com/arduino-libraries/Servo

//=============[SET PINS]=============
const int leftBack = 11; // Left wheel back
const int leftForward = 10; // Left wheel forward
const int rightForward = 9; // Right wheel forward
const int rightBack = 5; // Right wheel back

const int PIN = 4;
const int numbPixels = 4;

const int firstButtonPin = 12; // B1 pin
const int secondButtonPin = 8; // B2 pin
const int gripper = 7; // Gripper pin
Servo servoMotor; // Declaring servo object

//=============[DECLARE VARIABLES]=============
int pos = 0;
int buttonStateOne = 0;
int buttonStateTwo = 0;

//=============[SETUP]=============
void setup() {
  servoMotor.attach(7);
  pinMode(firstButtonPin, INPUT);
  pinMode(secondButtonPin, INPUT);

  Serial.begin(9600);
  pinMode(leftBack, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBack, OUTPUT);
}
//=============[LOOP]=============
void loop() {
  moveBackwards();
  
  closeGripper();
  openGripper();
}

/**************[END OF PROGRAM]**************/

//=============[Functions]=============
void moveForward() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 255);
  analogWrite(rightForward, 255);
  analogWrite(rightBack,0);
}

void moveBackwards() {
  analogWrite(leftBack,255);
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightBack, 255);
}

void turnLeft() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 255);
  analogWrite(rightForward, 0);
  analogWrite(rightBack,0);
}

void turnRight() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 255);
  analogWrite(rightBack,0);
}

void rotateOnAxis() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 255);
  analogWrite(rightForward, 0);
  analogWrite(rightBack,255);
}

void closeGripper() { // Close gripper with button one
  buttonStateOne = digitalRead(firstButtonPin);
  if (buttonStateOne == LOW) {
    servoMotor.write(40);  
  }
}

void openGripper() {
  buttonStateTwo = digitalRead(secondButtonPin);
  if (buttonStateTwo == LOW) { // Close gripper with button two
    servoMotor.write(100);
  }
}
