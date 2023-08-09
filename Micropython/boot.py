from machine import Pin
import bluetooth
from BLE import BLEUART

led = Pin(2, Pin.OUT)
led.value(1)

name = 'ESP32'
ble = bluetooth.BLE()
uart = BLEUART(ble, name)

def on_rx():
    rx_buffer = uart.read().decode('utf-8').strip()
    uart.write('ESP32 received: ' + str(rx_buffer) + '\n')

    if rx_buffer == 'on':
        led.value(0)
    elif rx_buffer == 'off':
        led.value(1)

uart.irq(handler=on_rx)
