#include "Web.h"
#include "index_html.h"
#include "Web_css.h"
#include "Web_js.h"

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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", INDEX_HTML);
  });

  server.on("/Web.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/css", WEB_CSS);
  });

  server.on("/Web.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/javascript", WEB_JS);
  });
}

//void INMP441_Data() {
//  server.on("/I2S_Data", HTTP_GET, [](AsyncWebServerRequest *request) {
    //int value = Mic_Data();
    //request->send(200, "text/plain", String(value));
//  });
//}
