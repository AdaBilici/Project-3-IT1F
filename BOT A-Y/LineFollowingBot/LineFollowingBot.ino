#include <Servo.h>

// ================= PINS ====================
// wheel movement and sensors
const int wheelLeftForward = 3; //used to be 10
const int wheelLeftBackward = 13; //forget
const int wheelRightBackward = 12; //forget
const int wheelRightForward = 5; //used to be 11
const int wheelRightSensor = 5; //forget
const int wheelLeftSensor = 7; //forget

// echo sensor
const int distanceTrig = 8;
const int distanceEcho = 9;

//line sensors
const int lineSensors[] = {A6, A5, A4, A3, A2, A0}; 
const int lineSensorsLength = 6;

// line sensor value table
const int lineSensorValues[] = {-9, -3, -1, 1, 3, 9};

// line sensor bool values
boolean lineSensorBoolValues[6];

//line sensor minimum turn power in percent;
const int minTurnPower = 15;


// line sensor sensitivity (black values below this will not be considered)
const float lineSensorSensitivity = 700;

// gripper
const int gripperPin = 6;
const Servo gripper;

// yellow led (for testing)
int Lled = 3;

// operational variables
int lastTurnCoefficient;
long duration, distance;
int currentSpeedPercent;

// ================= SETUP ====================

void setup() {
// wheel movement
pinMode(wheelLeftBackward, OUTPUT);
pinMode(wheelLeftForward, OUTPUT);
pinMode(wheelRightBackward, OUTPUT);
pinMode(wheelRightForward, OUTPUT);

//sensors
pinMode(wheelLeftSensor, INPUT);
pinMode(wheelRightSensor, INPUT);
pinMode(distanceTrig, OUTPUT);
pinMode(distanceEcho, INPUT);

// assign line sensor input type (pinMode(A0-A7, INPUT))
for (int i = 0; i < lineSensorsLength; i++){
  pinMode(lineSensors[i], INPUT);
}

//gripper
gripper.attach(gripperPin);

//Led
pinMode(Lled, OUTPUT);

//serial
Serial.begin(9600);

// initial variables
currentSpeedPercent = 70;
}

// ================= LOOP ====================

void loop() {
  forewardTurn(getTurnCoefficient());
  //simpleForeward(currentSpeedPercent);
}

// ================= SENSORS ====================
// reads distances from the echo sensor
int getDistance(){
  digitalWrite(distanceTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(distanceTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(distanceTrig, LOW);

  duration = pulseIn(distanceEcho, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

// gets a list of boolean values (true if there's a black line, false if nothing is detected)
void updateSensorBoolList(){
  for (int i = 0; i < lineSensorsLength; i++){
    if (analogRead(lineSensors[i]) > lineSensorSensitivity){
      lineSensorBoolValues[i] = true;
    } else {
      lineSensorBoolValues[i] = false;  
    }
  }
}

// get the integer from line sensors (0 foreward, - left, + right)
int getTurnCoefficient(){
  updateSensorBoolList();
  int coefficient = 0;
  
  for (int i = 0; i < lineSensorsLength; i++){
    if (lineSensorBoolValues[i]){
      coefficient += lineSensorValues[i];
    }
  }
  coefficient = map(coefficient, lineSensorValues[0], -lineSensorValues[0], -100, 100); // actually working equivilant to: coefficient/
  lastTurnCoefficient = coefficient;
  
  return coefficient;
}

String getSensorBoolValuesString(){
  String msg = "";
  for (int i = 0; i < lineSensorsLength; i++){
    msg += "[";
    msg += lineSensorBoolValues[i];
    msg += "],";
  }
  return msg;
}

// ================= MOVEMENT ====================
// moves the robot foreward, not practical, to be used for testing
void simpleForeward(int speed){
  currentSpeedPercent = speed; 
  speed = scaleFromPercent(speed);
  
  analogWrite(wheelLeftForward, speed);
  analogWrite(wheelRightForward, speed);
}

// turns the robot by turnpercent (from -100 to 100) while still moving forewards
void forewardTurn(int turnPercent){
    int modTurnPercent = 100 - abs(turnPercent); // ie. turns -30 into 70
    if (modTurnPercent < minTurnPower){modTurnPercent = minTurnPower;};
    int turnWheelSpeed = scaleFromPercent(currentSpeedPercent / (100 / modTurnPercent)); // divides current speed by modified turnPercent fraction (50/(100/50) = 25)
    int forWheelSpeed = scaleFromPercent(currentSpeedPercent);

    analogWrite(wheelLeftBackward, 0);
    analogWrite(wheelRightBackward, 0);
    
  if (turnPercent < 0){
    analogWrite(wheelLeftForward, turnWheelSpeed);
    analogWrite(wheelRightForward, forWheelSpeed);
  }
  else if (turnPercent > 0){
    analogWrite(wheelLeftForward, forWheelSpeed);
    analogWrite(wheelRightForward, turnWheelSpeed);
  }
  else{
    simpleForeward(currentSpeedPercent);
  }
}

// grab an object using a grabber, or move the grabber to the specified degrees using the optional argument
void grab(int degrees = 0){
  gripper.write(degrees);  
}


// =========== SIMPLE FUNCTIONS ====================================
void forwards()
{
 analogWrite(wheelLeftForward, 255);
 analogWrite(wheelLeftBackward, 0);
 analogWrite(wheelRightForward, 255);
 analogWrite(wheelRightBackward, 0);
}

void backwards()
{
 analogWrite(wheelLeftForward, 0);
 analogWrite(wheelLeftBackward, 255);
 analogWrite(wheelRightForward, 0);
 analogWrite(wheelRightBackward, 255);
}

void sharpLeft()
{
 analogWrite(wheelLeftForward, 0);
 analogWrite(wheelLeftBackward, 0);
 analogWrite(wheelRightForward, 255);
 analogWrite(wheelRightBackward, 0);
}

void sharpRight()
{
 analogWrite(wheelLeftForward, 255);
 analogWrite(wheelLeftBackward, 0);
 analogWrite(wheelRightForward, 0);
 analogWrite(wheelRightBackward, 0);
}

void slowLeft()
{
 analogWrite(wheelLeftForward, 200);
 analogWrite(wheelLeftBackward, 0);
 analogWrite(wheelRightForward, 255);
 analogWrite(wheelRightBackward, 0);
}

void slowRight()
{
 analogWrite(wheelLeftForward, 255);
 analogWrite(wheelLeftBackward, 0);
 analogWrite(wheelRightForward, 200);
 analogWrite(wheelRightBackward, 0);
}

void stopCar()
{
 analogWrite(wheelLeftForward, 0);
 analogWrite(wheelLeftBackward, 0);
 analogWrite(wheelRightForward, 0);
 analogWrite(wheelRightBackward, 0);
}

void rotate()
{
 analogWrite(wheelLeftForward, 255);
 analogWrite(wheelLeftBackward, 0);
 analogWrite(wheelRightForward, 0);
 analogWrite(wheelRightBackward, 255);
}

// ================= CALCULATIONS ====================

// calculates an analogue pin value based on percentage given
int scaleFromPercent(float percent){
  return map(percent, 0, 100, 0, 255);
}
