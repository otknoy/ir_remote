#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include <IRDaikinESP.h>

#include "wifi_config.h"

#define LED 12
#define IR 5

ESP8266WebServer server(80);

void jsonResponse(int status, String message) {
  String response = "{";
  response += "\"status\": " + String(status);
  response += ", ";
  response += "\"message\": \"" + message + "\"";
  response += "}";

  server.send(status, "application/json", response);
}

void handleRoot() {
  String s = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"></head><body>";
  s = s + "<p>ssid: " + ssid + "</p>";
  s = s + "<p>: " + ssid + "</p>";
  s = s + "<p>IP address: " + WiFi.localIP() + "</p>";
  s = s + "</body></html>";
  server.send(200, "text/html", s);
}

void handleNotFound() {
  jsonResponse(404, "file not found");
}

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  Serial.println("Booting");

  WiFi.config(IPAddress(192, 168,   1,  60),
	      IPAddress(192, 168,   1,   1),
	      IPAddress(255, 255, 255,   0));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, HIGH);
    delay(250);

    digitalWrite(LED, LOW);
    delay(250);

    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(LED, HIGH);

  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  //ArduinoOTA.setPassword((const char *)"otknoy");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  server.on("/", handleRoot);
  server.on("/light", handleLight);
  server.on("/tv", handleTV);
  server.on("/ac", handleAC);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  ArduinoOTA.handle();

  server.handleClient();

  yield();
}
