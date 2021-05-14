/**
 * @file CaptiveRequestHandler.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"

#include "FS.h"
#include "SPIFFS.h"

class CaptiveRequestHandler : public AsyncWebHandler 
{
public:
    static StaticJsonDocument<2056> doc;

    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request){
        return true;
    }

    static String processor(const String& var)
    {
        return doc[var];
    }

    void readFile(fs::FS &fs, const char * path){
        Serial.printf("Reading file: %s\r\n", path);

        File file = fs.open(path);
        if(!file || file.isDirectory()){
            Serial.println("- failed to open file for reading");
            return;
        }

        Serial.println("- read from file:");
        while(file.available()){
            Serial.write(file.read());
        }
        file.close();
    }

    void handleRequest(AsyncWebServerRequest *request){
        Serial.println("Request Received");
        if(request->hasParam("data")){
            String json;
            bool data = request->getParam("data")->value();
            if(data){
                serializeJson(doc, json);
            }
            request->send(200, "text/json", json);
        }else if(request->hasParam("screen")){
            String screen = "/" + request->getParam("screen")->value();
            request->send(SPIFFS, screen);
            Serial.println(screen);
            // readFile(SPIFFS, screen.c_str());
        }else{
            request->send(SPIFFS, "/home.htm");
            Serial.println("/home.htm");
            // readFile(SPIFFS, "/home.htm");
        }
    }
};

StaticJsonDocument<2056> CaptiveRequestHandler::doc;