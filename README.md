# 🚀 LoRa-Based ESP32 TTGO LoRa T3 V1.6 Transceiver with OLED & Web Server  

## Overview:
This project implements a LoRa-based wireless data receiver using the ESP32 TTGO LoRa T3 V1.6 module. It receives sensor data over LoRa, displays it on an **OLED screen**, and hosts a **web-based dashboard** where users can monitor real-time data using a web browser.

## 📌 Features  
- 📡 **Reception** - Receives sensor data from a remote LoRa transmitter.  
- 🖥️ **OLED Display (SSD1306)** – Displays received data.  
- 🌐 **Wi-Fi Web Server** – Access data remotely.  
- ⏳ **NTP Time Sync** – Fetches timestamps for received packets.  
- 📊 **JSON API Endpoint** – Exposes sensor data in JSON format.  

## 🛠️ Hardware Requirements  
- ESP32 TTGO LoRa v1.6
- SX1278 LoRa Module (433 MHz)
- SSD1306 OLED Display (128x64 pixels)
- Wi-Fi Connection  

## 🔌 Wiring Connections  
### LoRa Module (SX1278)  
| **ESP32 Pin** | **LoRa Pin** |  
|--------------|------------|  
| 5  | SCK  |  
| 19 | MISO |  
| 27 | MOSI |  
| 18 | NSS  |  
| 14 | RST  |  
| 26 | DIO0 |  

### OLED Display (SSD1306)  
| **ESP32 Pin** | **OLED Pin** |  
|--------------|------------|  
| 21 | SDA  |  
| 22 | SCL  |  
| 23 | RST  |  

## ⚙️ Installation & Setup  
1. **Install Required Libraries:**  
   - WiFi  
   - ESPAsyncWebServer  
   - SPIFFS  
   - LoRa  
   - Wire  
   - Adafruit GFX  
   - Adafruit SSD1306  
   - NTPClient  
   - ArduinoJson  

2. **Update Wi-Fi Credentials in the Code:**  
   ```cpp
   const char* ssid = "YourWiFiSSID";
   const char* password = "YourWiFiPassword";

3. **Upload the Code to ESP32 using Arduino IDE**
## 📡 How It Works
1.**The LoRa receiver listens for packets from a LoRa transmitter.**
2.**Extracts distance (mm), acceleration (m/s²), and gyration (rad/s).**
3.**Displays the data on an OLED screen.**
4.**Connects to Wi-Fi and starts a web server.**
5.**Provides real-time JSON data via an API endpoint.**

