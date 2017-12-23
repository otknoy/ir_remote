#include <WiFi.h>

#include "Request.h"

class MyServer {
private:
  WiFiServer server;
  const int port;
  
 public:
  MyServer(const int port): 
    port(port), server(port) {}
  void begin();
  void handle_request();
};

void MyServer::begin() {
  this->server.begin();
}

void MyServer::handle_request() {
  WiFiClient client = this->server.available();
  if (!client) {
    return;
  }

 client.println("hello, world!");

 client.stop();
}
