#include <Adafruit_NeoPixel.h>
#include <MSE2202_Lib.h>

//Pin Allocations

#define MOTOR_1A
#define MOTOR_1B
#define MOTOR_2A
#define MOTOR_2B
#define MOTOR_3A
#define MOTOR_3B
#define MOTOR_4A
#define MOTOR_4B

#define MOTOR_5A
#define MOTOR_5B
#define MOTOR_6A
#define MOTOR_6B

#define SERVO_L1
#define SERVO_L2
#define SERVO_R

#define TRIGGER_FRONT
#define ECHO_FRONT
#define TRIGGER_REAR
#define ECHO_REAR

#define LED

#define BUTTON

//Constants

const int Lift1_Raised = 0;
const int Lift2_Raised = 0;
const int Lift1_Lowered = 0;
const int Lift2_Lowered = 0;
const int Rack_Centre = 0;
const int Rack_Side = 0;

const int drive_speed = 0;
const int reverse_distance

//Variables

unsigned int drive_index;

unsigned long ping_jump;
unsigned long jump_size;

unsigned long timer;
boolean timeup = false;

//Objects

Motion Bot = Motion();

void setup() 
{
  Bot.servoBegin("S1",SERVO_L1);
  Bot.servoBegin("S2",SERVO_L2);
  Bot.servoBegin("S3",SERVO_L3);

  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(LED, OUTPUT);

  pinMode(TRIGGER_FRONT, OUTPUT);
  pinMode(TRIGGER_REAR, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(ECHO_REAR, INPUT);

  drive_index = 0;
}

void loop() 
{
  timer = timer + 1;
  if(timer > 100)
  {
    timer = 0;
    timeup = true;
  }

  switch(drive_index)
  {
    case 0:
    {
      if(digitalRead(BUTTON) == HIGH)
      {
        drive_index = 1;
        ping_jump = getSonar(TRIGGER_REAR, ECHO_REAR);
      }
      break;
    }
    case 1: //Drive to ledge
    {
      digitalWrite();
      digitalWrite();

      if(timeup = true)
      {
        timeup = false;
        if(abs(ping_jump - getSonar(TRIGGER_REAR, ECHO_REAR)) >= jump_size)
        {
          drive_index = 2;
          digitalWrite();
          digitalWrite();
        }
        ping_jump = getSonar(TRIGGER_REAR, ECHO_REAR);
      }
      break;
    }
    case 2: //Centre pulley over rope
    {
      Bot.ToPosition(SERVO_L1, Lift1_Raised);
      Bot.ToPosition(SERVO_L2, Lift2_Raised);

      if(timeup = true)
      {
        drive_index = 3;
        timeup = false;
        ping_jump = getSonar(TRIGGER_FRONT, ECHO_FRONT);        
        
        Bot.ToPosition(SERVO_R, Rack_Centre);
      }
      break;
    }
    case 3: //Drive along rope
    {
      Bot.ToPosition(SERVO_L1, Lift1_Lowered);
      Bot.ToPosition(SERVO_L2, Lift2_Lowered);

      digitalWrite();
      digitalWrite();

      if(timeup = true)
      {
        timeup = false;
        if(abs(ping_jump - getSonar(TRIGGER_FRONT, ECHO_FRONT)) >= jump_size)
        {
          drive_index = 4;
        }
        ping_jump = getSonar(TRIGGER_FRONT, ECHO_FRONT);
      }
      break;
    }
    case 4: //Land on other table
    {
      Bot.ToPosition(SERVO_L1, Lift1_Raised);
      Bot.ToPosition(SERVO_L2, Lift2_Raised);

      digitalWrite();
      digitalWrite();
      
      if(timeup = true)
      {
        drive_index = 5;
        timeup = false;       
        
        Bot.ToPosition(SERVO_R, Rack_SIDE);
        digitalWrite();
        digitalWrite();
        ping_jump = getSonar(TRIGGER_FRONT, ECHO_FRONT);
      }
      break;
    }
    case 5: //Drive to ledge
    {
      if(timeup = true)
      {
        timeup = false;
        if(abs(ping_jump - getSonar(TRIGGER_FRONT, ECHO_FRONT)) >= jump_size)
        {
          drive_index = 6;
          digitalWrite();
          digitalWrite();
        }
        ping_jump = getSonar(TRIGGER_FRONT, ECHO_FRONT);
      }
      break;
    }
    case 6:
    {
      digitalWrite();
      digitalWrite();

      delay(reverse_distance / drive_speed);

      digitalWrite();
      digitalWrite();

      digitalWrite(LED, HIGH);

      delay(1000);

      drive_index = 0;
    }    
  }
}

unsigned long getSonar(int trigger, int echo) 
{
  unsigned long pingTime;
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  pingTime = pulseIn(echo, HIGH, 42000);
  return pingTime;
}