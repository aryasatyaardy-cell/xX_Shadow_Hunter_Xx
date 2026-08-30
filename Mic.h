#ifndef Mic_H
#define Mic_H
#include <Arduino.h>
#include <driver/i2s.h>

void I2S_Setup();
void I2S_Set_INMP441_Pin();
int Mic_Data();
void Start_I2S();
void Cleaned_Sound();

#endif