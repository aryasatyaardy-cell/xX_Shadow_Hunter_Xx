#include "Web.h"
#include <LittleFS.h>

void InitateWifi() {

  const char *wifiName = "Futuristik";
  const char *password = "H4f1dzGen";

  WiFi.begin(wifiName, password);

  led.setBrightness(20);
  led.setPixelColor(0, led.Color(0, 255, 255));
  led.show();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  led.clear();
  led.show();
}

void Web() {
  // Make sure LittleFS is mounted once (safe to call again if already mounted elsewhere)
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed - upload Web.html, Web.css, Web.js to the filesystem image");
  }

  // Serve the three files straight off flash. serveStatic sets the right
  // Content-Type automatically based on the file extension.
  server.serveStatic("/", LittleFS, "/Web.html").setDefaultFile("Web.html");
  server.serveStatic("/Web.css", LittleFS, "/Web.css");
  server.serveStatic("/Web.js", LittleFS, "/Web.js");
}

void INMP441_Data() {
  server.on("/I2S_Data", HTTP_GET, [](AsyncWebServerRequest *request){
    int value = Mic_Data();
    request->send(200, "text/plain", String(value));
  });
}
