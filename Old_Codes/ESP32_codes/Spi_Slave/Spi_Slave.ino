#include <SPI.h>
#define slaveSelect 5

int delayTime = 50;
float receivedDistance, receivedVelocity;
char receivedDirection;

void setup() {
  Serial.begin(115200);
  pinMode(slaveSelect, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
}

void loop() {
  // Receiving data from Jetson Nano
  digitalWrite(slaveSelect, LOW);
  receivedDistance = SPI.transfer(0);
  receivedVelocity = SPI.transfer(0);
  receivedDirection = SPI.transfer(0);
  digitalWrite(slaveSelect, HIGH);

  // Process the received data
  Serial.println("Getting output");
  Serial.println(receivedDistance);
  Serial.println(receivedVelocity);
  Serial.println(receivedDirection);

  // Sending data to Jetson Nano
  int acceleration[3] = {1, 2, 3};
  int velocity[3] = {4, 5, 6};
  int position[3] = {7, 8, 9};
  int angles[3] = {10, 11, 12};

  digitalWrite(slaveSelectPin, LOW);
  for (int i = 0; i < 3; i++) {
    SPI.transfer(acceleration[i]);
  }
  for (int i = 0; i < 3; i++) {
    SPI.transfer(velocity[i]);
  }
  for (int i = 0; i < 3; i++) {
    SPI.transfer(position[i]);
  }
  for (int i = 0; i < 3; i++) {
    SPI.transfer(angles[i]);
  }
  digitalWrite(slaveSelectPin, HIGH);

  delay(delayTime);
}
