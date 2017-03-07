// light
unsigned int light_on[83]  = {3600, 1550, 550, 300, 550, 350, 500, 1200, 500, 1200, 550, 350, 500, 1200, 500, 350, 500, 350, 500, 350, 550, 1200, 500, 350, 500, 350, 500, 1200, 500, 400, 500, 1200, 500, 350, 500, 1200, 500, 400, 450, 400, 500, 1200, 500, 350, 500, 400, 450, 400, 450, 400, 500, 350, 500, 350, 500, 1200, 500, 1250, 500, 350, 500, 1200, 500, 400, 450, 400, 450, 1250, 500, 400, 450, 1250, 450, 400, 450, 400, 500, 1200, 500, 400, 450, 400, 450};  // UNKNOWN CA21A230
unsigned int light_off[83] = {
	 3600, 1600, 550, 300, 550, 300, 550, 1150, 550, 1200, 550, 300, 550, 1200, 500, 350, 500, 350, 500, 350, 500, 1200, 550, 350, 500, 350, 500, 1200, 500, 350, 500, 1250, 500, 350, 500, 1200, 500, 350, 500, 350, 500, 1250, 500, 350, 500, 350, 500, 350, 500, 350, 500, 1250, 500, 1200, 500, 1200, 500, 1250, 500, 350, 500, 1200, 500, 350, 500, 400, 500, 350, 500, 1200, 500, 1200, 500, 400, 500, 350, 500, 1200, 500, 350, 500, 350, 500};  // UNKNOWN 1B9C2A92

void handleLight() {
  if (server.method() != HTTP_GET) {
    jsonResponse(400, "error");
    return;
  }

  IRsend irsend(IR);
  irsend.begin();

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