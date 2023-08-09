import bluetooth

def pair_device(device_name):
    nearby_devices = bluetooth.discover_devices(duration=8, lookup_names=True, lookup_class=True)

    for address, name, device_class in nearby_devices:
        if name == device_name:
            print(f"Found device: {name} ({address})")
            try:
                # Attempt to pair with the device
                bluetooth.pair(address)
                print(f"Successfully paired with {name} ({address})")
                return
            except Exception as e:
                print(f"Pairing failed: {e}")
                return

    print(f"Device '{device_name}' not found nearby.")

if __name__ == "__main__":
    device_name_to_pair = "ESP32test"  # Replace with the name of your target device
    pair_device(device_name_to_pair)
