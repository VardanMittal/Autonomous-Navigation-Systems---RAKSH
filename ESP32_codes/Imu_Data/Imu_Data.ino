#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
float acceleration[3] = {0,0,0};
float velocity[3] = {0, 0, 0};
float position[3] = {0, 0, 0};
float angle[3] = {0, 0, 0};
// offset values after calibraion

float gyro_offset[3] = {427.80608415682497, 95.77760385127479, 352.8281772415862};

float accel_offset[3] = {1.625006842506306, 1.7734327402858574, 2.121733964930266};

float velo_offset[3] = {-0.14814994500101084, -0.21071542999354495, 0.0016253932162201268};

// time counter
unsigned long prevTime = 0;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Accelerometer: ");
  acceration[0] += a.acceleration.x - accel_offset[0];
  Serial.print(",");
  acceration[1] += a.acceleration.y - accel_offset[1];
  Serial.print(",");
  acceration[2] += a.acceleration.z - accel_offset[2];

  Serial.print(" Gyroscope: ");
  gyro[0] += g.gyro.x - gyro_offset[0];
  Serial.print(",");
  gyro[1] += g.gyro.y - gyro_offset[1];
  Serial.print(",");
  gyro[2] += g.gyro.z - gyro_offset[2];

  // Calculate time difference
  unsigned long currTime = millis();
  float timeDiff = (currTime - prevTime) / 1000.0; // in seconds
  prevTime = currTime;

  // Integration for velocity
  velocity[0] += acceleration[0] * timeDiff;
  velocity[1] += acceleration[1] * timeDiff;
  velocity[2] += acceleration[2] * timeDiff;

  // Integration for position
  position[0] += velocity[0] * timeDiff + 0.5 * acceleration[0] * timeDiff * timeDiff;
  position[1] += velocity[1] * timeDiff + 0.5 * acceleration[1] * timeDiff * timeDiff;
  position[2] += velocity[2] * timeDiff + 0.5 * acceleration[2] * timeDiff * timeDiff;

  // Gyroscope angle calculation
  angle[0] += gyro[0] * timeDiff;
  angle[1] += gyro[1] * timeDiff;
  angle[2] += gyro[2] * timeDiff;

  /* Print out the calculated values */
  Serial.print(" Velocity: ");
  Serial.print(velocity[0]);
  Serial.print(",");
  Serial.print(velocity[1]);
  Serial.print(",");
  Serial.print(velocity[2]);

  Serial.print(" Position: ");
  Serial.print(position[0]);
  Serial.print(",");
  Serial.print(position[1]);
  Serial.print(",");
  Serial.print(position[2]);

  Serial.print(" Angle: ");
  Serial.print(angle[0]);
  Serial.print(",");
  Serial.print(angle[1]);
  Serial.print(",");
  Serial.print(angle[2]);

  Serial.println("");

  delay(10);
}
