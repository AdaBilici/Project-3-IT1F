#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

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


//motor pins
const int  MOTOR_A1=13;
const int MOTOR_A2=12;  
const int MOTOR_B1=11;
const int MOTOR_B2=4;

//===[ Button Pins + States ]======================


const int BUTTON1=2;
const int BUTTON2=3;
const int BUTTON3=4;

int buttonState1=0;
int buttonState2=0;
int buttonState3=0;


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
<<<<<<< HEAD
=======

//Line sensor
QTRSensors lineSensor;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

>>>>>>> 7df6dc8818afd2238f964f3bc630e757268528ed
//===[ Functions ]=================================

void setup_motor_pins()
{ 
  pinMode(MOTOR_A1,OUTPUT);
  pinMode(MOTOR_A2,OUTPUT);
  pinMode(MOTOR_B1,OUTPUT);
  pinMode(MOTOR_B2,OUTPUT);
}
void moveForward(int power)
{
<<<<<<< HEAD
  leds.fill(BLUE,0,4);
  leds.show();
  digitalWrite(motorLeftForward,power);
  digitalWrite(motorRightForward,power);
=======
  digitalWrite(MOTOR_A1,power);
  digitalWrite(MOTOR_B1,power);
>>>>>>> 7df6dc8818afd2238f964f3bc630e757268528ed
}

void moveBackwards(int power=255)
{
  digitalWrite(MOTOR_A2,power);
  digitalWrite(MOTOR_B2,power);
}

<<<<<<< HEAD
void stopRobot() {
  analogWrite(motorRightBackwards,0);
  analogWrite(motorRightForward, 0);
  analogWrite(motorLeftBackwards, 0);
  analogWrite(motorLeftForward,0);
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
  while ((countLeft<nrOfPulses && countRight<nrOfPulses) && distanceLeft>=10)
    {
      rotateOnAxis();
     // showNrOfPulse();
      
      getDistanceLeft();
    }
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
  Serial.println(distanceLeft);
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
=======
>>>>>>> 7df6dc8818afd2238f964f3bc630e757268528ed
//===[SETUP ]============================

void setup() {
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinLeft, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinLeft, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  leds.begin();
  leds.fill(BLUE,0,4);
  leds.show();
  setup_motor_pins();
<<<<<<< HEAD
=======
  pinMode(BUTTON1,INPUT);
  pinMode(BUTTON2,INPUT);
  gripper.attach(gripper_pin);
  lineSensor.setTypeRC();
  lineSensor.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  lineSensor.setEmitterPin(2);
  //calibration
  pinMode(13, OUTPUT);
  for (uint16_t i = 0; i < 400; i++){
    lineSensor.calibrate();
  }
  for (uint8_t i = 0; i < SensorCount; i++){
    Serial.print(lineSensor.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
  for(uint8_t i = 0; i < SensorCount; i++){
    Serial.print(lineSensor.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(100);
  Serial.begin(9600);

//  gripper.attach(gripper_pin);

>>>>>>> 7df6dc8818afd2238f964f3bc630e757268528ed
}

int value=0;

//===[ LOOP ]============================

<<<<<<< HEAD
void loop()
{
 
 getDistanceLeft();
 getDistanceFront();
 if (distanceLeft<=15)
 {
  moveForward(200);
 }
 else
  {
    rotatePulses(72);
  }
 
}

  
=======
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


  if (buttonState2==LOW)
  {
    moveBackwards(255);
    }
  else{
    analogWrite(MOTOR_A2,0);
    analogWrite(MOTOR_B2,0);
    }

 if(buttonState1==LOW)
 {value++;
 gripper.write(180);
 delay(5000);
 }
 
 if(value>0)
 value --;
 
 gripper.write(0);
 leds.fill(START,0,4);
  leds.show();

 //Line sensor
  //calibration
  uint16_t position = lineSensor.readLineBlack(sensorValues);
  for (uint8_t i = 0; i < SensorCount; i++){
    Serial.print(sensorValues[i]);
    Serial.print('/t');
  }
  Serial.println(position);
  delay(250);

  //print values
  lineSensor.read(sensorValues);
  for (uint8_t i = 0; i < SensorCount; i++){
    Serial.print(sensorValues[i]);
    Serial.print('/t');
  }
  Serial.println();

}


//Functions
    // Clears the trigPin
  //digitalWrite(trigPin, LOW);
  //delayMicroseconds(2);
>>>>>>> 7df6dc8818afd2238f964f3bc630e757268528ed
