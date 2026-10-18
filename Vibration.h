#ifndef Vibration_H
#define Vibration_H
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <Wire.h>
// I2S Library

extern AsyncWebServer server;

extern const int VibrationSensorAddressI2C;
extern float VibrationData;
extern float VibrationDataDivided;
extern bool StateVibration;

extern const int VibrationSDA;
extern const int VibrationSCL;

extern const int X;
extern const int Y;
extern const int Z;

int ReadAxis(int reg);

void VibrationReading();
void VibrationSensorInitialization();

void SendVibrationData();

#endif