#include <math.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#include <DHT.h>

#include "Mic.h"
#include "Web.h"
#include "WebButtons.h"

const char *wifiName = "Futuristik";
const char *password = "H4f1dzGen";

#define LED_PIN 48
Adafruit_NeoPixel led(1, LED_PIN, NEO_GRB + NEO_KHZ800);

AsyncWebServer server(80);

const int INMP441 = 15;
bool State15 = false;
float I2S_Data = 0;

DHT dht(10, DHT22);

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();

  pinMode(INMP441, OUTPUT);
  digitalWrite(INMP441, LOW);

  // INMP441
  I2S_Setup();
  I2S_Set_INMP441_Pin();
  Start_I2S();
  delay(500);

  //Web
  InitateWifi();
  Web();

  //Mic Data
  INMP441_Data();

  //buttons asking data from web
  INMP441On();
  INMP441Off();


  server.begin();
  Serial.println("Web server started");
}

void SerialPlotting() {
  int db = Mic_Data();

  Serial.print("ymin:");
  Serial.print(-100);
  Serial.print(" ymax:");
  Serial.print(100);
  Serial.print(" mic:");
  Serial.println(db);
}

void loop() {
  float t = Temperature();
  float h = Humidity();

  //SerialPlotting();

  delay(20);
}

float Temperature() {
    float temperature = dht.readTemperature();
    Serial.println(temperature);
    Serial.print("°C");
    return temperature;
  }

  float Humidity() {
    float humidity = dht.readHumidity();
    Serial.println(humidity);
    Serial.print("%");
    return humidity;
  }