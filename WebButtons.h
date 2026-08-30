#ifndef WebButtons_H
#define WebButtons_H
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>

extern AsyncWebServer server;
extern Adafruit_NeoPixel led;
extern const int INMP441;
extern bool State15;

void INMP441On();
void INMP441Off();

#endif