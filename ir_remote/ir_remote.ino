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
    irsend.sendRaw(light_on, sizeof(light_on) / sizeof(light_on[0]), 38);
  } else if (power == "off") {
    irsend.sendRaw(light_off, sizeof(light_off) / sizeof(light_off[0]), 38);
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
    irsend.sendRaw(tv_power, sizeof(tv_power) / sizeof(tv_power[0]), 38);
  } else if (command == "next_channel") {
    irsend.sendRaw(tv_next_channel, sizeof(tv_next_channel) / sizeof(tv_next_channel[0]), 38);
  } else if (command == "previous_channel") {
    irsend.sendRaw(tv_previos_channel, sizeof(tv_previos_channel) / sizeof(tv_previos_channel[0]), 38);
  } else if (command == "volume_up") {
    irsend.sendRaw(tv_volume_up, sizeof(tv_volume_up) / sizeof(tv_volume_up[0]), 38);
  } else if (command == "volume_down") {
    irsend.sendRaw(tv_volume_down, sizeof(tv_volume_down) / sizeof(tv_volume_down[0]), 38);
  } else if (command == "switch_source") {
    irsend.sendRaw(tv_switch_source, sizeof(tv_switch_source) / sizeof(tv_switch_source[0]), 38);
  } else if (command == "1") {
    irsend.sendRaw(tv_channel_1, sizeof(tv_channel_1) / sizeof(tv_channel_1[0]), 38);
  } else if (command == "2") {
    irsend.sendRaw(tv_channel_2, sizeof(tv_channel_2) / sizeof(tv_channel_2[0]), 38);
  } else if (command == "3") {
    irsend.sendRaw(tv_channel_3, sizeof(tv_channel_3) / sizeof(tv_channel_3[0]), 38);
  } else if (command == "4") {
    irsend.sendRaw(tv_channel_4, sizeof(tv_channel_4) / sizeof(tv_channel_4[0]), 38);
  } else if (command == "5") {
    irsend.sendRaw(tv_channel_5, sizeof(tv_channel_5) / sizeof(tv_channel_5[0]), 38);
  } else if (command == "6") {
    irsend.sendRaw(tv_channel_6, sizeof(tv_channel_6) / sizeof(tv_channel_6[0]), 38);
  } else if (command == "7") {
    irsend.sendRaw(tv_channel_7, sizeof(tv_channel_7) / sizeof(tv_channel_7[0]), 38);
  } else if (command == "8") {
    irsend.sendRaw(tv_channel_8, sizeof(tv_channel_8) / sizeof(tv_channel_8[0]), 38);
  } else if (command == "9") {
    irsend.sendRaw(tv_channel_9, sizeof(tv_channel_9) / sizeof(tv_channel_9[0]), 38);
  } else if (command == "10") {
    irsend.sendRaw(tv_channel_10, sizeof(tv_channel_10) / sizeof(tv_channel_10)[0], 38);
  } else if (command == "11") {
    irsend.sendRaw(tv_channel_11, sizeof(tv_channel_11) / sizeof(tv_channel_11[0]), 38);
  } else if (command == "12") {
    irsend.sendRaw(tv_channel_12, sizeof(tv_channel_12) / sizeof(tv_channel_12[0]), 38);
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

  pinMode(LED, OUTPUT);

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

  server.on("/", handleRoot);
  server.on("/light", handleLight);
  server.on("/tv", handleTV);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  ArduinoOTA.handle();

  server.handleClient();

  yield();
}
