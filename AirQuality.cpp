#include "AirQuality.h"

void AirReading() {
  if (!StateAir) {
    return;
  }

  int airValue = analogRead(AirData);

  Serial.print("Air Quality: ");
  Serial.println(airValue);
  delay(1000);


  delay(200);
}

void SendAirData() {
  server.on("/Temperature_Data", HTTP_GET, [](AsyncWebServerRequest *request) {
    float AirReading = analogRead(AirData);
    request->send(200, "text/plain", String(AirReading));
  });
}