#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>

#include "wifi_config.h"
#include "signal.h"

#define LED 12
#define IR 5

ESP8266WebServer server(80);

IRsend irsend(IR);

void send_ir(IRsend irsend, unsigned int signal[]) {
  int hlz = 38;
  irsend.sendRaw(signal, sizeof(signal) / sizeof(signal[0]), hlz);
}

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

void handleLight() {
  if (server.method() != HTTP_GET) {
    jsonResponse(400, "error");
    return;
  }

  String power = server.arg("power");
  if (power == "on") {
    send_ir(irsend, light_on);
  } else if (power == "off") {
    send_ir(irsend, light_off);
  } else {
    jsonResponse(400, "invalid parameter (power=" + power + ")");
    return;
  }

  jsonResponse(200, "success (power=" + power + ")");
}

void handleTV() {
  if (server.method() != HTTP_GET) {
    jsonResponse(400, "error");
    return;
  }

  String command = server.arg("command");
  if (command == "power") {
    send_ir(irsend, tv_power);
  } else if (command == "next_channel") {
    send_ir(irsend, tv_next_channel);
  } else if (command == "previous_channel") {
    send_ir(irsend, tv_previos_channel);
  } else if (command == "volume_up") {
    send_ir(irsend, tv_volume_up);
  } else if (command == "volume_down") {
    send_ir(irsend, tv_volume_down);
  } else if (command == "switch_source") {
    send_ir(irsend, tv_switch_source);
  } else {
    jsonResponse(400, "invalid parameter (command=" + command + ")");
    return;
  }

  jsonResponse(200, "success (command=" + command + ")");
}


void setup() {
  irsend.begin();
  Serial.begin(115200);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

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

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/light", handleLight);
  server.on("/tv", handleTV);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}

void loop() {
  ArduinoOTA.handle();

  server.handleClient();
}
