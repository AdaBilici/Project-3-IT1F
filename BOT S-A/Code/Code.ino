#include <QTRSensors.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Servo.h>

//int ledRed=13;
//int ledYellow=12;
//int ledGreen=11;
//int brightness=0;

//===[ DistanceSenzors ]============================

long durationFront;
int distanceFront;
const int trigPinFront = 8;
const int echoPinFront = 9;

long durationLeft;
int distanceLeft;
const int trigPinLeft = 2;
const int echoPinLeft = 3;

//===[ Gripper ]====================================

const int gripper_pin=4;
Servo gripper;

//bluetooth

const int BT_R=5;
const int BT_T=6;
SoftwareSerial BT_module(BT_R,BT_T);

//motor pins
const int  MOTOR_A1=13;
const int MOTOR_A2=12;  
const int MOTOR_B1=11;
const int MOTOR_B2=4;

//===[ Button Pins + States ]======================

/*
const int BUTTON1=2;
const int BUTTON2=3;
const int BUTTON3=4;

int buttonState1=0;
int buttonState2=0;
int buttonState3=0;
*/

//===[ Pixels ]====================================

const int PIXEL_PIN=7;
const int PIXEL_NUMBER=4;


//===[ Led Pixels ]================================

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
  pinMode(MOTOR_A1,OUTPUT);
  pinMode(MOTOR_A2,OUTPUT);
  pinMode(MOTOR_B1,OUTPUT);
  pinMode(MOTOR_B2,OUTPUT);
}
void moveForward(int power=255)
{
  digitalWrite(MOTOR_A1,power);
  digitalWrite(MOTOR_B1,power);
}

void moveBackwards(int power=255)
{
  digitalWrite(MOTOR_A2,power);
  digitalWrite(MOTOR_B2,power);
}

//===[SETUP ]============================

void setup() {
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

int value=0;

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


if(distanceFront > 0 && distanceLeft < 10)
{
 leds.fill(BLUE,0,4);
 leds.show();
 moveForward(255);
}
else {
  digitalWrite(MOTOR_A1,0);
  digitalWrite(MOTOR_B1,0);
    leds.fill(RED, 0, 4);
    leds.show();
  }
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
