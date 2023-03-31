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
int countLeft = 0;
int countRight = 0;

//=============[ Gripper ]=============
const int gripper = 7;
Servo servoMotor;
int pos = 40;

//=============[ Neo pixels ]=============
const int PIN = 4;
const int numbPixels = 4;

//=============[ Line sensor ]=============
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
unsigned long time;

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
      delay(280);
      stopCar();
    } else if (i == 25) {
      moveForward();
      delay(280);
      stopCar();
    } else if (i == 50) {
      moveForward();
      delay(280);
      stopCar();
    } else if (i == 75) {
      moveForward();
      delay(280);
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
  
  pinMode(motor_R1, INPUT);
  pinMode(motor_R2, INPUT);
  attachInterrupt(digitalPinToInterrupt(motor_R1),countB,CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_R2),countA,CHANGE);

  pinMode(leftBack, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBack, OUTPUT);
  Serial.begin(9600);

  cornerLeft();
  wait(700);
}

//=============[LOOP]====================================================

void loop() {
  sensorRead();
  countLeft = 0;
  countRight = 0;
  
  if (sensorValues[3] > 600 || sensorValues[4] > 600) {
    if (sensorValues[0] > 600 || sensorValues[1] > 600) {
      closeGripper();
      while(countRight < 120 && countLeft < 120) {
        closeGripper();
        cornerRight();
        countA();
        countB();
      }
      closeGripper();
    } else if (sensorValues[7] > 600 || sensorValues[6] > 600) {
      stopCar();
      wait(200);
      moveForward();
      wait(350);
      closeGripper();
    } else if ((sensorValues[2] > 600 || sensorValues[3] > 600 || sensorValues[4] > 600) && (sensorValues[1]< 600 || sensorValues[2] < 600 || sensorValues[3]<600)) {
      moveForward();
      closeGripper();
    } else {
      moveForward();
      closeGripper();
    }
  } else if (sensorValues[0] > 600 || sensorValues[1] > 600 || sensorValues[2] > 600) {
    turnRight();
    closeGripper();
  } else if (sensorValues[5] > 600 || sensorValues[6] > 600 || sensorValues[7] > 600) {
    turnLeft();
    closeGripper();
  } else if (sensorValues < 600) {
    while(countLeft < 102 && countRight < 102) {
      rotate();
      countA();
      countB();
      closeGripper();
    }
    stopCar();
    wait(200);
  }

    /*
  sensorRead();
  countLeft = 0;
  countRight = 0;
  
  if (sensorValues[3] > 600 || sensorValues[4] > 600) {
    if (sensorValues[3] > 600 && sensorValues[4] > 600) {
      moveForward();
    } else if (sensorValues[1] > 600 || sensorValues[2] > 600) {
      turnRight();
      closeGripper();
    } else if (sensorValues[5] > 600 || sensorValues[6] > 600) {
      turnLeft();
      closeGripper();
    } else {
      moveForward();
    }
  } else if (sensorValues < 600) {
    while(countLeft < 102 && countRight < 102) {
      rotate();
      countA();
      countB();
      closeGripper();
    }
    stopCar();
    wait(200);
  }
  
  if (sensorValues[0] > 600 || sensorValues[1] > 600) {
    while(countRight < 120 && countLeft < 120) {
        closeGripper();
        cornerRight();
        countA();
        countB();
    }
  }

  if (sensorValues[8] > 600 || sensorValues[7] > 600) {
    while(countRight < 120 && countLeft < 120) {
        closeGripper();
        cornerLeft();
        countA();
        countB();
    }
  }
  */
}

/**************[END OF PROGRAM]**************/

//=============[Functions]===============================================

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
  analogWrite(leftForward, 200);
  analogWrite(rightForward, 100);
  analogWrite(rightBack,0);
}

void rotate() {
  analogWrite(leftBack,200);
  analogWrite(leftForward, 0);
  analogWrite(rightForward,180);
  analogWrite(rightBack,0);
}

void cornerLeft(){
  analogWrite(leftBack,0); //Left wheel backward
  analogWrite(leftForward,0);//Left wheel forward
  analogWrite(rightForward,200); //Right wheel forward
  analogWrite(rightBack,0); //Right wheel backward
}

void cornerRight(){
  analogWrite(leftBack,0); //Left wheel backward
  analogWrite(leftForward,210);//Left wheel forward
  analogWrite(rightForward,0); //Right wheel forward
  analogWrite(rightBack,0); //Right wheel backward
}

void wait(int waitingTime) {
  time = millis();
  while(millis() < time + waitingTime){
  }
}

void countA() {
  noInterrupts();
  countLeft++;
  Serial.println(countLeft);
  interrupts();
}

void countB() {
  noInterrupts();
  countRight++;
  Serial.println(countRight);
  interrupts();
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
}
