
import time
import usocket as socket

def conectar_wifi():
    import network
    sta_if = network.WLAN(network.STA_IF)
    sta_if.active(True)  # Activate the station interface
    while not sta_if.isconnected():
        print("conectando")
        sta_if.active(True)
        sta_if.scan()          
        sta_if.connect("Pellecer_Inadria", "fans123")  # Connect to an AP
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
        sock = usocket.socket(usocket.AF_INET, usocket.SOCK_DGRAM)  # create a UDP socket
        sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))  # send the message to the destination
    
    def send(self, message):
        self.socket.sendto(message.encode(), (self.ip, self.port))



