# LoRa-Based ESP32 Transceiver with OLED Display & Web Server
This project implements a LoRa-based data receiver using an ESP32. It collects sensor data from a LoRa transmitter, displays it on an OLED screen, and makes the data accessible via a Wi-Fi web server. The received data includes distance (mm), acceleration (m/s²), gyration (rad/s), and signal strength (RSSI). The project also integrates NTP (Network Time Protocol) to timestamp received data.
## Features
📡 LoRa Communication: Receives real-time data from a LoRa transmitter (SX1278).
🖥️ OLED Display (SSD1306): Shows the received sensor data.
🌐 Wi-Fi Web Server: Provides a web interface for real-time data monitoring.
⏳ NTP Time Synchronization: Fetches timestamps for received packets.
📂 SPIFFS File System: Serves an index.html page to display the data.
📊 JSON API Endpoint: Allows external devices to fetch data in JSON format.
