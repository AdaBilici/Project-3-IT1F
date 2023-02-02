#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
// the setup function runs once when you press reset or power the board
//int ledRed=13;
//int ledYellow=12;
//int ledGreen=11;
//int brightness=0;

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

const int PIXEL_PIN=0;
const int PIXEL_NUMBER=4;

//colors
Adafruit_NeoPixel leds(PIXEL_NUMBER, PIXEL_PIN, NEO_RGB + NEO_KHZ800);
const uint32_t RED=leds.Color(255,0,0);
const uint32_t YELLOW=leds.Color(255,150,0);
const uint32_t BLUE=leds.Color(0,0,255);
const uint32_t START=leds.Color(0,0,0);

void moveForward(int power=255)
{
  leds.clear();
  leds.fill(BLUE,2,4);
  leds.show(); 
  analogWrite(MOTOR_A1,power);
  analogWrite(MOTOR_B1,power);
}
void setup() {
  Serial.begin(9600);
  leds.begin();
  leds.fill(START,0,4);
  leds.show();
  setup_motor_pins();
  pinMode(BUTTON1,INPUT);
}

// the loop function runs over and over again forever

void loop() {
  
  buttonState1=digitalRead(BUTTON1);
  buttonState2=digitalRead(BUTTON2);
  buttonState3=digitalRead(BUTTON3);
  if (buttonState1==LOW)
  {
  moveForward(255);
  }
  else{  
    analogWrite(MOTOR_A1,0);
    analogWrite(MOTOR_B1,0);
    }
  if (buttonState2==LOW)
  {
    analogWrite(MOTOR_A2,255);
    analogWrite(MOTOR_B2,255);
    }
  else{
    analogWrite(MOTOR_A2,0);
    analogWrite(MOTOR_B2,0);
    }

}

//Functions
void setup_motor_pins()
{
  pinMode(MOTOR_A1,OUTPUT);
  pinMode(MOTOR_A2,OUTPUT);
  pinMode(MOTOR_B1,OUTPUT);
  pinMode(MOTOR_B2,OUTPUT);
}
