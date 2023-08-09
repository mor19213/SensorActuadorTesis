
import time
import network

def conectar_wifi():
    sta_if = network.WLAN(network.STA_IF)
    sta_if.active(True)  # Activate the station interface
    if not sta_if.isconnected():
        print("conectando")
        sta_if.active(True)
        print(sta_if.scan())       
        sta_if.connect("Pellecer_Inadria", "fans123")
    if sta_if.isconnected():
        print("conectado")
        return True 
    else:
        print("no conectado")
        return False 

class coso_udp():
    def __init__(self, ip, port):
        import usocket
        self.ip = ip
        self.port = port
        self.socket = usocket.socket(usocket.AF_INET, usocket.SOCK_DGRAM)  # create a UDP socket
    
    def send(self, message):
        self.socket.sendto(message.encode(), (self.ip, self.port))



