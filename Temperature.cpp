#include "Temperature.h"

DHT TemperatureSensor(Temperature, DHT22);

void TemperatureSensorInitialization() {
  TemperatureSensor.begin();
}

void TemperatureReading() {
  if (!StateTemperature) {
    return;
  }

  float TemperatureReading = TemperatureSensor.readTemperature();
  Serial.print("Temp: ");
  Serial.print(TemperatureReading);
  Serial.println("°C");



  delay(200);
}

void SendTemperatureData() {
  server.on("/Temperature_Data", HTTP_GET, [](AsyncWebServerRequest *request) {
    float TemperatureReading = TemperatureSensor.readTemperature();
    request->send(200, "text/plain", String(TemperatureReading));
  });
}