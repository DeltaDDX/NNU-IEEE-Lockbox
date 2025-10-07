#include <Servo.h>

#define SERVO_PIN 7

const int SERVO_LOCK_ANGLE = 0;
const int SERVO_UNLOCK_ANGLE = 90;
const unsigned long HOLD_MS = 2000;  // time to hold each position

Servo lockServo; // Instantiate Servo Object

void setup() {
  lockServo.attach(SERVO_PIN);
  lockServo.write(SERVO_LOCK_ANGLE);
}


void loop() {
  delay(500);
}

void servo_unlock() {
  lockServo.write(SERVO_UNLOCK_ANGLE);
  delay(HOLD_MS);
  lockServo.write(SERVO_LOCK_ANGLE);
}
