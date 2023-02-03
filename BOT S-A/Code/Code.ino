#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
// the setup function runs once when you press reset or power the board
//int ledRed=13;
//int ledYellow=12;
//int ledGreen=11;
//int brightness=0;
//DistanceSenzor

const int trigPin = 8;
const int echoPin = 9;

//bluetooth

const int BT_R=5;
const int BT_T=6;
SoftwareSerial BT_module(BT_R,BT_T);

//motor pins
const int  MOTOR_A1=A0;
const int MOTOR_A2=A1;  
const int MOTOR_B1=A2;
const int MOTOR_B2=A3;

//buttons pins

const int BUTTON1=2;
const int BUTTON2=3;
const int BUTTON3=4;

//button states;

int buttonState1=0;
int buttonState2=0;
int buttonState3=0;

//pixels

const int PIXEL_PIN=7;
const int PIXEL_NUMBER=4;

//colors
Adafruit_NeoPixel leds(PIXEL_NUMBER, PIXEL_PIN, NEO_RGB + NEO_KHZ800);
const uint32_t RED=leds.Color(255,0,0);
const uint32_t YELLOW=leds.Color(255,150,0);
const uint32_t BLUE=leds.Color(0,0,255);
const uint32_t WHITE=leds.Color(255,255,255);
const uint32_t START=leds.Color(0,0,0);

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
}

// the loop function runs over and over again forever

void loop() {
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
  leds.fill(START,0,4);
  leds.show();
}

//Functions
void setup_motor_pins()
{
  pinMode(MOTOR_A1,OUTPUT);
  pinMode(MOTOR_A2,OUTPUT);
  pinMode(MOTOR_B1,OUTPUT);
  pinMode(MOTOR_B2,OUTPUT);
}
