#include <Servo.h>

// ================= PINS ====================
// wheel movement and sensors
const int wheelAforward = 3; //used to be 10
const int wheelABackward = 13; //forget
const int wheelBBackward = 12; //forget
const int wheelBforward = 5; //used to be 11
const int wheelBSensor = 5; //forget
const int wheelASensor = 7; //forget

// echo sensor
const int distanceTrig = 8;
const int distanceEcho = 9;

//line sensors
const int lineSensors[] = {A6, A5, A4, A3, A2, A0}; 
const int lineSensorsLength = 6;

// line sensor value table
const int lineSensorValues[] = {-9, -4, -2, 2, 4, 9};

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
pinMode(wheelABackward, OUTPUT);
pinMode(wheelAforward, OUTPUT);
pinMode(wheelBBackward, OUTPUT);
pinMode(wheelBforward, OUTPUT);

//sensors
pinMode(wheelASensor, INPUT);
pinMode(wheelBSensor, INPUT);
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
currentSpeedPercent = 100;
}

// ================= LOOP ====================

void loop() {
  updateSensorBoolList();

  if(getSensorAnomaly()){
    //removeExtremeSensors();
    forwardTurn(lastTurnCoefficient);
  }
  else{
    forwardTurn(getTurnCoefficient());
  }
  //simpleforward(currentSpeedPercent);
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

// get the integer from line sensors (0 forward, - left, + right)
int getTurnCoefficient(){
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

// returns true when an irregular pattern occurs (ie. there's a line in the middle and at the sides, or there is no line)
bool getSensorAnomaly(){
  if(lineSensorBoolValues[2] || lineSensorBoolValues[3]){
    if (lineSensorBoolValues[0] || lineSensorBoolValues[5]){
      return true;
    }
  }
  for (int i = 0; i < lineSensorsLength; i++) {
    if(lineSensorBoolValues[i]){
      return false;
    }
  }
  return true;
}

void removeExtremeSensors(){
  lineSensorBoolValues[0] = 0;
  lineSensorBoolValues[5] = 0;
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
// moves the robot forward, not practical, to be used for testing
void simpleforward(int speed){
  currentSpeedPercent = speed; 
  speed = scaleFromPercent(speed);
  
  analogWrite(wheelAforward, speed);
  analogWrite(wheelBforward, speed);
}

// turns the robot by turnpercent (from -100 to 100) while still moving forwards
void forwardTurn(int turnPercent){
    int modTurnPercent = 100 - abs(turnPercent); // ie. turns -30 into 70
    if (modTurnPercent < minTurnPower){modTurnPercent = minTurnPower; Serial.println("worked");};
    int turnWheelSpeed = scaleFromPercent(currentSpeedPercent / (100 / modTurnPercent)); // divides current speed by modified turnPercent fraction (50/(100/50) = 25)
    int forWheelSpeed = scaleFromPercent(currentSpeedPercent);
    

    analogWrite(wheelABackward, 0);
    analogWrite(wheelBBackward, 0);
    
  if (turnPercent < 0){
    analogWrite(wheelAforward, turnWheelSpeed);
    analogWrite(wheelBforward, forWheelSpeed);
  }
  else if (turnPercent > 0){
    analogWrite(wheelAforward, forWheelSpeed);
    analogWrite(wheelBforward, turnWheelSpeed);
  }
  else{
    simpleforward(currentSpeedPercent);
  }
}

// grab an object using a grabber, or move the grabber to the specified degrees using the optional argument
void grab(int degrees = 0){
  gripper.write(degrees);  
}

// ================= CALCULATIONS ====================

// calculates an analogue pin value based on percentage given
int scaleFromPercent(float percent){
  return map(percent, 0, 100, 0, 255);
}
