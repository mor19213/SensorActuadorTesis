#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Pellecer_Inadria";
const char* password = "fans1234";
const char* udpServerIP = "192.168.0.122"; // Replace with the IP address of your UDP server
const int udpServerPort = 5005; // Replace with the port number of your UDP server

const int analogPin = 34;
int potValue;
String sensorName = "temperatura"; 

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  pinMode(analogPin, INPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Begin UDP communication
  udp.begin(udpServerPort);
  Serial.print("UDP started on local port");

}

void loop() {
  int potValue = analogRead(analogPin);
  String message  = "sensor/" + sensorName + "/" + String(potValue);
  Serial.println(message);
  // Send UDP datagram
  udp.beginPacket(udpServerIP, udpServerPort);
  udp.print(message);
  udp.endPacket();


  delay(1000); // Send UDP datagram every 5 seconds
}
