//=============[LIBRARIES]=============

#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel
#include <Servo.h> // https://github.com/arduino-libraries/Servo
#include <QTRSensors.h> // https://github.com/pololu/qtr-sensors-arduino

#ifdef __AVR__
#include<avr/power.h> //required for 16 MHz Adafruit Trinket
#endif  
  
//=============[SET PINS]=============

const int leftBack = 11; // Left wheel back
const int leftForward = 6; // Left wheel forward
const int rightForward = 3; // Right wheel forward
const int rightBack = 5; // Right wheel back

const int PIN = 4;
const int numbPixels = 4;
Adafruit_NeoPixel pixels(numbPixels, PIN, NEO_GRB + NEO_KHZ800);

const int firstButtonPin = 12; // Button1 pin
const int secondButtonPin = 8; // Button2 pin
const int gripper = 7; // Gripper pin
Servo servoMotor; // Declaring servo object
  
//=============[DECLARE VARIABLES]=============

int pos = 40;
int buttonStateOne = 0;
int buttonStateTwo = 0;
int val;

unsigned long time;

QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

//=============[SETUP]=============

void setup() {
  // QTRsensor setup -----------------------------------------------------------------
  
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  Serial.begin(9600);
  pixels.begin();
  
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
  cornerLeft(); 
  wait(800);
}

//=============[LOOP]=============

void loop() {
 // sensorRead();
 // read raw sensor values
  qtr.read(sensorValues);

  // print the sensor values as numbers from 0 to 1023, where 0 means maximum
  // reflectance and 1023 means minimum reflectance
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();
  Serial.print("Distance = ");
  Serial.println(" cm");

  if (sensorValues[3] > 600 || sensorValues[4] > 600) {
    if (sensorValues[0] > 600 && sensorValues[1] > 600 && sensorValues[2] > 600) {
      cornerRight();
      wait(800);
      stopCar();
      pixels.setPixelColor(0, pixels.Color(0,0,0));
      pixels.setPixelColor(1, pixels.Color(0,0,0));
      pixels.setPixelColor(2, pixels.Color(55,0,0));
      pixels.setPixelColor(3, pixels.Color(0,0,0));
      pixels.show();
    }
     else if ((sensorValues[2] > 600 || sensorValues[3] > 600 || sensorValues[4] > 600) && (sensorValues[1]< 600 || sensorValues[2] < 600 || sensorValues[3]<600)) {
      moveForward();
      pixels.setPixelColor(0, pixels.Color(0,0,0));
      pixels.setPixelColor(1, pixels.Color(0,0,0));
      pixels.setPixelColor(2, pixels.Color(0,0,0));
      pixels.setPixelColor(3, pixels.Color(0,0,0));
    } else {
      moveForward();
      pixels.setPixelColor(0, pixels.Color(0,0,0));
      pixels.setPixelColor(1, pixels.Color(0,0,0));
      pixels.setPixelColor(2, pixels.Color(0,0,0));
      pixels.setPixelColor(3, pixels.Color(0,0,0));
      
    }
  } else if (sensorValues[0] > 600 || sensorValues[1] > 600 || sensorValues[2] > 600) {
    turnRight();
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.setPixelColor(1, pixels.Color(0,0,0));
    pixels.setPixelColor(2, pixels.Color(55,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));
    pixels.show();
    pixels.show();
  } else if (sensorValues[5] > 600 || sensorValues[6] > 600 || sensorValues[7] > 600) {
    turnLeft();
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.setPixelColor(1, pixels.Color(0,0,0));
    pixels.setPixelColor(2, pixels.Color(0,0,0));
    pixels.setPixelColor(3, pixels.Color(0,55,0));
    pixels.show();
  } else if (sensorValues < 600) {
    rotate();
    pixels.setPixelColor(0, pixels.Color(0,0,200));
    pixels.setPixelColor(1, pixels.Color(0,0,200));
    pixels.setPixelColor(2, pixels.Color(0,0,200));
    pixels.setPixelColor(3, pixels.Color(0,0,200));
    pixels.show();
  }
}
/**************[END OF PROGRAM]**************/

//=============[Functions]=============

void moveForward() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 230);
  analogWrite(rightForward, 210);
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
  analogWrite(leftForward, 110);
  analogWrite(rightForward, 210);
  analogWrite(rightBack,0);
}

void turnRight() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 220);
  analogWrite(rightForward, 100);
  analogWrite(rightBack,0);
}

void rotate() {
  analogWrite(leftBack,180);
  analogWrite(leftForward, 0);
  analogWrite(rightForward,155);
  analogWrite(rightBack,0);
}

void cornerRight(){
  analogWrite(leftBack,0); //Left wheel backward
  analogWrite(leftForward,220);//Left wheel forward
  analogWrite(rightForward,0); //Right wheel forward
  analogWrite(rightBack,0); //Right wheel backward   
} 

void cornerLeft(){
  analogWrite(leftBack,0); //Left wheel backward
  analogWrite(leftForward,0);//Left wheel forward
  analogWrite(rightForward,200); //Right wheel forward
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

void wait(int timeToWait_2){
  time = millis();
  
  while(millis() < time + timeToWait_2){
  }
}
