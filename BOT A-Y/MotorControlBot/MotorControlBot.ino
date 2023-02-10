
//================||PWM MOTOR PINS||=========================
int wheelAForwards = 10;
int wheelABackwards = 9;
int wheelBForwards = 6;
int wheelBBackwards = 5;
//==========================================================



void setup() {

  //initialize serial monitor and output pins
  Serial.begin(9600);
  pinMode(wheelAForwards, OUTPUT);
  pinMode(wheelABackwards, OUTPUT);
  pinMode(wheelBForwards, OUTPUT);
  pinMode(wheelBBackwards, OUTPUT);
     
}

void loop() {

 //Forwards for 1 second
 analogWrite(WheelAForwards, 255);
 analogWrite(WheelABackwards, 0);
 analogWrite(WheelBForwards, 255);
 analogWrite(WheelBBackwards, 0);
 delay(1000);
 
 //Sharp Left for 1 second
 analogWrite(WheelAForwards, 0);
 analogWrite(WheelABackwards, 0);
 analogWrite(WheelBForwards, 255);
 analogWrite(WheelBBackwards, 0);
 delay(1000);
 
 //Sharp Right for 1 second
 analogWrite(WheelAForwards, 255);
 analogWrite(WheelABackwards, 0);
 analogWrite(WheelBForwards, 0);
 analogWrite(WheelBBackwards, 0);
 delay(1000);
 
 
 //Slow Left for 1 second
 analogWrite(WheelAForwards, 200);
 analogWrite(WheelABackwards, 0);
 analogWrite(WheelBForwards, 255);
 analogWrite(WheelBBackwards, 0);
 delay(1000);
 
 //Slow Right for 1 second
 analogWrite(WheelAForwards, 255);
 analogWrite(WheelABackwards, 0);
 analogWrite(WheelBForwards, 200);
 analogWrite(WheelBBackwards, 0);
 delay(1000);

 
 //Stop the car for 1 second
 analogWrite(WheelAForwards, 0);
 analogWrite(WheelABackwards, 0);
 analogWrite(WheelBForwards, 0);
 analogWrite(WheelBBackwards, 0);
 delay(1000);

 
 //ROTATE  for 1 second
 analogWrite(WheelAForwards, 255);
 analogWrite(WheelABackwards, 0);
 analogWrite(WheelBForwards, 0);
 analogWrite(WheelBBackwards, 255);
 delay(1000);
 
}
