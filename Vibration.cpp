#include "Vibration.h"

int ReadAxis(int reg) {
  Wire.beginTransmission(VibrationSensorAddressI2C);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(VibrationSensorAddressI2C, 2);

  if (Wire.available() < 2) return 0;

  int low = Wire.read();
  int high = Wire.read();

  return (int16_t)((high << 8) | low);
}

void VibrationReading() {
  if (!StateVibration) {
    return;
  }

  static int x = 0, y = 0, z = 0;

  int NewX = ReadAxis(X);
  int NewY = ReadAxis(Y);
  int NewZ = ReadAxis(Z);

  VibrationData = ((NewX - x) * (NewX - x) + (NewY - y) * (NewY - y) + (NewZ - z) * (NewZ - z)) / 100000;

  x = NewX;
  y = NewY;
  z = NewZ;

  // Vibration data is raw indicator of movement or vibration

  // if (VibrationData >= 0 && VibrationData <= 120) {
  //   VibrationData = 0;
  // }

  if (VibrationData >= 300) {
    VibrationDataDivided = (0 - VibrationData) / 2;
  } else {
    VibrationDataDivided = VibrationData / 10;
  }
  Serial.println(VibrationDataDivided);

  delay(100);
}

void SendVibrationData() {
  server.on("/Vibration_Data", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(VibrationDataDivided));
  });
}

void VibrationSensorInitialization() {
  Wire.begin(VibrationSDA, VibrationSCL);

  Wire.beginTransmission(VibrationSensorAddressI2C);
  Wire.write(0x7E);
  Wire.write(0x11);
  Wire.endTransmission();
}