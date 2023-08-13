#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

const char* ssid = "Pellecer_Inadria";
const char* password = "fans1234";
const char* udpServerIP = "192.168.0.122"; // Replace with the IP address of your UDP server
const int udpServerPort = 5005; // IP del gw
const int ledPin = 23; // digital output pin
unsigned long previousMillis = 0;
const unsigned long interval = 4000; // 0.4 seconds

WiFiUDP udp;
StaticJsonDocument<200> jsonDoc;
String jsonString;
bool value;
String msg;
String actuadorName = "led azul"; 

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

  // Begin UDP communication
  udp.begin(udpServerPort);
  Serial.print("UDP started on local port");

  jsonString = "";
}

void loop() {

  int packetSize = udp.parsePacket();
  if (packetSize){
    char buffer[255];
    int len = udp.read(buffer, 255);
    if (len > 0){
      buffer[len] = 0;
      Serial.println(buffer);
      DeserializationError error = deserializeJson(jsonDoc, buffer); 
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
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    String message  = "actuador/" + actuadorName + "/";
    Serial.println(message);
    udp.beginPacket(udpServerIP, udpServerPort);
    udp.print(message);
    udp.endPacket();
  }
}
