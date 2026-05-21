#include <WiFi.h>
#include <PubSubClient.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#include "wifi_credentials.h"
#include "mqtt_config.h"
#include "graphics.h"
#include "generated/banana.h"

TFT_eSPI tft = TFT_eSPI();
WiFiClient espClient;
PubSubClient client(espClient);


int voltageBuffer[GRAPH_W];   // one value per pixel column
int bufferIndex = 0;
bool bufferFilled = false;


// ---------------------------
// ADD NEW VALUE TO BUFFER
// ---------------------------
void addVoltageToBuffer(int voltage) {
  if (voltage < 220) voltage = 220;
  if (voltage > 250) voltage = 250;

  voltageBuffer[bufferIndex] = voltage;
  bufferIndex++;

  if (bufferIndex >= GRAPH_W) {
    bufferIndex = 0;
    bufferFilled = true;
  }

  drawGraph( bufferFilled, voltageBuffer, bufferIndex);
}


// ---------------------------
// MQTT CALLBACK
// ---------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  String raw;
  for (unsigned int i = 0; i < length; i++) {
    raw += (char)payload[i];
  }

  int voltage = raw.toInt();

  showValue(String(voltage) + " V");
  addVoltageToBuffer(voltage);
}

// ---------------------------
// MQTT RECONNECT
// ---------------------------
void reconnect() {
  while (!client.connected()) {
    if (client.connect("CYD_Voltage_Client")) {
      client.subscribe(MQTT_SUB_TOPIC);
    } else {
      delay(2000);
    }
  }
}

// ---------------------------
// SETUP
// ---------------------------
void setup() {
  Serial.begin(115200);

  // Turn on backlight
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  // tft.setSwapBytes(false);
  tft.fillScreen(TFT_BLACK);

  
  // Draw graph frame
  // tft.drawRect(GRAPH_X, GRAPH_Y, GRAPH_W, GRAPH_H, TFT_WHITE);
  // tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  // tft.setCursor(GRAPH_X, GRAPH_Y - 20);
  // tft.print("Voltage (V)");
  showHelloWorld();


  // sleep(10);

  // tft.setTextSize(2);
  // tft.setCursor(40, 120);        // Center-ish position
  // tft.print("Voltage (V)");


  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }

  // MQTT
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}


// ---------------------------
// LOOP
// ---------------------------
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
