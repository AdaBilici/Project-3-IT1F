#include <QTRSensors.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Servo.h>

//===[ Time variables ]=============================

int currentTime=0;
int startTime=0;

//===[ DistanceSenzors ]============================

long durationFront;
int distanceFront;
const int trigPinFront = 8;
const int echoPinFront = 9;

long durationLeft;
int distanceLeft;
const int trigPinLeft = 5;
const int echoPinLeft = 6;

//===[ Gripper ]====================================

const int gripper_pin=4;
Servo gripper;

//bluetooth

const int BT_R=5;
const int BT_T=6;
SoftwareSerial BT_module(BT_R,BT_T);

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

//Line sensor
//QTRSensors lineSensor;

//const uint8_t SensorCount = 8;
//uint16_t sensorValues[SensorCount];

//===[ Functions ]=================================

void setup_motor_pins()
{ 
  pinMode(motorRightBackwards,OUTPUT);
  pinMode(motorRightForward,OUTPUT);
  pinMode(motorLeftBackwards,OUTPUT);
  pinMode(motorLeftForward,OUTPUT);
}
void moveForward(int power=255)
{

  digitalWrite(motorLeftForward,215);
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

void rotateOnAxis()
{
  analogWrite(motorLeftBackwards,0);
  analogWrite(motorLeftForward, 200);
  analogWrite(motorRightForward, 0);
  analogWrite(motorRightBackwards,200);
}

void CountA()
{
  noInterrupts();
  countLeft++;
  Serial.println(countLeft);
  interrupts();
}

void CountB()
{
  noInterrupts();
  countRight++;
  Serial.println(countRight);
  interrupts();
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
  BT_module.begin(9600);
  leds.begin();
  leds.fill(BLUE,0,4);
  leds.show();
  setup_motor_pins();

//  gripper.attach(gripper_pin);
}

//===[ LOOP ]============================

void loop() {
  //distance 
  // Sets the trigPin on HIGH state for 10 micro secondssensor
  
  digitalWrite(trigPinLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinLeft, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationLeft = pulseIn(echoPinLeft, HIGH);
  // Calculating the distance
  distanceLeft = durationLeft * 0.034 / 2;
 

  digitalWrite(trigPinFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinFront, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationFront = pulseIn(echoPinFront, HIGH);
  // Calculating the distance
  distanceFront = durationFront * 0.034 / 2;
  countLeft=0;
  countRight=0;
  
  while(countLeft<80&&countRight<80)
  {moveForward(255);
   CountA();
   CountB();
  }
  stopRobot();
  delay(5000);
/*
if(distanceFront > 10 && distanceLeft < 10)
{
 leds.fill(BLUE,0,4);
 leds.show();
 rotateOnAxis();
 delay(250);
 stopRobot();
}
else {
 stopRobot();
    leds.fill(RED, 0, 4);
    leds.show();
  }
*/
}

//Line sensor
/*
 lineSensor.read(sensorValues);
  
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();

  if(analogRead(sensorValues[i]) == 1){
    //stop
    //open gripper and park
  }
  elseif (analogRead(sensorValues[i]) == 0){
    moveForward;
*/
