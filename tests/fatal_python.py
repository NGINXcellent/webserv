import socket
import sys
import time

def simulate_fatal_crash():
    # Simulate a fatal crash by raising an exception
    raise RuntimeError("Simulated fatal crash")

def main():
    # Set the server address and port
    server_address = ('localhost', 8080)

    try:
        # Create a socket
        with socket.create_connection(server_address) as sock:
            print(f"Connected to {server_address}")

            # Simulate data transmission
            data_to_send = b"Hello, server!"
            sock.sendall(data_to_send)
            print(f"Sent data: {data_to_send.decode('utf-8')}")

            # Simulate a fatal crash during transmission
            simulate_fatal_crash()

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
