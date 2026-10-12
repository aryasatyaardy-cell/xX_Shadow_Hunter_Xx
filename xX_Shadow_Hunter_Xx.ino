#include <math.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#include "Web.h"
#include "WebButtons.h"
#include "Temperature.h"
#include "AirQuality.h"

const char *wifiName = "FuturistikBarat";
const char *password = "H4f1dzGen";

// Vibration
const int VibrationSensorAddressI2C = 0x69;
int VibrationData = 0;

#define VibrationSDA 21
#define VibrationSCL 47

#define X 0x12
#define Y 0x14
#define Z 0x16

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

//LED
#define LED_PIN 48
Adafruit_NeoPixel led(1, LED_PIN, NEO_GRB + NEO_KHZ800);

AsyncWebServer server(80);

const int Servo = 5;

const int Temperature = 15;
const int TemperatureData = 16;

const int Vibration = 20;

const int Air = 15;
const int AirData = 4;

const int Sound = 15;
const int SoundData = 0;

bool StateTemperature = false;
bool StateVibration = false;
bool StateAir = false;
bool StateSound = false;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(VibrationSDA, VibrationSCL);

  Wire.beginTransmission(VibrationSensorAddressI2C);
  Wire.write(0x7E);
  Wire.write(0x11);
  Wire.endTransmission();

  delay(100);

  pinMode(Vibration, OUTPUT);
  digitalWrite(Vibration, LOW);

  pinMode(Air, OUTPUT);
  digitalWrite(Air, LOW);

  pinMode(Sound, OUTPUT);
  digitalWrite(Sound, LOW);

  pinMode(Servo, OUTPUT);
  digitalWrite(Servo, LOW);

  //Web
  InitateWifi();
  Web();

  //buttons asking data from web
  TemperatureOn();
  TemperatureOff();

  VibrationOn();
  VibrationOff();

  AirOn();
  AirOff();

  SoundOn();
  SoundOff();

  server.begin();
  Serial.println("Web server started");

  led.show();

  //Air
  SendAirData();

  //Temperature
  pinMode(Temperature, OUTPUT);
  digitalWrite(Temperature, LOW);

  TemperatureSensorInitialization();
  SendTemperatureData();

  delay(2000);
}

void loop() {
  static int x = 0, y = 0, z = 0;

  int NewX = ReadAxis(X);
  int NewY = ReadAxis(Y);
  int NewZ = ReadAxis(Z);

  VibrationData = ((NewX - x) * (NewX - x) + (NewY - y) * (NewY - y) + (NewZ - z) * (NewZ - z)) / (NewX + NewY + NewZ);

  x = NewX;
  y = NewY;
  z = NewZ;

  // Vibration data is raw indicator of movement or vibration

  // if (VibrationData >= 0 && VibrationData <= 120) {
  //   VibrationData = 0;
  // }

  Serial.println(VibrationData);

  delay(500);

  TemperatureReading();
  AirReading();

  //Serial.print("ymin:");
  //Serial.print(-100);
  //Serial.print(" ymax:");
  //Serial.print(100);
  //Serial.print(" mic:");
}