#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel
#include <Servo.h> // https://github.com/arduino-libraries/Servo/
#include <QTRSensors.h> // https://github.com/pololu/qtr-sensors-arduino
  
//=============[ Motor pins ]=============
const int leftBack = 11; // Left wheel back
const int leftForward = 6; // Left wheel forward
const int rightForward = 3; // Right wheel forward
const int rightBack = 5; // Right wheel back

//=============[ Rotation sensors ]=============
const int motor_R1 = 8;
const int motor_R2 = 2;
volatile int countLeft = 0;
volatile int countRight = 0;

//=============[ Gripper ]=============
const int gripper = 7;
Servo servoMotor;
int pos = 40;

//=============[ Neo pixels ]=============
const int PIN = 4;
const int numbPixels = 4;
Adafruit_NeoPixel pixels(numbPixels, PIN, NEO_GRB + NEO_KHZ800);

//=============[ Line sensor ]=============
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
unsigned long time;

boolean checkEnd = false;


//=============[SETUP]====================================================

void setup() {
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  
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

  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
  
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
  
  pinMode(motor_R1, INPUT_PULLUP);
  pinMode(motor_R2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motor_R1),countA,CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_R2),countB,CHANGE);

  pinMode(leftBack, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBack, OUTPUT);
  Serial.begin(9600);

  cornerLeft();
  wait(800);
}

//=============[LOOP]=============

void loop() {
  countRight = 0;
  countLeft = 0;
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
   if(sensorValues[0] > 600 && sensorValues[7] > 600){
      stopCar();
      wait(500);
      moveForward();
      wait(200);
      stopCar();
      wait(400);
      checkEnd = true;
       qtr.read(sensorValues);
      if (checkEnd = true) {
      if(sensorValues[1] < 600 && sensorValues[6] < 600){
          stopCar();
          wait(100);
          cornerRight();
          wait(800);
       }
       else  if(sensorValues[0] > 600 && sensorValues[7] > 600){
          openGripper();
          wait(200);
          moveBackwards();
          wait(1000);
          stopCar();
          wait(10000000);
        }
      }
    }
  if (sensorValues[3] > 600 || sensorValues[4] > 600) {
    if (sensorValues[0] > 600 && sensorValues[1] > 600 && sensorValues[2] > 600 && sensorValues[6] < 600 && sensorValues[7] < 600 ) {
      stopCar();
      wait(200);
      cornerRight();
      wait(800);
      /*if(sensorValues[6] > 600 && sensorValues[7] > 600 && sensorValues[8] > 600){
        cornerLeft();
      }*/
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
    }else {
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
  }else{
    wait(200);
    stopCar();
    wait(20);
    rotate();
  }
}
/**************[END OF PROGRAM]**************/

//=============[Functions]=============

void moveForward() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 210);
  analogWrite(rightForward, 185);
  analogWrite(rightBack,0);
}

void stopCar() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightBack,0);
}

void moveBackwards() {
  analogWrite(leftBack,215);
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightBack, 190);
}

void turnLeft() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 100);
  analogWrite(rightForward, 220);
  analogWrite(rightBack,0);
}

void turnRight() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 235);
  analogWrite(rightForward, 70);
  analogWrite(rightBack,0);
}

void rotate() {
  analogWrite(leftBack,175);
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
  servoMotor.write(41);  
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
void countA() {
  noInterrupts();
  countLeft++;
  interrupts();
}

void countB() {
  noInterrupts();
  countRight++;
  interrupts();
}
