#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Pellecer_Inadria";
const char* password = "fans1234";
const char* mqtt_server = "192.168.0.122"; // IP del gw
const int analogPin = 34;
int potValue = 0;

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<200> jsonDoc;
String jsonString;
int value = 0;

void setup() {
  Serial.begin(115200);
  pinMode(analogPin, INPUT);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to Wi-Fi, IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT broker
  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
  
  jsonString = "";
}

void loop() {
  if (!client.connected()) {
    // Reconnect to MQTT broker if the connection is lost
    if (client.connect("ESP32Client")) {
      Serial.println("Reconnected to MQTT broker");
    }
  }
  
  potValue = analogRead(analogPin);
  Serial.println(potValue);

  jsonDoc["valor"] = String(potValue);
  serializeJson(jsonDoc, jsonString);
  client.publish("sensor/sensor3", jsonString.c_str());
  Serial.println(jsonString);

  jsonString = "";
  delay(2000);
}
