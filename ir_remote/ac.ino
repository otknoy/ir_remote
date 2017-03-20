void handleAC() {
  if (server.method() != HTTP_GET) {
    jsonResponse(400, "error");
    return;
  }

  IRDaikinESP dakinir(IR);

  /* dakinir.setFan(1); */
  /* dakinir.setMode(DAIKIN_COOL); */
  /* dakinir.setTemp(25); */
  /* dakinir.setSwingVertical(0); */
  /* dakinir.setSwingHorizontal(0); */

  String power = server.arg("power");
  if (power == "on") {
    dakinir.on();
  } else if (power == "off") {
    dakinir.off();
  } else {
    jsonResponse(400, "invalid parameter (power=" + power + ")");
    return;
  }

  dakinir.send();

  jsonResponse(200, "success (power=" + power + ")");
}
