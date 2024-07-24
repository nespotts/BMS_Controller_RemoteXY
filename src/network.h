
char ssid[] = "GL-AX1800-50f";
char pass[] = "SpottsHotSpot";

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  //  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void ConnectWifi() {
  // Serial.print("Attempting to connect to SSID: ");
  // Serial.println(WiFi.SSID());
  WiFi.persistent(false);  // May be able to set to true disables automatic saving of wifi credentials
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  // Blynk.connect();

  int t1 = millis();
  while (!WiFi.isConnected()) {  // was !Blynk.connected()
    int t2 = millis();
    delay(500);
    // Serial.print(".");
    if ((t2 - t1) > 10000) {
      ESP.wdtDisable();
      while (true) {};
    }
  }

  // Serial.println("\nConnected to wifi");
  printWifiStatus();
}

void manageBlynkConnection2() {
  if (!WiFi.isConnected()) {    // changed from !Blynk.connected()
    // Serial.println("Wifi Disconnected");
    led_off.Update();
    ConnectWifi();
  }
}