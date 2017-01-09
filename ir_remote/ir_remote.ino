#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include <IRDaikinESP.h>

#include "wifi_config.h"
#include "signal.h"

#define LED 12
#define IR 5

ESP8266WebServer server(80);

IRsend irsend(IR);

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

  const unsigned int hlz = 38;
  const unsigned int size = 83;

  String power = server.arg("power");
  if (power == "on") {
    irsend.sendRaw(light_on, size, hlz);
  } else if (power == "off") {
    irsend.sendRaw(light_off, size, hlz);
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

  const unsigned int hlz = 38;
  const unsigned int size = 67;

  String command = server.arg("command");
  if (command == "power") {
    irsend.sendRaw(tv_power, size, hlz);
  } else if (command == "switch_source") {
    irsend.sendRaw(tv_switch_source, size, hlz);
  } else if (command == "volume_up") {
    irsend.sendRaw(tv_volume_up, size, hlz);
  } else if (command == "volume_down") {
    irsend.sendRaw(tv_volume_down, size, hlz);
  } else if (command == "channel_next") {
    irsend.sendRaw(tv_channel_next, size, hlz);
  } else if (command == "channel_previous") {
    irsend.sendRaw(tv_channel_previos, size, hlz);
  } else if (command == "channel_1") {
    irsend.sendRaw(tv_channel_1, size, hlz);
  } else if (command == "channel_2") {
    irsend.sendRaw(tv_channel_2, size, hlz);
  } else if (command == "channel_3") {
    irsend.sendRaw(tv_channel_3, size, hlz);
  } else if (command == "channel_4") {
    irsend.sendRaw(tv_channel_4, size, hlz);
  } else if (command == "channel_5") {
    irsend.sendRaw(tv_channel_5, size, hlz);
  } else if (command == "channel_6") {
    irsend.sendRaw(tv_channel_6, size, hlz);
  } else if (command == "channel_7") {
    irsend.sendRaw(tv_channel_7, size, hlz);
  } else if (command == "channel_8") {
    irsend.sendRaw(tv_channel_8, size, hlz);
  } else if (command == "channel_9") {
    irsend.sendRaw(tv_channel_9, size, hlz);
  } else if (command == "channel_10") {
    irsend.sendRaw(tv_channel_10, size, hlz);
  } else if (command == "channel_11") {
    irsend.sendRaw(tv_channel_11, size, hlz);
  } else if (command == "channel_12") {
    irsend.sendRaw(tv_channel_12, size, hlz);
  } else if (command == "list") {
    irsend.sendRaw(tv_list, size, hlz);
  } else if (command == "rec_list") {
    irsend.sendRaw(tv_rec_list, size, hlz);
  } else if (command == "back") {
    irsend.sendRaw(tv_back, size, hlz);
  } else if (command == "end") {
    irsend.sendRaw(tv_end, size, hlz);
  } else if (command == "enter") {
    irsend.sendRaw(tv_enter, size, hlz);
  } else if (command == "u") {
    irsend.sendRaw(tv_u, size, hlz);
  } else if (command == "d") {
    irsend.sendRaw(tv_d, size, hlz);
  } else if (command == "l") {
    irsend.sendRaw(tv_l, size, hlz);
  } else if (command == "r") {
    irsend.sendRaw(tv_r, size, hlz);
  } else if (command == "uu") {
    irsend.sendRaw(tv_uu, size, hlz);
  } else if (command == "dd") {
    irsend.sendRaw(tv_dd, size, hlz);
  } else if (command == "ll") {
    irsend.sendRaw(tv_ll, size, hlz);
  } else if (command == "rr") {
    irsend.sendRaw(tv_rr, size, hlz);
  } else if (command == "blue") {
    irsend.sendRaw(tv_blue, size, hlz);
  } else if (command == "red") {
    irsend.sendRaw(tv_red, size, hlz);
  } else if (command == "green") {
    irsend.sendRaw(tv_green, size, hlz);
  } else if (command == "yellow") {
    irsend.sendRaw(tv_yellow, size, hlz);
  } else {
    jsonResponse(400, "invalid parameter (command=" + command + ")");
    return;
  }

  jsonResponse(200, "success (command=" + command + ")");
}

void handleAC() {
  if (server.method() != HTTP_GET) {
    jsonResponse(400, "error");
    return;
  }

  IRDaikinESP dakinir(IR);

  dakinir.off();
  /* dakinir.setFan(1); */
  /* dakinir.setMode(DAIKIN_COOL); */
  /* dakinir.setTemp(25); */
  /* dakinir.setSwingVertical(0); */
  /* dakinir.setSwingHorizontal(0); */
  dakinir.send();

  delay(2000);

  dakinir.on();
  dakinir.send();

  jsonResponse(200, "success (acpower=on)");
}

void setup() {
  irsend.begin();
  Serial.begin(115200);

  WiFi.config(IPAddress(192,168,1,60),IPAddress(192,168,1,1),IPAddress(255,255,255,0));

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
