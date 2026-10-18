#ifndef Sound_H
#define Sound_H

#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <driver/i2s.h>

#define I2S_WS WorldSelect
#define I2S_SD SerialData
#define I2S_SCK SerialClock
#define I2S_PORT I2S_NUM_0

#define SAMPLE_RATE 16000
#define MIC_BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_32BIT
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT

extern const int SerialClock;
extern const int WorldSelect;
extern const int SerialData;

extern int32_t SoundValue;

void InitializeSound();
int32_t SoundReading();
void SendSoundData();

#endif