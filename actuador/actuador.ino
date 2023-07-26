#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Pellecer_Inadria";
const char* password = "fans1234";
const char* mqtt_server = "192.168.0.122"; // IP del gw
const int ledPin = 4; // digital output pin
unsigned long previousMillis = 0;
const unsigned long interval = 800; // 0.4 seconds

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<200> jsonDoc;
String jsonString;
bool value;
String msg;

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Null-terminate the payload
  // Parse the received JSON payload
  DeserializationError error = deserializeJson(jsonDoc, payload);

  // Check for parsing errors
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Extract the value from the JSON payload
  if (jsonDoc.containsKey("valor")) {
    msg = jsonDoc["valor"].as<String>();
    Serial.print("Received value: ");
    Serial.println(msg);
    
    
    if (msg == "True") {
      // Turn on the LED
      digitalWrite(ledPin, HIGH);
      Serial.println("LED turned on");
    } else {
      digitalWrite(ledPin, LOW);
      Serial.println("LED turned off");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); 
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
  client.setCallback(callback); // Set the callback function
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected!");
      client.subscribe("actuador/led", 0);
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
      client.subscribe("actuador/led", 0); // Resubscribe to the topic
    }
  }

  // Maintain MQTT connection and handle incoming messages
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Send the message to the "actualizar/" topic
    String message = "led";
    client.publish("actualizar/", message.c_str());
    Serial.println("Message sent to topic 'actualizar/'");
  }
}
