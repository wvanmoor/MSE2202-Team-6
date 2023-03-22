#include <ESP32Servo.h>

#define LIFT
#define BALANCE
#define PULLEY_ARM
#define WHEELS
#define PULLEYS
#define TRIGGER
#define ECHO
#define CONTACT

int lift_lower = 0;
int lift_raise = 0;
int pulley_side = 0;
int pulley_centre = 0;

int sonar;
int sonar_jump = 1000;

int drive_mode = 0;
int reverse_time = 1000;

Servo lift;
Servo balance;
Servo pulley_arm;

void setup() 
{
  //Setup Servos
  lift.setPeriodHertz(50);
  balance.setPeriodHertz(50);
  pulley_arm.setPeriodHertz(50);

  lift.attach(LIFT, 500, 2500);
  balance.attach(BALANCE, 500, 2500);
  pulley_arm.attach(PULLEY_ARM, 500, 2500)

  //Setup USM
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

  sonar = getSonar();
}
void loop() 
{
  switch (drive_mode) {
    case 0:
    {
      lift.write(lift_lower);
      pulley_arm.write(pulley_side);
      digitalWrite(WHEELS, HIGH);

      if(abs(sonar - getSonar()) >= sonar_jump)
      {
        digitalWrite(WHEELS, LOW);
        drive_mode = 1;
      }
    }
    case 1:
    {
      write.lift(lift_raise);

      delay(100);

      write.pulley_arm(pulley_centre);

      delay(100);

      write.lift(lift_lower);
      drive_mode = 2;
    }
    case 2:
    {
      digitalWrite(PULLEYS, HIGH);

      delay(500);

      for (posVal = 0; digitalRead(CONTACT) == HIGH; posVal += 1) 
      {
        // in steps of 1 degree
        lift.write(posVal);       // tell servo to go to position in variable 'pos'
        delay(1);                   // waits 15ms for the servo to reach the position
      }
      digitalWrite(WHEELS, HIGH);

      delay(500);

      digitalWrite(WHEELS,LOW);
      digitalWrite(PULLEYS, LOW);
      write.lift(lift_raise);
      
      delay(50);

      write.pulley_arm(pulley_side);

      delay(50);

      write.lift(lift_lower);

      drive_mode 3;
    }
    case 3:
    {
      digitalWrite(WHEELS, HIGH);

      if(digitalRead(CONTACT) == LOW)
      {
        digitalWrite(WHEELS, LOW);
        drive_mode 4;
      }
    }
    case 4:
    {
      digitalWrite(WHEELS, REVERSE);
      
      delay(reverse_time);
    }
  
  }
}

float getSonar() {
  unsigned long pingTime;
  float distance;
  // make trigPin output high level lasting for 10μs to triger HC_SR04
  digitalWrite(TRIGGER, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  // Wait HC-SR04 returning to the high level and measure out this waitting time
  pingTime = pulseIn(ECHO, HIGH, 420000); 
  return pingTime; // return the distance value
}