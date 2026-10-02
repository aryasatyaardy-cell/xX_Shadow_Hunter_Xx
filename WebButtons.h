#ifndef WebButtons_H
#define WebButtons_H
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>

extern AsyncWebServer server;
extern Adafruit_NeoPixel led;

extern const int Temperature;
extern const int Vibration;
extern const int Air;
extern const int Sound;

extern bool StateTemperature;
extern bool StateVibration;
extern bool StateAir;
extern bool StateSound;

void TemperatureOn();
void TemperatureOff();

void VibrationOn();
void VibrationOff();

void AirOn();
void AirOff();

void SoundOn();
void SoundOff();

#endif