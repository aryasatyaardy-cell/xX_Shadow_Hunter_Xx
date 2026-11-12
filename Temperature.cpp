#include "Temperature.h"

DHT TemperatureSensor(TemperatureData, DHT22);

void TemperatureSensorInitialization() {
  TemperatureSensor.begin();
}

void TemperatureReading() {
  if (!StateTemperature) {
    return;
  }

  float TemperatureReading = TemperatureSensor.readTemperature();
  float HumidityReading = TemperatureSensor.readHumidity();

  Serial.print("Temp: ");
  Serial.print(TemperatureReading);
  Serial.print("°C  Humidity: ");
  Serial.print(HumidityReading);
  Serial.println("%");

  delay(2000);
}

void SendTemperatureData() {
  server.on("/Temperature_Data", HTTP_GET, [](AsyncWebServerRequest *request) {
    float TemperatureReading = TemperatureSensor.readTemperature();
    request->send(200, "text/plain", String(TemperatureReading));
  });

  server.on("/Humidity_Data", HTTP_GET, [](AsyncWebServerRequest *request) {
    float HumidityReading = TemperatureSensor.readHumidity();
    request->send(200, "text/plain", String(HumidityReading));
  });
}