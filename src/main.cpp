/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"
#include "CaptiveRequestHandler.hpp"
#include "DongleCfg.hpp"

#include "FS.h"
#include "SPIFFS.h"

DNSServer dnsServer;
AsyncWebServer server(80);
DongleCfg cfg = DongleCfg("Gerdau L2 IoT", "riograndense");

void taskCommunication();

void setup(){
  Serial.begin(115200);
  if(!SPIFFS.begin(true)){
      Serial.println("SPIFFS Mount Failed");
      return;
  }
  WiFi.softAP(cfg.getSsid(), cfg.getPass());
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  server.begin();
}

void loop(){
  dnsServer.processNextRequest();
  taskCommunication();
}

void taskCommunication(){
  if(Serial.available()){
    String data = Serial.readStringUntil('\n');
    if(data == "WifiOpcDongle?"){
      Serial.println("Yes");
    }else{
      StaticJsonDocument<512> temp;
      deserializeJson(temp, data);
      String key = temp["Name"];
      CaptiveRequestHandler::doc[key] = temp["Value"];
    }
  }
}