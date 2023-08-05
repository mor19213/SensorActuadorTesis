import machine
import time
from helpers import *

conectar_wifi()
pot = machine.ADC(machine.Pin(34))
led = machine.Pin(2, machine.Pin.OUT)

while True:
    print(pot.read())
    led.value(1)
    time.sleep(1)
    led.value(0)
    time.sleep(1)