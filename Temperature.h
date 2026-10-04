#ifndef Temperature_H
#define Temperature_H
#include <ESPAsyncWebServer.h>
#include <DHT.h>

extern const int Temperature;
extern AsyncWebServer server;
extern bool StateTemperature;

void TemperatureSensorInitialization();
void TemperatureReading();
void SendTemperatureData();

#endif