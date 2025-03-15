#include <ESP32_Servo.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


Adafruit_MPU6050 mpu;

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
#include <VL53L0X.h>


//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Servo myServo;  // Create a servo object to control a servo

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 433E6

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22 
#define OLED_RST 23
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SDA_PIN 21    // SDA pin for I2C communication
#define SCL_PIN 22    // SCL pin for I2C communication

#define relayPin 25
#define servoPin 00 // Define the servo signal pin


uint16_t distance_mm;
String LoRaMessage = "";
int readingID = 0;
int counter = 0;
float distance = 0;
float acceleration = 0;
float gyration = 0;
float pressure = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);


// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
//#define HIGH_ACCURACY

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);

  pinMode(relayPin, OUTPUT);
  myServo.attach(servoPin); // Attach the servo to the specified pin
  
  Wire.begin(SDA_PIN, SCL_PIN);  // Initialize I2C communication
  
  // Initialize sensor
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }
  
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  
// Lidar Sensor code
#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif

  // Gyroscope code
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);

  // Start continuous measurement
  sensor.startContinuous();

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();
  
  Serial.println("LoRa Sender Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);
}


void loop() {

    // Read distance
  int distance_mm = sensor.readRangeContinuousMillimeters();
  
  // Print distance
  Serial.print("Distance: ");
  Serial.print(distance_mm);
  Serial.println(" mm");

  if(distance_mm < 1000 && distance_mm > 500) {
     digitalWrite(relayPin, HIGH);
      myServo.write(0);
      delay(5); // Adjust delay time for increased speed
     } 
  else 
  {
     digitalWrite(relayPin, LOW);
     myServo.write(90); // Move servo to 90 degrees
     delay(5); // Delay before next measurement
  }
  delay(100); // Delay before next measurement

  /*if (distance_mm < 30) {
    myServo.write(180); // Move servo to 180 degrees
    delay(500); // Adjust delay as needed
    myServo.write(90); // Move servo back to 90 degrees
    delay(500); // Adjust delay as needed
  } else {
    myServo.write(90); // Move servo to 90 degrees
  }
  delay(100); // Delay before next measurement
}*/


  // Gyproscope code
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  

 // Combine readings into one variable (string)
  String acceleration = "X:" +String(a.acceleration.x) + "," + "Y:" + String(a.acceleration.y) + "," + "Z:" + String(a.acceleration.z) + "m/s^2";

  Serial.println(acceleration);


  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

   String gyration = "X:" +String(g.gyro.x) + "," + "Y:" + String(g.gyro.y) + "," + "Z:" + String(g.gyro.z) + "rad/s";

   Serial.println(gyration);

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);
  
  //Send LoRa packet to receiver
  
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("Distance: ");
  LoRa.print(distance_mm);
  LoRa.println(" mm");
  LoRa.print("Acceleration X: ");
  LoRa.print(a.acceleration.x);
  LoRa.print(", Y: ");
  LoRa.print(a.acceleration.y);
  LoRa.print(", Z: ");
  LoRa.print(a.acceleration.z);
  LoRa.println(" m/s^2");

  LoRa.print("Gyration X: ");
  LoRa.print(g.gyro.x);
  LoRa.print(", Y: ");
  LoRa.print(g.gyro.y);
  LoRa.print(", Z: ");
  LoRa.print(g.gyro.z);
  LoRa.println(" rad/s");
  LoRa.endPacket();

  
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LORA SENDER");
  display.setCursor(0,10);
  display.setTextSize(1);
  display.print("LoRa packet sent.");
  display.setCursor(0,20);
  display.setTextSize(0.5);
  display.print("Lidar Sensor:");
  display.print(distance_mm);
  display.setCursor(0,30);
  display.print("Acceleration X: ");
  display.print(a.acceleration.x);
  display.setCursor(0,40);
  display.print(", Y: ");
  display.print(a.acceleration.y);
  display.print(", Z: ");
  display.print(a.acceleration.z);
  display.setCursor(0,50);
  display.println(" m/s^2");
  
  display.display();
  delay(50);
}
