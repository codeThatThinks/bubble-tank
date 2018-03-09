#include <AFMotor.h>

AF_DCMotor left_motor(1);
AF_DCMotor right_motor(2);

void setup() {
  // put your setup code here, to run once:
  left_motor.setSpeed(128);
  right_motor.setSpeed(128);
}

void loop() {
  // put your main code here, to run repeatedly:
  left_motor.run(FORWARD);
  right_motor.run(FORWARD);
  delay(2000);
  left_motor.run(RELEASE);
  right_motor.run(RELEASE);
  delay(100);
  left_motor.run(BACKWARD);
  right_motor.run(BACKWARD);
  delay(2000);
  left_motor.run(RELEASE);
  right_motor.run(RELEASE);
  delay(100);
}
