// https://ai.thestempedia.com/docs/dabble-app/gamepad-module/

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

#include <DabbleESP32.h>
#include <Every.h>
#include <Servo.h>

// Right motor
#define IN1 21
#define IN2 22

// Left motor
#define IN3 16
#define IN4 17

// RPM values for the motor speed
#define RIGHT_SPEED 255
#define LEFT_SPEED 255

// Servo
#define SERVO_PIN 27
#define SERVO_DEF 90

Servo servo;
int servoPos = SERVO_DEF;

Every servoTimeout(100);
bool stop = true;

void setup() {
  Serial.begin(115200);

  setupMotor();
  rotateMotor(0, 0);

  pinMode(SERVO_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  servo.write(SERVO_DEF);

  Dabble.begin("##### ButterRobot #####");

  delay(200);
  Serial.println("Ready");
  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);

  int rightMotorSpeed = 0;
  int leftMotorSpeed = 0;

  Dabble.processInput();

  if (GamePad.isUpPressed()) {
    stop = false;
    rotateMotor(RIGHT_SPEED, LEFT_SPEED);
    Serial.println("Up");
  } else if (GamePad.isDownPressed()) {
    stop = false;
    rotateMotor(-RIGHT_SPEED, -LEFT_SPEED);
    Serial.println("Down");
  } else if (GamePad.isLeftPressed()) {
    stop = false;
    rotateMotor(-RIGHT_SPEED, LEFT_SPEED);
    Serial.println("Left");
  } else if (GamePad.isRightPressed()) {
    stop = false;
    rotateMotor(RIGHT_SPEED, -LEFT_SPEED);
    Serial.println("Right");
  } else {
    if (stop != true) {
      rotateMotor(0, 0);
      Serial.println("Stop");
      stop = true;
    }
  }

  if (servoTimeout()) {
    if (GamePad.isTrianglePressed()) {
      if (servoPos < 120) {
        servoPos += 10;
        servo.write(servoPos);
      }
      Serial.println(servoPos);
    }

    if (GamePad.isCrossPressed()) {
      if (servoPos > 50) {
        servoPos -= 10;
        servo.write(servoPos);
      }
      Serial.println(servoPos);
    }
  }
}

void setupMotor() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void rotateMotor(int rightSpeed, int leftSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  float pin1 = 0, pin2 = 0, pin3 = 0, pin4 = 0;

  if (leftSpeed > 0)
    pin2 = leftSpeed;
  else
    pin1 = -1 * leftSpeed;

  if (rightSpeed > 0)
    pin3 = rightSpeed;
  else
    pin4 = -1 * rightSpeed;

  analogWrite(IN1, (int)pin1);
  analogWrite(IN2, (int)pin2);
  analogWrite(IN3, (int)pin3);
  analogWrite(IN4, (int)pin4);
}
