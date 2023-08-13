#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

const int analogPin = 34;
String sensorName = "sensor1"; 

BluetoothSerial SerialBT;


void onBTConnect(esp_spp_cb_event_t event, esp_spp_cb_param_t* param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    // Send the array of strings when a device connects
    SerialBT.println("Conectado"); // Sending newline for clarity
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(analogPin, INPUT);
  SerialBT.begin("DispPot1"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  SerialBT.register_callback(onBTConnect);
}

void loop() {
  int potValue = analogRead(analogPin);

  String message  = "sensor/" + sensorName + "/" + String(potValue) +"\n";
  // Forward data from Serial to SerialBT
  SerialBT.println(message); // Sending newline for clarity

  // Forward data from SerialBT to Serial
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }

  delay(1000);
}
