//=============[LIBRARIES]=============
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel
#include <Servo.h> // https://github.com/arduino-libraries/Servo

//=============[SET PINS]=============
const int a1 = 11; //
const int a2 = 10; //
const int b1 = 9; //
const int b2 = 6; // 
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
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
}
//=============[LOOP]=============
void loop() {
  analogWrite(a1, 255);
  /*closeGripper();
  openGripper();*/
}

/**************[END OF PROGRAM]**************/

//=============[Functions]=============
void closeGripper() { // Close gripper with button one
  buttonStateOne = digitalRead(firstButtonPin);
  if (buttonStateOne == LOW) {
    servoMotor.write(40);  
  }
}

void openGripper() {
  buttonStateTwo = digitalRead(secondButtonPin);
  if (buttonStateTwo == LOW) { // Close gripper with button two
    servoMotor.write(120);
  }
}
