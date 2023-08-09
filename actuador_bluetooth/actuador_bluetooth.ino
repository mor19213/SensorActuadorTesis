#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

const int ledPin = 23;

BluetoothSerial SerialBT;

// Define an array of strings to be sent when a device connects
const char* stringsToSend[] = {
  "sensor/joystick y",
  "sensor/joystick x",
  "actuador/led",
  "actuador/led amarillo"
};

void onBTConnect(esp_spp_cb_event_t event, esp_spp_cb_param_t* param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    // Send the array of strings when a device connects
    for (const char* str : stringsToSend) {
    SerialBT.print(str);
    SerialBT.print(",");
    //delay(100); // Add a delay between sending each string
  }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  SerialBT.begin("sensorasdfactuador"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  SerialBT.register_callback(onBTConnect);
}

void loop() {
  digitalWrite(ledPin, HIGH);

  // Forward data from Serial to SerialBT
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }

  // Forward data from SerialBT to Serial
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }

  delay(20);
  digitalWrite(ledPin, LOW);
  delay(20);
}
