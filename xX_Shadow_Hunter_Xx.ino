  #include <math.h>
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
  #include <Adafruit_NeoPixel.h>

  #include "Mic.h"
  #include "Web.h"
  #include "WebButtons.h"

  const char *wifiName = "Futuristik";
  const char *password = "H4f1dzGen";

  #define LED_PIN 48
  Adafruit_NeoPixel led(1, LED_PIN, NEO_GRB + NEO_KHZ800);

  AsyncWebServer server(80);

  const int Temperature = 15;
  const int Vibration = 15;
  const int Air = 15;
  const int Sound = 15;

  bool StateTemperature = false;
  bool StateVibration = false;
  bool StateAir = false;
  bool StateSound = false;

  void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(Temperature, OUTPUT);
    digitalWrite(Temperature, LOW);

    pinMode(Vibration, OUTPUT);
    digitalWrite(Vibration, LOW);

    pinMode(Air, OUTPUT);
    digitalWrite(Air, LOW);

    pinMode(Sound, OUTPUT);
    digitalWrite(Sound, LOW);

    //Web
    InitateWifi();
    Web();

    //buttons asking data from web
    TemperatureOn();
    TemperatureOff();

    VibrationOn();
    VibrationOff();

    AirOn();
    AirOff();

    SoundOn();
    SoundOff();

    server.begin();
    Serial.println("Web server started");

    led.show();
  }

  void loop() {
    //Serial.print("ymin:");
    //Serial.print(-100);
    //Serial.print(" ymax:");
    //Serial.print(100);
    //Serial.print(" mic:");

    delay(20);
  }
