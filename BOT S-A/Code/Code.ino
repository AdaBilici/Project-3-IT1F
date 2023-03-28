#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

//===[ Time variables ]=============================
unsigned long time;

//===[ DistanceSenzors ]============================

long durationFront;
int distanceFront;
const int trigPinFront = 8;
const int echoPinFront = 9;

long durationLeft;
int distanceLeft;
const int trigPinLeft = 5;
const int echoPinLeft = 6;


//===[ Motor pins ]======================

const int motorRightBackwards=13;
const int motorRightForward=12;  
const int motorLeftBackwards=11;
const int motorLeftForward=10;

//===[ Motor tests for pulses ]====================

const int motor_R1=2;
const int motor_R2=3;
int countLeft=0;
int countRight=0;
int countsLeft=0,previousCountLeft;
int countsRight=0,previousCountRight;


//===[ Led Pixels ]================================

const int PIXEL_PIN=7;
const int PIXEL_NUMBER=4;
Adafruit_NeoPixel leds(PIXEL_NUMBER, PIXEL_PIN, NEO_RGB + NEO_KHZ800);
const uint32_t RED=leds.Color(255,0,0);
const uint32_t YELLOW=leds.Color(255,150,0);
const uint32_t BLUE=leds.Color(0,0,255);
const uint32_t WHITE=leds.Color(255,255,255);
const uint32_t START=leds.Color(0,0,0);
//===[ Functions ]=================================

void setup_motor_pins()
{ 
  pinMode(motorRightBackwards,OUTPUT);
  pinMode(motorRightForward,OUTPUT);
  pinMode(motorLeftBackwards,OUTPUT);
  pinMode(motorLeftForward,OUTPUT);
}
void moveForward(int power)
{
  leds.fill(BLUE,0,4);
  leds.show();
  digitalWrite(motorLeftForward,power);
  digitalWrite(motorRightForward,power);
}

void moveBackwards(int power=255)
{
  digitalWrite(motorRightBackwards,power);
  digitalWrite(motorLeftBackwards,power);
}

void stopRobot() {
  analogWrite(motorRightBackwards,0);
  analogWrite(motorRightForward, 0);
  analogWrite(motorLeftBackwards, 0);
  analogWrite(motorLeftForward,0);
}
void turnLeft()
{
  leds.fill(RED, 0, 4);
  leds.show();
  analogWrite(motorLeftBackwards,0);
  analogWrite(motorLeftForward, 0);
  analogWrite(motorRightForward, 200);
  analogWrite(motorRightBackwards,0);
}
void rotateOnAxis()
{
  leds.fill(RED, 0, 4);
  leds.show();
  analogWrite(motorLeftBackwards,150);
  analogWrite(motorLeftForward, 0);
  analogWrite(motorRightForward, 150);
  analogWrite(motorRightBackwards,0);
}
void rotateCounterAxis()
{
  analogWrite(motorLeftBackwards,0);
  analogWrite(motorLeftForward, 150);
  analogWrite(motorRightForward, 0);
  analogWrite(motorRightBackwards,150);
}
void rotatePulses(int nrOfPulses)
{
  stopRobot();
  countLeft=0;
  countRight=0;
  getDistanceLeft();
  turnLeft();
  while ((countLeft<nrOfPulses && countRight<nrOfPulses) && distanceLeft>=10)
    {
     // showNrOfPulse(); 
      getDistanceLeft();
    }
  stopRobot();
}

void moveForwardOnPulses(int nrOfPulses)
{
  Serial.println("start moving");
  stopRobot();
  countLeft=0;
  countRight=0;
  moveForward(150);
  while ((countLeft<nrOfPulses && countRight<nrOfPulses))
  {
    showNrOfPulse();
  }
  Serial.println("stopMoving");
  stopRobot();
}
void showNrOfPulse()
{
 Serial.print(countLeft);
 Serial.print(" ");
 Serial.print(countRight);
 Serial.println();
}
void CountA()
{
  noInterrupts();
  countLeft++;
  interrupts();
}

void CountB()
{
  noInterrupts();
  countRight++;
  interrupts();
}

void getDistanceLeft()
{
  digitalWrite(trigPinLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinLeft, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationLeft = pulseIn(echoPinLeft, HIGH);
  // Calculating the distance
  distanceLeft = durationLeft * 0.034 / 2; 
}

void getDistanceFront()
{  
  digitalWrite(trigPinFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinFront, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationFront = pulseIn(echoPinFront, HIGH);
  // Calculating the distance
  distanceFront = durationFront * 0.034 / 2;
 
}

void wait(int waitingTime) {
  time = millis();
  while(millis() < time + waitingTime){
    }
}
//===[SETUP ]============================

void setup() {
  pinMode(motor_R1, INPUT_PULLUP);
  pinMode(motor_R2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motor_R1),CountA,CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_R2),CountB,CHANGE);
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinLeft, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinLeft, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  leds.begin();
  leds.fill(BLUE,0,4);
  leds.show();
  setup_motor_pins();
  // moveForwardOnPulses(20);
  rotatePulses(68);
  wait(2000);
}

//===[ LOOP ]============================

void loop()
{
 getDistanceLeft();
 getDistanceFront();
 if (distanceLeft<=10&&distanceFront>=10)
 {
  moveForwardOnPulses(50);
  stopRobot();
  wait(200);
 }
 else if(distanceLeft>10 && distanceFront>10)
  {
    moveForwardOnPulses(30);
    rotatePulses(50);
    stopRobot();
    wait(200);
    rotatePulses(68);
  }
 else if(distanceLeft<10&&distanceFront<=10)
 {
  rotateCounterAxis();
  wait(700);
  }

/*
 
 */
}

  
