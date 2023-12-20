#include <Arduino.h>
#include <QuickPID.h>
#include <math.h>
/////////////////////////////////SPI DETAILS?????????????????????
#define WheelDia 0.08

float distance, speed;
char direction;

float count = distance / (3.14 * WheelDia);

// Motor Pin Variables
#define motorPWM 9
#define motorIN1 16
#define motorIN2 17
#define motorIN3 14
#define motorIN4 15
// #define encoderA1 2
// #define encoderA2 3
// #define encoderB1 20
// #define encoderB2 21

// PID Controller parameters
float kp = 0.6, ki = 0.3, kd = 0.2;

// PID Controller initialization
float input, output, setpoint;

QuickPID PID(&input, &output, &setpoint);

// Getting encoder reading (Need to think on how to use these four values togeather)
// int readEncoder()
// {
//   int enc1 = digitalRead(encoderA1);
//   int enc2 = digitalRead(encoderA2);
//   int enc3 = digitalRead(encoderB1);
//   int enc4 = digitalRead(encoderB2);
//   return enc1;
// }

void setup()
{
  //controller setups
  PID.SetOutputLimits(0,255);
  //Motor Control pin setup
  pinMode(motorPWM, OUTPUT);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  pinMode(motorIN3, OUTPUT);
  pinMode(motorIN4, OUTPUT);
  // Encoder pin setup
  // pinMode(encoderA1, INPUT_PULLUP);
  // pinMode(encoderA2, INPUT_PULLUP);
  // pinMode(encoderB1, INPUT_PULLUP);
  // pinMode(encoderB2, INPUT_PULLUP);
  //Serial beginner
  // Serial.begin(115200);
  direction = 'F';
  speed = 1;
}

void loop()
{
  // PID and PWM control signal updation
  setpoint += count;
  // input = readEncoder();
  output = PID.Compute();


  //Control logic
  if(direction =='F'){
    moveForward();
  }
  else if(direction == 'B'){
    moveBackward();
  }
  else if(direction == 'R'){
    moveRight();
  }
  else if(direction == 'L'){
    moveLeft();
  }
  else{
    stopMotor();
  }

  //Final Control Signals
  analogWrite(motorPWM, output);
  delay(100);
}

// control Signals
void moveForward() {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, HIGH);
  digitalWrite(motorIN4, LOW);
}

void moveBackward() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, HIGH);
}

void moveLeft() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
  digitalWrite(motorIN3, HIGH);
  digitalWrite(motorIN4, LOW);
}

void moveRight() {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, HIGH);
}

void stopMotor() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, LOW);
}
