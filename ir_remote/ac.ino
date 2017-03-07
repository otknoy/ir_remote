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
