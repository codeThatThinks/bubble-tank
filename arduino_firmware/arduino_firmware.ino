#include <Servo.h>

const int max_speed = 255;
const int max_turning = 255;
const int servo_increment = 3;
const int lmotorf = 3;
const int lmotorb = 5;
const int rmotorf = 6;
const int rmotorb = 11;

int forwardJoystick = 0;
bool forwardDirection = true;   // true = forward, false = backward
int turningJoystick = 0;
bool turningDirection = true;   // true = left, false = right

Servo baseServo;
int baseAngle = 60;
const int minBaseAngle = 0;
const int maxBaseAngle = 120;

Servo tiltServo;
int tiltAngle = 65;
const int minTiltAngle = 65;
const int maxTiltAngle = 110;

Servo triggerServo;
int triggerAngle = 90;
const int minTriggerAngle = 90;
const int maxTriggerAngle = 142;

bool shootBubbles = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  pinMode(lmotorf,OUTPUT);
  pinMode(lmotorb,OUTPUT);
  pinMode(rmotorf,OUTPUT);
  pinMode(rmotorb,OUTPUT);
  
  baseServo.attach(2);
  baseServo.write(baseAngle);
  tiltServo.attach(4);
  tiltServo.write(tiltAngle);
  triggerServo.attach(7);
  triggerServo.write(triggerAngle);

  // prime bubble gun
  triggerServo.write((maxTriggerAngle - minTriggerAngle) / 2);
  delay(250);
  triggerServo.write(maxTriggerAngle);
  delay(4000);
  triggerServo.write((maxTriggerAngle - minTriggerAngle) / 2);
  delay(250);
  triggerServo.write(minTriggerAngle);
  
}

void setTrigger()
{
  if(shootBubbles)
  {
    triggerAngle = maxTriggerAngle;
  }
  else
  {
    triggerAngle = minTriggerAngle;
  }
}

void setServoAngles()
{
  baseServo.write(baseAngle);
  tiltServo.write(tiltAngle);
  triggerServo.write(triggerAngle);
}

void setMotorSpeeds()
{
  if(turningJoystick == 0)
  {
    if(forwardJoystick == 0)
    {
      motorrun(1,1,0);
      motorrun(1,-1,0);
      motorrun(2,1,0);
      motorrun(2,-1,0);
    }
    else
    {
      if(forwardDirection)
      {
        motorrun(1,-1,0);
        motorrun(1,1,map(forwardJoystick, 0, 255, 0, max_speed));
        
        motorrun(2,-1,0);
        motorrun(2,1,map(forwardJoystick, 0, 255, 0, max_speed));
      }
      else
      {
        motorrun(1,1,0);
        motorrun(1,-1,map(forwardJoystick, 0, 255, 0, max_speed));
        
        motorrun(2,1,0);
        motorrun(2,-1,map(forwardJoystick, 0, 255, 0, max_speed));
      }
    }
  }
  else
  {
    if(turningDirection && forwardDirection)
    {
      motorrun(1,-1,0);
      motorrun(1,1,map(forwardJoystick / (max_turning - map(turningJoystick, 0, 255, 0, max_turning)), 0, 255, 0, max_speed));
        
      motorrun(2,-1,0);
      motorrun(2,1,map(forwardJoystick, 0, 255, 0, max_speed));
    }
    else
    {
      motorrun(1,1,0);
      motorrun(1,-1,map(forwardJoystick, 0, 255, 0, max_speed));
        
      motorrun(2,1,0);
      motorrun(2,-1,map(forwardJoystick / (max_turning - map(turningJoystick, 0, 255, 0, max_turning)), 0, 255, 0, max_speed));
    }
  }
}

void readJoysticks()
{
  String command, command_char;
  
  while(Serial.available() > 0)
  {
    command = Serial.readStringUntil(';');
    command.trim();
    command_char = command.substring(0,1);
    
    if(command_char == "F")
    {
      forwardJoystick = command.substring(1, command.length()).toInt();
      forwardDirection = true;
    }
    else if(command_char == "V")
    {
      forwardJoystick = command.substring(1, command.length()).toInt();
      forwardDirection = false;
    }
    else if(command_char == "L")
    {
      turningJoystick = command.substring(1, command.length()).toInt();
      turningDirection = true;
    }
    else if(command_char == "R")
    {
      turningJoystick = command.substring(1, command.length()).toInt();
      turningDirection = false;
    }
    else if(command_char == "U")
    {
      tiltAngle = constrain(tiltAngle + servo_increment, minTiltAngle, maxTiltAngle);
    }
    else if(command_char == "D")
    {
      tiltAngle = constrain(tiltAngle - servo_increment, minTiltAngle, maxTiltAngle);
    }
    else if(command_char == "E")
    {
      baseAngle = constrain(baseAngle - servo_increment, minBaseAngle, maxBaseAngle);
    }
    else if(command_char == "I")
    {
      baseAngle = constrain(baseAngle + servo_increment, minBaseAngle, maxBaseAngle);
    }
    else if(command_char == "+")
    {
      triggerAngle = maxTriggerAngle;
    }
    else if(command_char == "-")
    {
      triggerAngle = minTriggerAngle;
    }
  }
}

void motorrun(int motor, int direction, int speed)
{
 if(motor == 1)
 {
  if(direction == 1)
  {
    analogWrite(lmotorf,speed);
  }
  else if(direction == -1)
  {
    analogWrite(lmotorb,speed);
  }
  
 }
 else if(motor == 2)
 {
  if(direction == 1)
  {
    analogWrite(rmotorf,speed);
  }
  else if(direction == -1)
  {
    analogWrite(rmotorb,speed);
  }
 }
}

void loop() {
  readJoysticks();
  setMotorSpeeds();
  setServoAngles();
  //setTrigger();
}

