import socket

def receive_udp_datagrams(host, port):
    # Create a UDP socket
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket to the specified host and port
    udp_socket.bind((host, port))

    print(f"Listening for UDP datagrams on {host}:{port}...")

    try:
        while True:
            # Receive data from the socket
            data, address = udp_socket.recvfrom(1024)
            message = data.decode('utf-8')
            print(f"Received datagram from {address[0]}:{address[1]}: {message}")

    except KeyboardInterrupt:
        print("UDP receiver stopped.")

if __name__ == "__main__":
    # Replace these values with your desired host and port
    host = '0.0.0.0'  # Listens on all available network interfaces
    port = 12345

    receive_udp_datagrams(host, port)
