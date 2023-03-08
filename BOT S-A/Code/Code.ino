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

const int trigPinFront = 8;
const int echoPinFront = 9;

const int trigPinLeft = 2;
const int echoPinLeft = 3;

//===[ Gripper ]====================================

const int gripper_pin=4;
Servo gripper;
//bluetooth

long duration;
int distance;
const int BT_R=5;
const int BT_T=6;
SoftwareSerial BT_module(BT_R,BT_T);

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
   leds.clear();
   leds.fill(WHITE,2,2);
   leds.show();
  analogWrite(MOTOR_A1,power);
  analogWrite(MOTOR_B1,power);
}

void moveBackwards(int power=255)
{
   leds.clear();
   leds.fill(RED,0,2);
   leds.show();
  analogWrite(MOTOR_A2,power);
  analogWrite(MOTOR_B2,power);
}

//===[SETUP ]============================

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  BT_module.begin(9600);
  leds.begin();
  leds.fill(BLUE,0,4);
  leds.show();
  setup_motor_pins();
  pinMode(BUTTON1,INPUT);
  pinMode(BUTTON2,INPUT);
//  gripper.attach(gripper_pin);
}

int value=0;

//===[ LOOP ]============================

void loop() {
  //distance senzor
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);  
  
  buttonState1=digitalRead(BUTTON1);
  buttonState2=digitalRead(BUTTON2);

  if (buttonState1==LOW)
  {
  moveForward(255);
  }
  else
  {
    analogWrite(MOTOR_A1,0);
    analogWrite(MOTOR_B1,0);
    }


  if (buttonState2==LOW)
  {
    moveBackwards(255);
    }
  else{
    analogWrite(MOTOR_A2,0);
    analogWrite(MOTOR_B2,0);
    }
    
/*
 if(buttonState1==LOW)
 {value++;
 gripper.write(160);
 delay(5000);
 }
 
 if(value>0)
 value --;
 
 gripper.write(0);
 */
 
 leds.fill(START,0,4);
 leds.show();
}
