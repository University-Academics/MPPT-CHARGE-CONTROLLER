void WIFI_init() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  if (client.connect("ESP8266Client", "mppt-solar", "sem4electronics@entc19")) {
    Serial.println("connected");
    PrevConnectionStatus = true;
    // Subscribe
    //client.subscribe("esp32/output");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" System Will Try to Reconnect in 5s");
  }
}



void UPDATE_CONNECTED_DEVICE() {
  if (!client.connected() && abs((int)CurrentTime - DisconnectedTime) > WifiReconnectionInterval)
    reconnect();
  if (PrevConnectionStatus && !client.connected()) {
    DisconnectedTime = CurrentTime;
    PrevConnectionStatus = false;
  }
}