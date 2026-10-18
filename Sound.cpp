#include "Sound.h"

extern AsyncWebServer server;
extern bool StateSound;

int32_t SoundValue = 0;

void InitializeSound() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = MIC_BITS_PER_SAMPLE,
    .channel_format = I2S_MIC_CHANNEL,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  const i2s_pin_config_t pin_config = {
    .mck_io_num = I2S_PIN_NO_CHANGE,
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

int32_t SoundReading() {
  if (!StateSound) {
    return SoundValue;
  }

  size_t bytesRead = 0;
  int32_t sample = 0;
  int64_t sum = 0;

  for (int i = 0; i < 10; i++) {
    i2s_read(I2S_PORT, &sample, sizeof(sample), &bytesRead, pdMS_TO_TICKS(100));
    sum += abs(sample);
  }

  SoundValue = sum / 100000;

  if (SoundValue >= 500) {
    SoundValue = 0 - SoundValue;
  }

  Serial.println(SoundValue - 1000);

  return SoundValue;
}

void SendSoundData() {
  server.on("/Sound_Data", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(SoundValue));
  });
}