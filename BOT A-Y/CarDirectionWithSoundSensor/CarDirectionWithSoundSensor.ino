//================||PwM MOTOR PINS||=========================
int wheelAForwards = 10;
int wheelABackwards = 9;
int wheelBForwards = 6;
int wheelBBackwards = 5;
//==========================================================


//=============||DISTANCE SENSOR PINS AND VARIABLES||=====================
int trigPin = 12;    // Trigger
int echoPin = 13;    // Echo
long cm, duration;
//=========================================================================

void setup() {

  //initialize serial monitor and motor pins
  Serial.begin(9600);
  pinMode(wheelAForwards, OUTPUT);
  pinMode(wheelABackwards, OUTPUT);
  pinMode(wheelBForwards, OUTPUT);
  pinMode(wheelBBackwards, OUTPUT);

  //initialize the distance sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
     
}

//==================||DISTANCE MEASUREMENT COMMANDS||======================
void readMeasurement(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
}
void printMeasurement(){
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
}


//==================||END OF DISTANCE MEASUREMENT COMMANDS||===============



 //=================== ||CAR CONTROLL COMMANDS||=======================
void forwards(){
 
 analogWrite(wheelAForwards, 255);
 analogWrite(wheelABackwards, 0);
 analogWrite(wheelBForwards, 255);
 analogWrite(wheelBBackwards, 0);
}

void sharpLeft(){
 analogWrite(wheelAForwards, 0);
 analogWrite(wheelABackwards, 0);
 analogWrite(wheelBForwards, 255);
 analogWrite(wheelBBackwards, 0);
}

void sharpRight(){
 analogWrite(wheelAForwards, 255);
 analogWrite(wheelABackwards, 0);
 analogWrite(wheelBForwards, 0);
 analogWrite(wheelBBackwards, 0);
}

void slowLeft(){
 analogWrite(wheelAForwards, 200);
 analogWrite(wheelABackwards, 0);
 analogWrite(wheelBForwards, 255);
 analogWrite(wheelBBackwards, 0);
}

void slowRight(){
 analogWrite(wheelAForwards, 255);
 analogWrite(wheelABackwards, 0);
 analogWrite(wheelBForwards, 200);
 analogWrite(wheelBBackwards, 0);

}

void halt(){
 analogWrite(wheelAForwards, 0);
 analogWrite(wheelABackwards, 0);
 analogWrite(wheelBForwards, 0);
 analogWrite(wheelBBackwards, 0);
}

void rotate(){
 analogWrite(wheelAForwards, 255);
 analogWrite(wheelABackwards, 0);
 analogWrite(wheelBForwards, 0);
 analogWrite(wheelBBackwards, 255);
}

//===================||END OF CAR CONTROLL COMMANDS||===================

void loop() {

readMeasurement();
printMeasurement();

if(cm < 30) {
  rotate();
  delay(500);
}
delay(100);
 
}
