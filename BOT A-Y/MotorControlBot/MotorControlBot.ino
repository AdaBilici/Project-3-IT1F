
//================||PWM MOTOR PINS||=========================
int LeftWheelForwards = 10;
int LeftWheelBackwards = 9;
int RightWheelForwards = 6;
int RightWheelBackwards = 5;
//==========================================================



void setup() {

  //initialize serial monitor and output pins
  Serial.begin(9600);
  pinMode(LeftWheelForwards, OUTPUT);
  pinMode(LeftWheelBackwards, OUTPUT);
  pinMode(RightWheelForwards, OUTPUT);
  pinMode(RightWheelBackwards, OUTPUT);
     
}

void forwards()
{
 analogWrite(LeftWheelForwards, 255);
 analogWrite(LeftWheelBackwards, 0);
 analogWrite(RightWheelForwards, 255);
 analogWrite(RightWheelBackwards, 0);
}

void backwards()
{
 analogWrite(LeftWheelForwards, 0);
 analogWrite(LeftWheelBackwards, 255);
 analogWrite(RightWheelForwards, 0);
 analogWrite(RightWheelBackwards, 255);
}

void sharpLeft()
{
 analogWrite(LeftWheelForwards, 0);
 analogWrite(LeftWheelBackwards, 0);
 analogWrite(RightWheelForwards, 255);
 analogWrite(RightWheelBackwards, 0);
}

void sharpRight()
{
 analogWrite(LeftWheelForwards, 255);
 analogWrite(LeftWheelBackwards, 0);
 analogWrite(RightWheelForwards, 0);
 analogWrite(RightWheelBackwards, 0);
}

void slowLeft()
{
 analogWrite(LeftWheelForwards, 200);
 analogWrite(LeftWheelBackwards, 0);
 analogWrite(RightWheelForwards, 255);
 analogWrite(RightWheelBackwards, 0);
}

void slowRight()
{
 analogWrite(LeftWheelForwards, 255);
 analogWrite(LeftWheelBackwards, 0);
 analogWrite(RightWheelForwards, 200);
 analogWrite(RightWheelBackwards, 0);
}

void stopCar()
{
 analogWrite(LeftWheelForwards, 0);
 analogWrite(LeftWheelBackwards, 0);
 analogWrite(RightWheelForwards, 0);
 analogWrite(RightWheelBackwards, 0);
}

void rotate()
{
 analogWrite(LeftWheelForwards, 255);
 analogWrite(LeftWheelBackwards, 0);
 analogWrite(RightWheelForwards, 0);
 analogWrite(RightWheelBackwards, 255);
}

void loop() {

 //Forwards for 1 second
 forwards();
 delay(1000);

 //Backwards for 1 second
 backwards();
 delay(1000);
 
//Sharp Left for 1 second
 sharpLeft();
 delay(1000);
 
//Sharp Right for 1 second
 sharpRight();
 delay(1000);
 
 
//Slow Left for 1 second
 slowLeft();
 delay(1000);
 
//Slow Right for 1 second
 slowRight();
 delay(1000);

 
//Stop the car for 1 second
 stopCar();
 delay(1000);


//ROTATE  for 1 second
 rotate();
 delay(1000);
 
}
