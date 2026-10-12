#ifndef AirQuality_H
#define AirQuality_H
#include <ESPAsyncWebServer.h>

extern const int Air;
extern const int AirData;

extern AsyncWebServer server;
extern bool StateAir;

void AirReading();
void SendAirData();

#endif