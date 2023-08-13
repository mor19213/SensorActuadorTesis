#include "BluetoothSerial.h"
#include <ArduinoJson.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

const int ledPin = 23;

BluetoothSerial SerialBT;
String receivedString = "";
StaticJsonDocument<200> jsonDoc;
String msg;


void onBTConnect(esp_spp_cb_event_t event, esp_spp_cb_param_t* param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    // Send the array of strings when a device connects
    SerialBT.println("Conectado"); // Sending newline for clarity
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  SerialBT.begin("DispEspActuador"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  SerialBT.register_callback(onBTConnect);
}

void loop() {
  digitalWrite(ledPin, HIGH);

  // Forward data from Serial to SerialBT
  SerialBT.println("actuador/led amarillo\n"); // Sending newline for clarity

  // Forward data from SerialBT to Serial
  while (SerialBT.available()) {
    char data = SerialBT.read();
    receivedString += data;
    
  }
  if (!receivedString.isEmpty()) {
    Serial.println("Received: " + receivedString);
     DeserializationError error = deserializeJson(jsonDoc, receivedString); 
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

    receivedString = ""; // Clear the received string
  }

  delay(3000);
}
