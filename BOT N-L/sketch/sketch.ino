//=============[LIBRARIES]=============

#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel
#include <Servo.h> // https://github.com/arduino-libraries/Servo
#include <QTRSensors.h> // https://github.com/pololu/qtr-sensors-arduino
  
//=============[SET PINS]=============

const int leftBack = 11; // Left wheel back
const int leftForward = 6; // Left wheel forward
const int rightForward = 3; // Right wheel forward
const int rightBack = 5; // Right wheel back

const int PIN = 4;
const int numbPixels = 4;

const int firstButtonPin = 12; // Button1 pin
const int secondButtonPin = 8; // Button2 pin
const int gripper = 7; // Gripper pin
Servo servoMotor; // Declaring servo object
  
//=============[DECLARE VARIABLES]=============

int pos = 40;
int buttonStateOne = 0;
int buttonStateTwo = 0;
int val;

QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

//=============[SETUP]=============

void setup() {
  // QTRsensor setup -----------------------------------------------------------------
  
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  Serial.begin(9600);
  
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  for (uint16_t i = 0; i < 100; i++){
    if (i == 0) {
      moveForward();
      delay(270);
      stopCar();
    } else if (i == 25) {
      moveForward();
      delay(270);
      stopCar();
    } else if (i == 50) {
      moveForward();
      delay(270);
      stopCar();
    } else if (i == 75) {
      moveForward();
      delay(270);
      stopCar();
    } else if (i == 99) {
      closeGripper();
      delay(100);
      moveForward();
    }
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(700);
  
  // Gripper setup
  servoMotor.attach(7);
  pinMode(firstButtonPin, INPUT);
  pinMode(secondButtonPin, INPUT);

  // Wheels setup
  pinMode(leftBack, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBack, OUTPUT);
}

//=============[LOOP]=============

void loop() {
 sensorRead();

 if (sensorValues[3] > 600 || sensorValues[4] > 600) {
   if (sensorValues[0] > 600 && sensorValues[1] > 600 && sensorValues[2] > 600) {
     cornerRight();
     delay(800);
   } else if ((sensorValues[2] > 600 || sensorValues[3] > 600 || sensorValues[4] > 600) && (sensorValues[1] > 600 || sensorValues[2] > 600 || sensorValues[3] > 600)) {
     moveForward();
   } else {
     moveForward();
   }
 } else if (sensorValues[0] > 600 || sensorValues[1] > 600 || sensorValues[2] > 600) {
   turnRight();
 } else if (sensorValues[5] > 600 || sensorValues[6] > 600 || sensorValues[7] > 600) {
   turnLeft();
 } else if (sensorValues < 600) {
   rotate();
 }
}

/**************[END OF PROGRAM]**************/

//=============[Functions]=============

void moveForward() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 220);
  analogWrite(rightForward, 200);
  analogWrite(rightBack,0);
}

void stopCar() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightBack,0);
}

void moveBackwards() {
  analogWrite(leftBack,200);
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightBack, 190);
}

void turnLeft() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 100);
  analogWrite(rightForward, 230);
  analogWrite(rightBack,0);
}

void turnRight() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 230);
  analogWrite(rightForward, 80);
  analogWrite(rightBack,0);
}

void rotate() {
  analogWrite(leftBack,180);
  analogWrite(leftForward, 0);
  analogWrite(rightForward,150);
  analogWrite(rightBack,0);
}

void cornerLeft(){
  analogWrite(leftBack,0); //Left wheel backward
  analogWrite(leftForward,0);//Left wheel forward
  analogWrite(rightForward,210); //Right wheel forward
  analogWrite(rightBack,0); //Right wheel backward
}

void cornerRight(){
  analogWrite(leftBack,0); //Left wheel backward
  analogWrite(leftForward,230);//Left wheel forward
  analogWrite(rightForward,0); //Right wheel forward
  analogWrite(rightBack,0); //Right wheel backward
} 

void closeGripper() {
  servoMotor.write(35);  
}

void openGripper() {
  servoMotor.write(120);
}

void sensorRead() {
  uint16_t position = qtr.readLineBlack(sensorValues);

  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);

  delay(250);
}
