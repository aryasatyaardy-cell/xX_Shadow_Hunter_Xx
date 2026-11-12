#include "AirQuality.h"

int airValue = 0;

void AirReading() {
  if (!StateAir) {
    return;
  }

  airValue = analogRead(AirData);

  Serial.print("Air Quality: ");
  Serial.println(airValue);

  // Re-enable later once you know your clean-air baseline:
  // if (airValue <= 1000) {
  //   airValue = 0;
  // }

  delay(200);
}

void SendAirData() {
  server.on("/Air_Data", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(airValue));
  });
}