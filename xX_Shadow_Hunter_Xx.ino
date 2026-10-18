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

const int Air = 15;
const int AirData = 4;

const int Sound = 17;
const int SoundData = 0;

bool StateTemperature = false;
bool StateVibration = false;
bool StateAir = false;
bool StateSound = false;

String KeyPressed = ""; 

void KeyInput() {
  server.on("/Key", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value") && request->hasParam("state")) {
      String Key = request->getParam("value")->value();
      bool pressed = request->getParam("state")->value() == "1";

      KeyPressed = Key + (pressed ? "Pressed" : "Released");  
    }
    WalkingInput();
    request->send(200, "text/plain", "ok");
  });
}

void WalkingInput() {
  if (KeyPressed == "wPressed") {
    WalkForward();
  } else if (KeyPressed == "aPressed"){
    WalkLeft();
  } else if (KeyPressed == "sPressed"){
    WalkBackward();
  } else if (KeyPressed == "dPressed"){
    WalkRight();
  } else if (KeyPressed == "qPressed"){
    TurnLeft();
  } else if (KeyPressed == "ePressed"){
    TurnRight();
  } else if (
    KeyPressed == "wReleased" || 
    KeyPressed == "aReleased" || 
    KeyPressed == "sReleased" || 
    KeyPressed == "dReleased" ||
    KeyPressed == "qReleased" ||
    KeyPressed == "eReleased" 
    ) {
    StayStill();
  }
}

void WalkForward(){
  Serial.println("Walking forward...");
}

void WalkLeft(){
  Serial.println("Walking left...");
}

void WalkRight(){
  Serial.println("Walking right...");
}

void WalkBackward(){
  Serial.println("Walking backward...");
}

void TurnLeft(){
  Serial.println("Turning left...");
}

void TurnRight(){
  Serial.println("Turning right...");
}

void StayStill(){
  Serial.println("Standing still...");
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
