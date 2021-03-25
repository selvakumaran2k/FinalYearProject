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
#include<Arduino.h>
#include <Servo.h>
//functions
void scan();
int angleToPulse(int ang);
void p(int rawoutput[16]);
void compute(int rawoutput[16]);
void project(int rawoutput[16]);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  155 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  555 // this is the 'maximum' pulse length count (out of 4096)
long duration; // variable for the duration of sound wave travel
int distance;
// our servo # counter
uint8_t servonum = 0;
Servo myservox;
Servo myservoy; // create servo object to control a servo
// twelve servo objects can be created on most boards
//variables
int posx = 40;    // variable to store the servo position
int posy = 40;
int stepx = 5;
int stepy = 5;
int t =10;
int row = 0;
int button = 7;
int scanStatusLED = A0;
int rawoutput[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int computed_matrix[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(button, INPUT_PULLUP);
  pinMode(scanStatusLED, OUTPUT);
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  myservox.attach(9);
  myservoy.attach(10);
  myservox.write(40);
  myservoy.write(40);
  pwm.begin();
  pwm.setPWMFreq(60);
}

void loop() {
  if(digitalRead(button)){
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
  compute(rawoutput);
  project(computed_matrix);
  row = 0;
  }
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
   analogWrite(scanStatusLED, 512);
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
   if(distance>800)
   {
     distance = 800;
   }
   rawoutput[row++] = distance;
  analogWrite(scanStatusLED, 0);
 }

int angleToPulse(int ang){
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
  //  Serial.print("Angle: ");Serial.print(ang);
  //  Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}
void project(int computed_matrix[16]){
  for (int servo_and_mat = 0; servo_and_mat < 16;servo_and_mat++)
  {
      pwm.setPWM(servo_and_mat, 0, angleToPulse(computed_matrix[servo_and_mat]));
      delay(50);//servo mechanical delay
  }
}
void compute(int rawoutput[16])
{
  //raw mapping
  for (int i = 0; i < 16;i++)
  {
    int var = map(rawoutput[i],0,800, 10, 170);
    computed_matrix[i] = var;
  }
  Serial.println("Computed Matrix");
  p(computed_matrix);
}
