#ifndef Web_H
#define Web_H
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include "Mic.h"

extern AsyncWebServer server;
extern Adafruit_NeoPixel led;

void InitateWifi();
void Web();
void INMP441_Data();

#endif