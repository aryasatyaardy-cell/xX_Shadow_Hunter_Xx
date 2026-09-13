#include "Mic.h"

#define I2S_SD   11 //Data
#define I2S_SCK  12 //Clock
#define I2S_WS   13 //Left/right

#define I2S_INMP441 I2S_NUM_0

void Start_I2S(){
  i2s_start(I2S_INMP441);
}

int Mic_Data() {
  int32_t sample = 0;
  size_t byte = 0;
  int32_t sum = 0;
  int count = 0;
  int db = 0;

  for (int i = 0; i < 10; i++) { //loop 20x, data di average kan
    i2s_read(I2S_INMP441, (char*)&sample, sizeof(sample), &byte, 100);
    if (byte > 0) {
      sum += (sample >> 18); //jumlah semua data yang udh dibagi sama angkanya pokonya (kurleb)
      count++; 
    }
  }

  if (count > 0) {
  db = (sum / count);
  return db;
  }

  if (db > 100) {
    server.on("/LoudNoise", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Loud sound detected nearby");
    });
  }

  return 0;
}

void I2S_Setup() {
  const i2s_config_t I2S_setup = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), //i2s_mode_t added cus c++ convert (i2s mode | i2s rx) to an int
    .sample_rate = 16000,
    .bits_per_sample =  i2s_bits_per_sample_t(32),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB), //normal I2S data is normal data obv, the mbs arrange it so it's in correct order
    .intr_alloc_flags = 0,  //ngasi tau that inmp got a data
    .dma_buf_count = 8, //how many waiting room
    .dma_buf_len = 64, //how many chair in that room
    .use_apll = false //fancy way for esp to make a clock
  };

  i2s_driver_install(I2S_INMP441, &I2S_setup, 0, NULL);
}


void I2S_Set_INMP441_Pin(){
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_set_pin(I2S_INMP441, &pin_config);
}