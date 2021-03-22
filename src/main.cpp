/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// defines variables
long duration; // variable for the duration of sound wave travel
int distance;
#include<Arduino.h>
#include <Servo.h>
void scan();
int angleToPulse(int ang);
void p(int rawoutput[16]);
void compute(int rawoutput[16]);
void project(int rawoutput[16]);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  155 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  555 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;

Servo myservox;
Servo myservoy; // create servo object to control a servo
// twelve servo objects can be created on most boards

int posx = 40;    // variable to store the servo position
int posy = 40;
int stepx = 5;
int stepy = 5;
int t =30;
int row = 0;
int rawoutput[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int computed_matrix[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  myservox.attach(9);
  myservoy.attach(10);
  myservox.write(40);
  myservoy.write(40);
  pwm.begin();
  pwm.setPWMFreq(60);
}

void loop() {
  bool oddeven=true;
  for (posy = 40; posy < 60;posy+=stepy)
  {
    myservoy.write(posy);
    delay(t);
        scan();
   if(oddeven){
    oddeven=false;
    for (posx = 45; posx < 60;posx+=stepx)
    {
      myservox.write(posx);
      
      delay(t);
      scan();
    }}else
    {
      oddeven=true;
      for (posx = 50; posx > 35;posx-=stepx)
    {
      myservox.write(posx);
      delay(t);
      scan();

    }
    }
  }
  p(rawoutput);
  project(rawoutput);
  row = 0;
}
void p(int rawoutput[16])
{
  for (int ite = 1; ite <= 16;ite++)
  {
    Serial.print("  ");
    Serial.print(rawoutput[ite-1]);
    if(ite%4==0){
    Serial.println();}
  }
  Serial.println("____________________________________");
  Serial.println();
}
 void scan()
 {
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   // Reads the echoPin, returns the sound wave travel time in microseconds
   duration = pulseIn(echoPin, HIGH);
   // Calculating the distance
   distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
   rawoutput[row++] = distance;
 }

int angleToPulse(int ang){
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
   Serial.print("Angle: ");Serial.print(ang);
   Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}
void project(int computed_matrix[16]){
  for (int servo_and_mat = 0; servo_and_mat < 16;servo_and_mat++)
  {
    delay(500);
      pwm.setPWM(servo_and_mat, 0, angleToPulse(computed_matrix[servo_and_mat]));
  }
}
void compute(int rawoutput[16])
{
  //calculating max and min
  int max=0;
  int min=99999;
  for (int ite = 0; ite < 16;ite++)
  {
    if(max<rawoutput[ite])
    {
      max = rawoutput[ite];
    }
    if(min>rawoutput[ite])
    {
      min = rawoutput[ite];
    }
  }
  for (int i = 0; i < 16;i++)
  {
    int var = map(rawoutput[i],0,500, min, max);
    computed_matrix[i] = var;
  }
  Serial.println("Computed Matrix");
  p(computed_matrix);
}
