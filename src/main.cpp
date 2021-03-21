/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance;
#include<Arduino.h>
#include <Servo.h>
void scan();
void p(int rawoutput[16]);
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
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  myservox.attach(9);
  myservoy.attach(10);
  myservox.write(40);
  myservoy.write(40);
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

