#include <Wire.h>

int mpuAddr = -1;
float roll = 0.0, pitch = 0.0;
unsigned long lastTime = 0;

int findMPU() {
  int addrs[] = {0x68, 0x69};
  for (int i = 0; i < 2; i++) {
    Wire.beginTransmission(addrs[i]);
    if (Wire.endTransmission() == 0) {
      Wire.beginTransmission(addrs[i]);
      Wire.write(0x6B); // PWR_MGMT_1 register
      Wire.write(0x00); // Wake up MPU6050
      Wire.endTransmission(true);
      return addrs[i];
    }
  }
  return -1;
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Wire.begin(21, 22);
  Wire.setClock(100000);
  Wire.setTimeOut(50);

  mpuAddr = findMPU();
  lastTime = millis();
}

void loop() {
  if (mpuAddr == -1) {
    mpuAddr = findMPU();
    if (mpuAddr == -1) {
      Serial.println("NO_SENSOR_CHECK_WIRES");
      delay(400);
      return;
    }
  }

  Wire.beginTransmission(mpuAddr);
  Wire.write(0x3B); // ACCEL_XOUT_H
  if (Wire.endTransmission(false) != 0) {
    Serial.println("I2C_READ_ERR");
    mpuAddr = -1;
    delay(300);
    return;
  }

  Wire.requestFrom(mpuAddr, 14, true);
  if (Wire.available() < 14) {
    Serial.println("SHORT_READ_ERR");
    delay(300);
    return;
  }

  int16_t rawAx = (int16_t)((Wire.read() << 8) | Wire.read());
  int16_t rawAy = (int16_t)((Wire.read() << 8) | Wire.read());
  int16_t rawAz = (int16_t)((Wire.read() << 8) | Wire.read());
  Wire.read(); Wire.read(); // Skip temp
  int16_t rawGx = (int16_t)((Wire.read() << 8) | Wire.read());
  int16_t rawGy = (int16_t)((Wire.read() << 8) | Wire.read());
  Wire.read(); Wire.read(); // Skip Gz

  float ax = rawAx / 16384.0;
  float ay = rawAy / 16384.0;
  float az = rawAz / 16384.0;
  float gx = rawGx / 131.0;
  float gy = rawGy / 131.0;

  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  if (dt > 0.5 || dt <= 0) dt = 0.04;
  lastTime = now;

  float accelRoll  = atan2(ay, az) * 180.0 / PI;
  float accelPitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;

  roll  = 0.96 * (roll + gx * dt) + 0.04 * accelRoll;
  pitch = 0.96 * (pitch + gy * dt) + 0.04 * accelPitch;

  Serial.print(roll, 2);
  Serial.print(",");
  Serial.println(pitch, 2);

  delay(40);
}
