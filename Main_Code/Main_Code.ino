#include <Adafruit_NeoPixel.h>
#include <MSE2202_Lib.h>

//Pin Allocations

#define MOTOR_1A 7
#define MOTOR_1B 6
#define MOTOR_2A 4
#define MOTOR_2B 5
#define MOTOR_3A 36
#define MOTOR_3B 35
#define MOTOR_4A 38
#define MOTOR_4B 37

#define MOTOR_5A 10
#define MOTOR_5B 9
#define MOTOR_6A 11
#define MOTOR_6B 12

#define SERVO_L1 42
#define SERVO_L2 41
#define SERVO_R 40

#define TRIGGER_FRONT 13
#define ECHO_FRONT 14
#define TRIGGER_REAR 17
#define ECHO_REAR 18

#define LED 15

#define BUTTON 0

//Constants

const int Lift1_Raised = 1900;
const int Lift2_Raised = 800;
const int Lift1_Lowered = 1000;
const int Lift2_Lowered = 1600;
const int Rack_Centre = 1;
const int Rack_Side = 1;

const int drive_speed = 1;
const int reverse_distance = 10000;

//Variables

unsigned int drive_index;

long ping_jump;
long jump_size = 1000;

unsigned long timer;
boolean timeup = false;

//Objects

Motion Bot = Motion();

void setup() 
{
  Serial.begin(9600);

  Bot.servoBegin("S1",SERVO_L1);
  Bot.servoBegin("S2",SERVO_L2);
  Bot.servoBegin("S3",SERVO_R);

  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(MOTOR_1A, OUTPUT);
  pinMode(MOTOR_1B, OUTPUT);
  pinMode(MOTOR_2A, OUTPUT);
  pinMode(MOTOR_2B, OUTPUT);
  pinMode(MOTOR_3A, OUTPUT);
  pinMode(MOTOR_3B, OUTPUT);
  pinMode(MOTOR_4A, OUTPUT);
  pinMode(MOTOR_4B, OUTPUT);
  pinMode(MOTOR_5A, OUTPUT);
  pinMode(MOTOR_5B, OUTPUT);
  pinMode(MOTOR_6A, OUTPUT);
  pinMode(MOTOR_6B, OUTPUT);

  pinMode(LED, OUTPUT);

  pinMode(TRIGGER_REAR, OUTPUT);
  pinMode(ECHO_REAR, INPUT);

  drive_index = 0;

  digitalWrite(MOTOR_1A, LOW);
}

void loop() 
{
  timer = timer + 1;
  if(timer > 1000)
  {
    timer = 0;
    timeup = true;
  }

  switch(drive_index)
  {
    case 0:
    {
      if(digitalRead(BUTTON) == LOW)
      {
        drive_index = 1;
        ping_jump = getSonar(TRIGGER_REAR, ECHO_REAR);
      }
      break;
    }
    case 1: //Drive to ledge
    {
      digitalWrite(MOTOR_1A, HIGH);
      digitalWrite(MOTOR_2A, HIGH);
      digitalWrite(MOTOR_3A, HIGH);
      digitalWrite(MOTOR_4A, HIGH);
      Serial.println(getSonar(TRIGGER_REAR, ECHO_REAR) - ping_jump);

      if(timeup = true)
      {
        timeup = false;
        if((getSonar(TRIGGER_REAR, ECHO_REAR) - ping_jump) >= jump_size)
        {
          drive_index = 3;
          digitalWrite(MOTOR_1A, LOW);
          digitalWrite(MOTOR_2A, LOW);
          digitalWrite(MOTOR_3A, LOW);
          digitalWrite(MOTOR_4A, LOW);
        }
        ping_jump = getSonar(TRIGGER_REAR, ECHO_REAR);
      }
      break;
    }
    case 2: //Centre pulley over rope
    {
      Bot.ToPosition("S1", Lift1_Raised);
      Bot.ToPosition("S2", Lift2_Raised);

      if(timeup = true)
      {
        drive_index = 2;
        timeup = false;
        ping_jump = getSonar(TRIGGER_REAR, ECHO_REAR);        
        
        Bot.ToPosition("S3", Rack_Centre);
      }
      break;
    }
    case 3: //Drive along rope
    {
      //Bot.ToPosition("S1", Lift1_Lowered);
      //Bot.ToPosition("S2", Lift2_Lowered);
        digitalWrite(MOTOR_5A, HIGH);
        digitalWrite(MOTOR_6A, HIGH);
      /*if(timeup = true)
      {
        timeup = false;
        if(getSonar(TRIGGER_REAR, ECHO_REAR) - ping_jump <= -1*jump_size)
        {
          drive_index = 4;
        }
        ping_jump = getSonar(TRIGGER_REAR, ECHO_REAR);
      }*/
      break;
    }
    case 4: //Land on other table
    {
      Bot.ToPosition("S1", Lift1_Raised);
      Bot.ToPosition("S2", Lift2_Raised);

      digitalWrite(MOTOR_1A, HIGH);
      digitalWrite(MOTOR_2A, HIGH);
      
      if(timeup = true)
      {
        drive_index = 5;
        timeup = false;       
        
        Bot.ToPosition("S3", Rack_Side);
        digitalWrite(MOTOR_5A, LOW);
        digitalWrite(MOTOR_6A, LOW);
        ping_jump = getSonar(TRIGGER_FRONT, ECHO_FRONT);
      }
      break;
    }
    case 5: //Drive to ledge
    {
      digitalWrite(MOTOR_1A, HIGH);
      digitalWrite(MOTOR_2A, HIGH);
      digitalWrite(MOTOR_3A, HIGH);
      digitalWrite(MOTOR_4A, HIGH);

      if(timeup = true)
      {
        timeup = false;
        if(getSonar(TRIGGER_REAR, ECHO_REAR) - ping_jump >= jump_size)
        {
          drive_index = 6;
          digitalWrite(MOTOR_1A, LOW);
          digitalWrite(MOTOR_2A, LOW);
          digitalWrite(MOTOR_3A, LOW);
          digitalWrite(MOTOR_4A, LOW);
        }
        ping_jump = getSonar(TRIGGER_REAR, ECHO_REAR);
      }
      break;
    }
    case 6:
    {
      digitalWrite(MOTOR_1B, HIGH);
      digitalWrite(MOTOR_2B, HIGH);
      digitalWrite(MOTOR_3B, HIGH);
      digitalWrite(MOTOR_4B, HIGH);

      delay(reverse_distance / drive_speed);

      digitalWrite(MOTOR_1B, LOW);
      digitalWrite(MOTOR_2B, LOW);
      digitalWrite(MOTOR_3B, LOW);
      digitalWrite(MOTOR_4B, LOW);

      digitalWrite(LED, HIGH);

      delay(1000);

      drive_index = 0;
    }    
  }
}

long getSonar(int trigger, int echo) 
{
  unsigned long pingTime;
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  pingTime = pulseIn(echo, HIGH, 42000);
  return pingTime;
}