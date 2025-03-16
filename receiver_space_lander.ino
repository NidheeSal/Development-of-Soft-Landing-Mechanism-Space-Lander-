// Import Wi-Fi library
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <SPIFFS.h>

// Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

// Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Libraries to get time from NTP Server
#include <NTPClient.h>
#include <WiFiUdp.h>

// Libraries for JSON data
#include <ArduinoJson.h>

// Define pins
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define BAND 433E6

#define OLED_SDA 21
#define OLED_SCL 22 
#define OLED_RST 23
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Replace with your network credentials
const char* ssid     = "AAAAAA";
const char* password = "000000";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String formattedDate;
String day;
String hour;
String timestamp;

int rssi;
String distance_mm;
String acceleration;
String gyration;
String LoRaData;

AsyncWebServer server(80);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void initOLED() {
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("LORA RECEIVER");
}

void initLoRA() {
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  int counter = 0;
  while (!LoRa.begin(BAND) && counter < 10) {
    Serial.print(".");
    counter++;
    delay(500);
  }
  if (counter == 10) {
    Serial.println("Starting LoRa failed!");
  }
  Serial.println("LoRa Initialization OK!");
  display.setCursor(0, 10);
  display.clearDisplay();
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);
}

void connectWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.setCursor(0, 20);
  display.print("Access web server at: ");
  display.setCursor(0, 30);
  display.print(WiFi.localIP());
  display.display();
}

void getLoRaData() {
  Serial.print("Lora packet received: ");
  while (LoRa.available()) {
    LoRaData = LoRa.readString();
    int distanceIndex = LoRaData.indexOf("e:");
    if (distanceIndex != -1) {
      distance_mm = LoRaData.substring(distanceIndex + 2, LoRaData.indexOf("mm"));
    }
    int accelerationIndex = LoRaData.indexOf("Acceleration X:");
    if (accelerationIndex != -1) {
      int mpsIndex = LoRaData.indexOf("m/s^2", accelerationIndex);
      if (mpsIndex != -1) {
        acceleration = LoRaData.substring(accelerationIndex + 15, mpsIndex);
      }
    }
    int gyrationIndex = LoRaData.indexOf("Gyration X:");
    if (gyrationIndex != -1) {
      int dpsIndex = LoRaData.indexOf("rad/s", gyrationIndex);
      if (dpsIndex != -1) {
        gyration = LoRaData.substring(gyrationIndex + 11, dpsIndex);
      }
    }
    Serial.print(LoRaData);
    Serial.print(distance_mm);
    Serial.println(acceleration);
    Serial.println(gyration);

    display.clearDisplay();
    display.setTextSize(0.5);
    display.setCursor(0, 0);
    display.print("LORA RECEIVER");
    display.setCursor(0, 10);
    display.print(LoRaData);
    display.display();
    delay(5000);
  }
  rssi = LoRa.packetRssi();
  Serial.print(" with RSSI ");
  Serial.println(rssi);
}

void getTimeStamp() {
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);

  int splitT = formattedDate.indexOf("T");
  day = formattedDate.substring(0, splitT);
  Serial.println(day);
  hour = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
  Serial.println(hour);
  timestamp = day + " " + hour;
}

String processor(const String& var) {
  if (var == "LIDAR") {
    return distance_mm;
  } else if (var == "GYRO") {
    return acceleration;
  } else if (var == "RO") {
    return gyration;
  } else if (var == "RRSI") {
    return String(rssi);
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  initOLED();
  initLoRA();
  connectWiFi();

  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
  });
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<200> doc;
    doc["distance_mm"] = distance_mm;
    doc["acceleration"] = acceleration;
    doc["gyration"] = gyration;
    doc["timestamp"] = timestamp;
    doc["rssi"] = rssi;
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
  });
  server.begin();

  timeClient.begin();
  timeClient.setTimeOffset(0);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    getLoRaData();
    getTimeStamp();
  }
}
