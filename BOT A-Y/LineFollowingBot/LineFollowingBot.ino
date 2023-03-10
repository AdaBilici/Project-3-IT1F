#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// ================= PINS ====================
// wheel movement and sensors
const int wheelAforward = 3; //used to be 10
const int wheelABackward = 0;
const int wheelBBackward = 0;
const int wheelBforward = 5; //used to be 11
const int PIXEL_PIN=2;
const int PIXEL_NUMBER=4;

// led pixels
Adafruit_NeoPixel leds(PIXEL_NUMBER, PIXEL_PIN, NEO_RGB + NEO_KHZ800);
const uint32_t RED=leds.Color(255,0,0);
const uint32_t YELLOW=leds.Color(255,150,0);
const uint32_t BLUE=leds.Color(0,0,255);
const uint32_t WHITE=leds.Color(255,255,255);
const uint32_t START=leds.Color(0,0,0);


// echo sensor
const int distanceTrig = 8;
const int distanceEcho = 9;
const float objectDetectionDistance = 20;
const int maxObjectDetections = 50;

//line sensors
const int lineSensors[] = {A6, A5, A4, A3, A2, A0}; 
const int lineSensorsLength = 6;

// line sensor value table
const int lineSensorValues[] = {-9, -6, -2, 2, 6, 9};

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
int blackZoneFrameCount;
int objectDetectionCount;
int maxObjectDetectionCount;
boolean startDetectionToggle;
int startDetectionCount; // counts lines at start sequence;
int objectDetectionBuffer; // detects objects every x frames
int objectDetectionBufferRate;
int phase;

// ================= SETUP ====================

void setup() {
// wheel movement
pinMode(wheelABackward, OUTPUT);
pinMode(wheelAforward, OUTPUT);
pinMode(wheelBBackward, OUTPUT);
pinMode(wheelBforward, OUTPUT);

//sensors
pinMode(distanceTrig, OUTPUT);
pinMode(distanceEcho, INPUT);

// assign line sensor input type (pinMode(A0-A7, INPUT))
for (int i = 0; i < lineSensorsLength; i++){
  pinMode(lineSensors[i], INPUT);
}

//Led
pinMode(Lled, OUTPUT);
leds.begin();
leds.fill(RED,0,2);
leds.fill(WHITE,2,0);
leds.setBrightness(60);
leds.show();

//gripper
gripper.attach(gripperPin);


//serial
Serial.begin(9600);

//buffer/timing/framerate related
objectDetectionBufferRate = 400;
maxObjectDetectionCount = 10;

// initial variables
startDetectionToggle = false;
currentSpeedPercent = 100;
phase = 0;
}

// ================= LOOP ====================

// main loop
void loop() {
  // if isn't in resting phase
  if(objectDetectionBuffer >= objectDetectionBufferRate){
    Serial.println(objectDetectionCount);
    objectDetectionBuffer = 0;
    updateObjectDetectionCount();
  }
  else{
    objectDetectionBuffer++;
  }
  updateSensorBoolList();
  updateBlackZoneFrameCount();
  
  // while in start zone
  if (phase == 0){
    startSequence();
  }
  
  // while in actual course
  else if (phase == 1){
    lineFollowSequence();
  }
  else if (phase == 2){
    endSequence();
  }
  else if (phase == 3){
    avoidObjectSequence();
  }
}

// moves into the block and turns left
void startSequence(){
  grab(1);
  forwardTurn(0);
  Serial.println(startDetectionCount);
  if (isBlackZoneFrame()){
    if(startDetectionToggle){
      startDetectionToggle=false;
      startDetectionCount++; 
    }
  }
  else{
    startDetectionToggle = true;
  }
  if (startDetectionCount >= 4){
    grab(0);
    delay(200);
    simpleLeft();
    forwardTurn(0);
    delay(300);
    phase = 1;
  }
}

void lineFollowSequence(){
  if(getSensorAnomaly()){
    forwardTurn(lastTurnCoefficient);
  }
  else{
    forwardTurn(getTurnCoefficient());
  }
  if (blackZoneFrameCount > 100){
    // end of course sequence
    phase=2;
  }
}

void avoidObjectSequence(){
  phase = 1;
}

void endSequence(){
  Serial.println("end");
  movementStop();
  while(true){
    delay(99999999);
  }
}

// ================= SENSORS ====================
// reads distance in centimeters from the echo sensor
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

void updateObjectDetectionCount(){
  if(getDistance() < objectDetectionDistance){
    objectDetectionCount++;
  }
  else{
    if(objectDetectionCount <= 0){
      objectDetectionCount = 0;
    }
    else{
      objectDetectionCount -= 2;
    }
  }
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

void updateBlackZoneFrameCount(){
  if(isBlackZoneFrame()){
    blackZoneFrameCount++;
  }
  else{
    blackZoneFrameCount = 0;
  }
}

bool isBlackZoneFrame(){
  for (int i = 1; i < lineSensorsLength-1; i++){ //ignores first and last sensor
    if(!lineSensorBoolValues[i]){
      return false;
    }
  }
  return true;
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
  analogWrite(wheelABackward, 0);
  analogWrite(wheelBBackward, 0);
  currentSpeedPercent = speed; 
  speed = scaleFromPercent(speed);
  
  analogWrite(wheelAforward, speed);
  analogWrite(wheelBforward, speed);
}

// does 90 degree turn left
void simpleLeft(){
  analogWrite(wheelABackward, 0);
  analogWrite(wheelBBackward, 0);
  analogWrite(wheelAforward, 0);
  analogWrite(wheelBforward, 255);
  delay(800);
}

void complexLeft(int delayA){ // does a turn to the left
  analogWrite(wheelABackward, 0);
  analogWrite(wheelBBackward, 0);
  analogWrite(wheelAforward, 0);
  analogWrite(wheelBforward, 255);
  delay(delayA);
}

// does 90 degree turn right
void simpleRight(){
  analogWrite(wheelABackward, 0);
  analogWrite(wheelBBackward, 0);
  analogWrite(wheelAforward, 255);
  analogWrite(wheelBforward, 0);
  delay(800);
}

// turns the robot by turnpercent (from -100 to 100) while still moving forwards
void forwardTurn(int turnPercent){
    int modTurnPercent = 100 - abs(turnPercent); // ie. turns -30 into 70
    if (modTurnPercent < minTurnPower){modTurnPercent = minTurnPower;};
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

void movementStop(){
  analogWrite(wheelAforward, 0);
  analogWrite(wheelBforward, 0);
  analogWrite(wheelABackward, 0);
  analogWrite(wheelBBackward, 0);
}

// grab an object using a grabber, or move the grabber to the specified degrees using the optional argument
void grab(int degrees){
  degrees = map(degrees, 0, 1, 40, 120);
  gripper.write(degrees);  
}

// ================= CALCULATIONS ====================

// calculates an analogue pin value based on percentage given
int scaleFromPercent(float percent){
  return map(percent, 0, 100, 0, 255);
}
