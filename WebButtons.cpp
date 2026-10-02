#include "WebButtons.h"

//////////////////////////////////////////////////////////////////

void TemperatureOn() {
  server.on("/TemperatureStateOn", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(Temperature, HIGH);
    StateTemperature = true;
    request->send(200, "text/plain", "Detecting temperature");

    led.setBrightness(20);
    led.setPixelColor(0, led.Color(0, 148, 217));
    led.show();
    delay(1000);
    led.clear();
    led.show();
  });
}

void TemperatureOff() {
  server.on("/TemperatureStateOff", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(Temperature, LOW);
    StateTemperature = false;
    request->send(200, "text/plain", "Stop detecting temperature");

    led.setBrightness(20);
    led.setPixelColor(0, led.Color(0, 148, 217));
    led.show();
    delay(1000);
    led.clear();
    led.show();
  });
}

//////////////////////////////////////////////////////////////////

void VibrationOn() {
  server.on("/VibrationStateOn", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(Vibration, HIGH);
    StateVibration = true;
    request->send(200, "text/plain", "Detecting vibration");

    led.setBrightness(20);
    led.setPixelColor(0, led.Color(15, 212, 8));
    led.show();
    delay(1000);
    led.clear();
    led.show();
  });
}

void VibrationOff() {
  server.on("/VibrationStateOff", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(Vibration, LOW);
    StateVibration = false;
    request->send(200, "text/plain", "Stop detecting vibration");

    led.setBrightness(20);
    led.setPixelColor(0, led.Color(15, 212, 8));
    led.show();
    delay(1000);
    led.clear();
    led.show();
  });
}

//////////////////////////////////////////////////////////////////

void AirOn() {
  server.on("/AirStateOn", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(Air, HIGH);
    StateAir = true;
    request->send(200, "text/plain", "Assessing air quality");

    led.setBrightness(20);
    led.setPixelColor(0, led.Color(179, 181, 179));
    led.show();
    delay(1000);
    led.clear();
    led.show();
  });
}

void AirOff() {
  server.on("/AirStateOff", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(Air, LOW);
    StateAir = false;
    request->send(200, "text/plain", "Stop assessing air quality");

    led.setBrightness(20);
    led.setPixelColor(0, led.Color(179, 181, 179));
    led.show();
    delay(1000);
    led.clear();
    led.show();
  });
}

//////////////////////////////////////////////////////////////////

void SoundOn() {
  server.on("/SoundStateOn", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(Sound, HIGH);
    StateSound = true;
    request->send(200, "text/plain", "Sound sensor up.");

    Serial.println("State Sound = True");

    led.setBrightness(20);
    led.setPixelColor(0, led.Color(245, 237, 0));
    led.show();
    delay(1000);
    led.clear();
    led.show();
  });
}

void SoundOff() {
  server.on("/SoundStateOff", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(Sound, LOW);
    StateSound = false;
    request->send(200, "text/plain", "Sound sensor off.");

    Serial.println("State Sound = False");

    led.setBrightness(20);
    led.setPixelColor(0, led.Color(245, 237, 0));
    led.show();
    delay(1000);
    led.clear();
    led.show();
  });
}