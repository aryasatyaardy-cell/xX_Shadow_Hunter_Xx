#include <math.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#include "Web.h"
#include "WebButtons.h"
#include "Temperature.h"
#include "AirQuality.h"
#include "Vibration.h"
#include "Sound.h"
#include "Walking.h"

// with the Vibration pins (INMP441: SCK, WS, SD)
const int SerialClock = 10;
const int WorldSelect = 11;
const int SerialData = 3;

// 10-14 is free

// Vibration
const int VibrationSensorAddressI2C = 0x68;
float VibrationData = 0;
float VibrationDataDivided = 0;

const int X = 0x12;
const int Y = 0x14;
const int Z = 0x16;

// Cables for sda and scl of the vibration thing
const int VibrationSDA = 38;
const int VibrationSCL = 39;

//LED
#define LED_PIN 48
Adafruit_NeoPixel led(1, LED_PIN, NEO_GRB + NEO_KHZ800);

AsyncWebServer server(80);

const int Servo = 5;

const int Temperature = 15;
const int TemperatureData = 16;

const int Vibration = 40;

const int Air = 6;
const int AirData = 4;

const int Sound = 17;
const int SoundData = 0;

bool StateTemperature = false;
bool StateVibration = false;
bool StateAir = false;
bool StateSound = false;

// Keys sent by the web page: w a s d = walk, q e = turn.
// The page sends state 1 when a key goes down and 0 when it is released.
void KeyInput() {
  server.on("/Key", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value") && request->hasParam("state")) {
      String Key = request->getParam("value")->value();
      bool pressed = request->getParam("state")->value() == "1";

      if (Key.length() == 1) {
        if (pressed) {
          WalkingKeyPressed(Key.charAt(0));
        } else {
          WalkingKeyReleased(Key.charAt(0));
        }
      }
    }

    request->send(200, "text/plain", "ok");
  });
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  KeyInput();

  pinMode(Vibration, OUTPUT);
  digitalWrite(Vibration, HIGH);

  delay(5000);

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

  //Air.h
  SendAirData();

  delay(2000);

  //Temperature.h
  pinMode(Temperature, OUTPUT);
  digitalWrite(Temperature, LOW);

  TemperatureSensorInitialization();
  SendTemperatureData();

  delay(2000);

  // Vibration.h
  // VibrationSensorInitialization();
  SendVibrationData();

  // Sound.h
  // InitializeSound();
  SendSoundData();


  delay(2000);

  // Servo driver + walking (runs in its own task)
  WalkingBegin();
}

void loop() {
  int ReadAxis(int reg);
  
  TemperatureReading();
  AirReading();
  VibrationReading();
  SoundReading();

  //Serial.print("ymin:");
  //Serial.print(-100);
  //Serial.print(" ymax:");
  //Serial.print(100);
  //Serial.print(" mic:");
}