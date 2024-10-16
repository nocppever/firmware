import psutil
import time
import socket
import json

class SystemMonitor:
    def __init__(self):
        self.cpu_percent = 0
        self.memory_percent = 0
        self.disk_percent = 0

    def update_metrics(self):
        self.cpu_percent = psutil.cpu_percent()
        self.memory_percent = psutil.virtual_memory().percent
        self.disk_percent = psutil.disk_usage('/').percent

    def get_metrics(self):
        return {
            'cpu_percent': self.cpu_percent,
            'memory_percent': self.memory_percent,
            'disk_percent': self.disk_percent
        }

def start_server(monitor):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('0.0.0.0', 12345))  # Listen on all available interfaces
    server_socket.listen(1)

    print("Server listening on port 12345")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Connection from {addr}")

        while True:
            try:
                monitor.update_metrics()
                metrics = monitor.get_metrics()
                json_data = json.dumps(metrics)
                client_socket.send(json_data.encode() + b'\n')
                time.sleep(1)  # Send data every second
            except:
                break

        client_socket.close()

if __name__ == "__main__":
    monitor = SystemMonitor()
    start_server(monitor)