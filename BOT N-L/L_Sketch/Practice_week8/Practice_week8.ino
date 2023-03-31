#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel
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

//=============[ Distance sensors ]=============
int trigPin = 13;
int echoPin = 12;
int distance;
int duration;

//=============[ Gripper ]=============
const int gripper = 7;

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
boolean startRace = false;

const int treshold = 600;


//=============[SETUP]====================================================

void setup() {
  openGripper();
  
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode(gripper, OUTPUT);
  
  pinMode(motor_R1, INPUT_PULLUP);
  pinMode(motor_R2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motor_R1),countA,CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_R2),countB,CHANGE);

  pinMode(leftBack, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBack, OUTPUT);
  Serial.begin(9600);
}

//=============[LOOP]=============

void loop() {
  countRight = 0;
  countLeft = 0;
  sensorRead();
    
  if (startRace == true) {
    if(sensorValues[0] > treshold && sensorValues[7] > treshold){ // try with sv1 and sv6
      stopCar();
      wait(200);
      moveForward();
      wait(100);
      stopCar();
      wait(200);
      checkEnd = true;
      sensorRead();
      if (checkEnd = true) {
        if (sensorValues[0] < treshold && sensorValues[7] < treshold) {
          stopCar();
          wait(100);
          cornerRight();
          wait(800);
        } else if (sensorValues[2] > treshold && sensorValues[5] > treshold) {
          openGripper();
          wait(200);
          moveBackwards();
          wait(1000);
          stopCar();
          wait(1000000);
        }
      }
    }
    
    if (sensorValues[3] > treshold || sensorValues[4] > treshold) {
      if (sensorValues[0] > treshold && sensorValues[1] > treshold && sensorValues[2] > treshold && sensorValues[6] < treshold && sensorValues[7] < treshold) {
        stopCar();
        wait(100);
        cornerRight();
        wait(800);
        pixels.setPixelColor(0, pixels.Color(0,0,0));
        pixels.setPixelColor(1, pixels.Color(0,0,0));
        pixels.setPixelColor(2, pixels.Color(55,0,0));
        pixels.setPixelColor(3, pixels.Color(0,0,0));
        pixels.show();
      } else if ((sensorValues[3] > treshold || sensorValues[4] > treshold) && (sensorValues[1]< treshold || sensorValues[2] < treshold || sensorValues[3]< treshold)) {
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
    } else if (sensorValues[1] > treshold || sensorValues[2] > treshold) {
      turnRight();
      pixels.setPixelColor(0, pixels.Color(0,0,0));
      pixels.setPixelColor(1, pixels.Color(0,0,0));
      pixels.setPixelColor(2, pixels.Color(55,0,0));
      pixels.setPixelColor(3, pixels.Color(0,0,0));
      pixels.show();
      pixels.show();
    } else if (sensorValues[5] > treshold || sensorValues[6] > treshold) {
      turnLeft();
      pixels.setPixelColor(0, pixels.Color(0,0,0));
      pixels.setPixelColor(1, pixels.Color(0,0,0));
      pixels.setPixelColor(2, pixels.Color(0,0,0));
      pixels.setPixelColor(3, pixels.Color(0,55,0));
      pixels.show();
    } else {
      wait(200);
      stopCar();
      wait(20);
      rotate();
    }
  } else {
    getStartDistance();
  }
}
/**************[END OF PROGRAM]**************/

//=============[Functions]=============

void moveForward() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 210);
  analogWrite(rightForward, 180);
  analogWrite(rightBack,0);
}

void stopCar() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightBack,0);
}

void moveBackwards() {
  analogWrite(leftBack,210);
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightBack, 190);
}

void turnLeft() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 90);
  analogWrite(rightForward, 185);
  analogWrite(rightBack,0);
}

void turnRight() {
  analogWrite(leftBack,0);
  analogWrite(leftForward, 200);
  analogWrite(rightForward, 80);
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
  analogWrite(leftForward,200);//Left wheel forward
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
  for (int i = 0; i < 10; i++) {
    digitalWrite(gripper, HIGH);
    delayMicroseconds(1000);
    digitalWrite(gripper, LOW);
    delay(10);
  }
}

void openGripper() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(gripper, HIGH);
    delayMicroseconds(1500);
    digitalWrite(gripper, LOW);
    delay(10);
  }
}

void sensorRead() {
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

void startFunction() {
  stopCar();
  wait(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  for (uint16_t i = 0; i < 100; i++){
    if (i == 0) {
      moveForward();
      delay(335);
      stopCar();
    } else if (i == 25) {
      moveForward();
      delay(335);
      stopCar();
    } else if (i == 50) {
      moveForward();
      delay(335);
      stopCar();
    } else if (i == 75) {
      moveForward();
      delay(335);
      stopCar();
    } else if (i == 99) {
      closeGripper();
    }
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
  startRace = true;
  cornerLeft();
  wait(800);
}

void getStartDistance() {
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    //Serial.println(distance);
    if (distance < 22) {
      startFunction();
    }
}
