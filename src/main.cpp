#include <M5Core2.h>
#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "Freebox-C2F799";
const char* password = "";
const char* server_ip = "192.168.1.73";
const int server_port = 12345;

WiFiClient client;

float cpu_percent = 0;
float memory_percent = 0;
float disk_percent = 0;

void setup() {
  M5.begin();
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("\nConnected to WiFi");
}

void drawUI() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("System Monitor");
  M5.Lcd.drawRect(10, 40, 300, 30, WHITE);  // CPU bar
  M5.Lcd.drawRect(10, 100, 300, 30, WHITE);  // Memory bar
  M5.Lcd.drawRect(10, 160, 300, 30, WHITE);  // Disk bar
  
  // CPU usage bar
  M5.Lcd.fillRect(12, 42, 296 * cpu_percent / 100.0, 26, RED);
  M5.Lcd.setCursor(10, 75);
  M5.Lcd.printf("CPU: %.1f%%", cpu_percent);
  
  // Memory usage bar
  M5.Lcd.fillRect(12, 102, 296 * memory_percent / 100.0, 26, GREEN);
  M5.Lcd.setCursor(10, 135);
  M5.Lcd.printf("Memory: %.1f%%", memory_percent);
  
  // Disk usage bar
  M5.Lcd.fillRect(12, 162, 296 * disk_percent / 100.0, 26, BLUE);
  M5.Lcd.setCursor(10, 195);
  M5.Lcd.printf("Disk: %.1f%%", disk_percent);
}

void updateMetrics() {
  if (!client.connected()) {
    M5.Lcd.println("Connecting to server...");
    if (client.connect(server_ip, server_port)) {
      M5.Lcd.println("Connected to server");
    } else {
      M5.Lcd.println("Connection failed");
      delay(5000);
      return;
    }
  }

  if (client.available()) {
    String line = client.readStringUntil('\n');
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, line);

    cpu_percent = doc["cpu_percent"].as<float>();
    memory_percent = doc["memory_percent"].as<float>();
    disk_percent = doc["disk_percent"].as<float>();

    drawUI();
  }
}

void loop() {
  updateMetrics();
  delay(1000);  // Update every second
}
