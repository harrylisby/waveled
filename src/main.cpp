#include <Arduino.h>
#include <ESPUI.h>
#include <EEPROM.h>
#include "ESPAsyncWiFiManager.h"
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
//#include <WiFiManager.h>

#if defined(ESP32)
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

const char *ssid = "Lisby";
const char *password = "lisby123456";
const char *ssid2 = "waveled";

long oldTime = 0;
bool switchi = false;
byte triesCount = 0;

AsyncWebServer server(80);
DNSServer dns;

void slider(Control sender, int type) {
  Serial.println(sender.value);
}

void buttonCallback(Control sender, int type) {
  switch (type) {
  case B_DOWN:
    Serial.println("Button DOWN");
    break;
  case B_UP:
    Serial.println("Button UP");
    break;
  }
}

void buttonExample(Control sender, int type) {
  switch (type) {
  case B_DOWN:
    Serial.println("Status: Start");
    ESPUI.print(0, "Status: Start");
    break;
  case B_UP:
    Serial.println("Status: Stop");
    ESPUI.print(0, "Status: Stop");
    break;
  }
}

void padExample(Control sender, int value) {
  switch (value) {
  case P_LEFT_DOWN:
    Serial.print("left down");
    break;
  case P_LEFT_UP:
    Serial.print("left up");
    break;
  case P_RIGHT_DOWN:
    Serial.print("right down");
    break;
  case P_RIGHT_UP:
    Serial.print("right up");
    break;
  case P_FOR_DOWN:
    Serial.print("for down");
    break;
  case P_FOR_UP:
    Serial.print("for up");
    break;
  case P_BACK_DOWN:
    Serial.print("back down");
    break;
  case P_BACK_UP:
    Serial.print("back up");
    break;
  case P_CENTER_DOWN:
    Serial.print("center down");
    break;
  case P_CENTER_UP:
    Serial.print("center up");
    break;
  }
  Serial.print(" ");
  Serial.println(sender.id);
}

void switchExample(Control sender, int value) {
  switch (value) {
  case S_ACTIVE:
    Serial.print("Active:");
    break;
  case S_INACTIVE:
    Serial.print("Inactive");
    break;
  }
  Serial.print(" ");
  Serial.println(sender.id);
}

void otherSwitchExample(Control sender, int value) {
  switch (value) {
  case S_ACTIVE:
    Serial.print("Active:");
    break;
  case S_INACTIVE:
    Serial.print("Inactive");
    break;
  }
  Serial.print(" ");
  Serial.println(sender.id);
}

void setup(void) {
  Serial.begin(115200);
  AsyncWiFiManager iMngr(&server,&dns);
  iMngr.autoConnect(ssid2);

/*
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(triesCount<10){
      triesCount++;
    }else if(triesCount>=10){
      #if defined(ESP32)
      WiFi.setHostname(ssid2);
      #else
      WiFi.hostname(ssid2);
      #endif
      WiFi.mode(WIFI_AP);
      WiFi.softAP(ssid2);
      // WiFi.softAP(ssid, password);
      Serial.println("");
      Serial.print("IP address: ");
      Serial.println(WiFi.softAPIP());
      break;
    }
  }
  if(WiFi.status()==WL_CONNECTED){
    Serial.println("");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
*/
  ESPUI.label("Status:", COLOR_WETASPHALT, "Stop");
  ESPUI.label("Millis:", COLOR_WETASPHALT, "0");
  ESPUI.button("Push Button", &buttonCallback, COLOR_WETASPHALT);
  ESPUI.button("Other Button", &buttonExample, COLOR_WETASPHALT, "Press");
  ESPUI.pad("Pad with center", true, &padExample, COLOR_WETASPHALT);
  ESPUI.pad("Pad without center", false, &padExample, COLOR_WETASPHALT);
  ESPUI.switcher("Switch one", false, &switchExample, COLOR_WETASPHALT);
  ESPUI.switcher("Switch two", true, &otherSwitchExample, COLOR_WETASPHALT);
  ESPUI.slider("Slider one", &slider, COLOR_WETASPHALT, "30");
  ESPUI.slider("Slider two", &slider, COLOR_WETASPHALT, "100");

  /*
     .begin loads and serves all files from PROGMEM directly.
     If you want to serve the files from SPIFFS use .beginSPIFFS (.prepareFileSystem has to be run in an empty sketch before)
   */
  ESPUI.begin("ESPUI Control");
}

void loop(void) {
  if (millis() - oldTime > 5000) {
    ESPUI.print("Millis:", String(millis()));
    switchi = !switchi;
    ESPUI.updateSwitcher("Switch one", switchi);
    oldTime = millis();
    //yay
  }
}
