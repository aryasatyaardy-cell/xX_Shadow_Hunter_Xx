#include "AirQuality.h"

int airValue = 0;

void AirReading() {
  if (!StateAir) {
    return;
  }

  airValue = analogRead(AirData);

  Serial.print("Air Quality: ");
  Serial.println(airValue);

  if (airValue >= 240) {
    airValue = (airValue + 300) * (-1);
  }

  delay(200);
}

void SendAirData() {
  server.on("/Air_Data", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(airValue));
  });
}