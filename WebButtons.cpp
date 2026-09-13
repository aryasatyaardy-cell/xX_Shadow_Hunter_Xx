#include "WebButtons.h"

void INMP441On() {
  server.on("/StateOn", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(INMP441, HIGH);
    State15 = true;
    request->send(200, "text/plain", "Enabling sound detector");

  led.setBrightness(20);
  led.setPixelColor(0, led.Color(0, 255, 255));
  led.show();
  delay(1000);
  led.clear();
  led.show();
  });
}

void INMP441Off() {
  server.on("/StateOff", HTTP_GET, [](AsyncWebServerRequest *request) {
  digitalWrite(INMP441, LOW);
  State15 = false;
  request->send(200, "text/plain", "Disabling sound detector");

  led.setBrightness(20);
  led.setPixelColor(0, led.Color(0, 255, 255));
  led.show();
  delay(1000);
  led.clear();
  led.show();
  });
}