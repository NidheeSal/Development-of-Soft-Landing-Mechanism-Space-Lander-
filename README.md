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
- LilyGO TTGO T3 LoRa32 433MHz V1.1.6 ESP32 with a built-in LoRa chip and an OLED display (128x64 pixels)
- 2 Pole 5V Single Channel Relay Module
- I2C Logic Level Converter 4 Channel Bi-directional Module
- Gyroscope Sensor MPU6050 
- VL53L0X TOF Based Lidar Lasar Distance Sensor
- 3.7V 6000 RPM Mini Drone DC Motor Shaft 
- Wi-Fi Connection

## 🛠️ Software Requirements
- Arduino IDE 
  
## 🔌  LORA ESP32 TTGO LoRa T3 V1.6 Pin Diagram
![Alt text](https://www.tinytronics.nl/image/cache/catalog/products_2022/LilyGO-TTGO-T3-LoRa32-433MHz-V2.1.6-ESP32-pinout-600x600w.jpg)

## Circuit Diagram
![Alt text](circuit_diagram.png)

## ⚙️ Installation & Setup  
1. **Install Required Libraries:**
   *Transmmiter*
   - VL53L0X
   - ESP32_Servo
   - Adafruit_MPU6050
   - Adafruit_Sensor
   - Wire
   - SPI
   - LoRa
   - Adafruit GFX
   - Adafruit SSD1306
 
   *Receiver* 
   - WiFi  
   - ESPAsyncWebServer  
   - SPI
   - SPIFFS  
   - LoRa  
   - Wire  
   - Adafruit GFX  
   - Adafruit SSD1306  
   - NTPClient
   - WiFiUdp 
   - ArduinoJson
  
   
  

3. **Update Wi-Fi Credentials in the Code:**  
   ```cpp
   const char* ssid = "YourWiFiSSID";
   const char* password = "YourWiFiPassword";

4. **Upload the Code to ESP32 using Arduino IDE**
## 📡 How It Works
1.**The LoRa receiver listens for packets from a LoRa transmitter.**
2.**Extracts distance (mm), acceleration (m/s²), and gyration (rad/s).**
3.**Displays the data on an OLED screen.**
4.**Connects to Wi-Fi and starts a web server.**
5.**Provides real-time JSON data via an API endpoint.**

