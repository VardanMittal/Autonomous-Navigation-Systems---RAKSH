#include <Arduino.h>
#include <QuickPID.h>

// Define the motor control pin
const int motorEnablePin = 9;
#define encoderPIN1 2
#define encoderPIN2 3
// PID Controller parameters
float kp = 0.6;
float ki = 0.3;
float kd = 0.2;

// PID Controller initialization
float prevError = 0;
float integral = 0;


QuickPID &PID()
// Hall effect encoder reading
int readEncoder()
{
  int enc1 = digitalRead(encoderPIN1);
  int enc2 = digitalRead(encoderPIN2);
  
  return 0;
}

float pidControl(float desiredValue, float currentValue)
{
  float error = desiredValue - currentValue;
  integral += error;
  float derivative = error - prevError;

  float output = kp * error + ki * integral + kd * derivative;

  prevError = error;

  return output;
}

void setup()
{
  pinMode(motorEnablePin, OUTPUT);
  pinMode(encoderPin1, INPUT_PULLUP);
  pinMode(encoderPin2, INPUT_PULLUP);
}

void loop()
{
  float desiredSpeed = 100; // Desired speed (e.g., in RPM)
  int currentSpeed = readEncoder(); // Read the current speed from the hall effect encoder

  // Compute the PID output
  float controlValue = pidControl(desiredSpeed, currentSpeed);

  // Convert the control value to PWM duty cycle
  if (controlValue > 0)
  {
    analogWrite(motorEnablePin, controlValue);
  }
  else
  {
    analogWrite(motorEnablePin, 0);
  }

  delay(100); // Control loop delay
}
