import mip
import network

sta_if = network.WLAN(network.STA_IF)
if not sta_if.isconnected():
    sta_if.active(True)
    sta_if.scan()                              # Scan for available access points
    sta_if.connect("Casa", "Clave123")         # Connect to an AP
if sta_if.isconnected():
    print(mip.install("https://github.com/insighio/microCoAPy/blob/master/microcoapy/__init__.py"))
else:
    print("no se conecto a internet")