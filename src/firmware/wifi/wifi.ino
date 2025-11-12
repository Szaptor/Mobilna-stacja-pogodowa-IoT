//Biblioteki
//Adafruit_SSD1306 2.5.15
//Adafruit-GFX-Library 1.12.4
//Adafruit_BusIO 1.17.4
//Adafruit Unified Sensor 1.1.15
//ArduinoJson 7.4.2
//DHT-sensor-library 1.4.6
//DallasTemperature 4.0.5
//MQUnifiedsensor 3.0.0
//OneWire Library2.3.8
//PubSubClient 2.8

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// --- DS18B20 sensor tempDS ---
#include <OneWire.h> // For communication with sensor via digital on 1 wire of sensor
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4  // Digital Pin where the DS18B20 is connected
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature  DS18B20(&oneWire);


// --- DHT22 sensor humDHT + tempDS ---
#include <DHT.h>
#define DHTPIN 11      // Digital Pin connected to the DHT22 (humDHT) data pin
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// --- MQ-3 dfrobot sensor alcohol/gas ---
#include <MQUnifiedsensor.h>
#define Board "ESP32-S3"
#define Pin 8                  // ADC pin you connected the sensor to
#define Type "MQ-7"    // sensor type
#define Voltage_Resolution 3.3 // power voltage
#define ADC_Bit_Resolution 12 // sensor bit resolution
#define RatioMQ7CleanAir 27.5 // Rs/R0 w czystym powietrzu

// RL wartość (zależna od modułu)
#define ResistanceLoad 10000  // 10 kΩ RL on most boards
MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);

// --- SSD1306 OLED connected to I2C (SDA, SCL pins) ---
#include <Wire.h> //I2C
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1 // No reset pin so -1
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define SDA_PIN 6
#define SCL_PIN 7


// ==== WIFI CONFIG ====
const char* ssid = "Note 12 Pro";
const char* password = "123456789";

// ==== MQTT CONFIG ====
const char* mqtt_server = "iot.marekurbaniak.pl";  // or IP address
const int mqtt_port = 1883;                         // TCP port (not TLS)
const char* mqtt_user = "WeatherStation";       // optional
const char* mqtt_pass = "iot2025projekt";       // optional
const char* mqtt_topic = "v1/devices/me/telemetry";        // your topic name

// ==== DATA ====
float tempDS = 23.55;
float humDHT = 55.25;
float coPpm = 1.45;

// ==== CLIENTS ====
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Test communication with OLED via I2C
    Serial.println("SSD1306 allocation failed"); 
      for(;;){;}
  }
  //--- Start and cofigure display ---
  OLED.display();
  delay(1000);
  OLED.clearDisplay();
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0, 10);
  OLED.println("Start...");
  OLED.display();
  Serial.println("Start...");

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("Connecting to ");
  OLED.setCursor(0, 20);
  OLED.println(ssid);
  OLED.display();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("WiFi connected");
  OLED.setCursor(0, 20);
  OLED.println("IP: ");
  OLED.print(WiFi.localIP());
  OLED.display();

   dht.begin(); // Start communication with DHT22

   // --- Gas sensor config ---
  MQ7.setRegressionMethod(1); // 1 = Exponential (dla MQ-3)
  MQ7.setA(99.042);           // współczynnik z krzywej z dokumentacji
  MQ7.setB(-1.518);           // współczynnik z krzywej z dokumentacji
  MQ7.init();

  float calcR0 = 0;
  for (int i = 0; i < 10; i++) {
    MQ7.update();
    calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
    delay(500);
    
  }
  MQ7.setR0(calcR0 / 10);

  DS18B20.begin(); // Start communication with DS18B20


  
  delay(1000);
}

void reconnect() {
  // Loop until connected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create client ID
    String clientId = "SIMA7670E";

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // Subscribe if needed:
      // client.subscribe("some/topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

String publishState="";
void publishData() {
  
  // --- Read humDHT and tempDS readings from DHT22
  // tempDS = dht.readTemperature(); // read DHT22 temp. - C by default //??
  humDHT = dht.readHumidity(); // read DHT22 humDHT - % by default
  humDHT = roundf(humDHT * 100) / 100.0;
  Serial.println(humDHT);
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("Temp: "+(String)tempDS);
  // --- Read tempDS readings from DS18B20
  DS18B20.requestTemperatures(); // Send command to get tempDSs
  tempDS = DS18B20.getTempCByIndex(0); // Read the first sensor
  tempDS = roundf(tempDS * 100) / 100.0;
  Serial.println(tempDS);
  OLED.setCursor(0, 20);
  OLED.println("Humidity: "+(String)humDHT);
   // --- Gas/alcohol level readings from MQ3 ---
  MQ7.update();                // odczyt nowej próbki
  coPpm = MQ7.readSensor(); // automatyczne przeliczenie
  coPpm = roundf(coPpm * 100) / 100.0;
  Serial.println(coPpm);
  OLED.setCursor(0, 30);
  OLED.println("PPM CO: "+(String)coPpm);
  OLED.display();
  delay(1000);
  // Build JSON payload
  if (isnan(tempDS) || isnan(humDHT) || isnan(coPpm)) {
    Serial.println("Failed to read from sensors!"); // Check for errors in readings
    OLED.clearDisplay();
    OLED.setCursor(0, 10);
    OLED.println("Failed to read from sensors!");
    OLED.display();
    return;
  }
 
  StaticJsonDocument<256> doc;
  doc["tempDS"] = tempDS;
  doc["humDHT"] = humDHT;
  doc["coPpm"] = coPpm;

  char jsonBuffer[256];
  size_t n = serializeJson(doc, jsonBuffer);

  Serial.print("Publishing to ");
  Serial.println(mqtt_topic);
  Serial.println(jsonBuffer);

  if (client.publish(mqtt_topic, jsonBuffer, n)) {
    Serial.println("Publish OK");
    OLED.clearDisplay();
    OLED.setCursor(0, 10);
    OLED.println("Publish OK");
    OLED.setCursor(0, 20);
    OLED.println(jsonBuffer);
    OLED.display();
    publishState ="Last send OK";
  } else {
    Serial.println("Publish failed");
    OLED.clearDisplay();
    OLED.setCursor(0, 10);
    OLED.println("Publish failed");
    OLED.setCursor(0, 20);
    OLED.println(jsonBuffer);
    OLED.display();
    publishState ="Last send failed";
  }
}

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
}

unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 60000; // 60 seconds

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();

  // publish every 60s without blocking
  if (currentMillis - lastPublishTime >= publishInterval) {
    lastPublishTime = currentMillis;
    publishData();
  }

  // calculate remaining time until next publish
  unsigned long elapsed = currentMillis - lastPublishTime;
  unsigned long remaining = (elapsed < publishInterval) ? (publishInterval - elapsed) : 0;

  // --- OLED display ---
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("Temp: "+(String)tempDS+" C");
  OLED.setCursor(0, 20);
  OLED.println("Wilg: "+(String)humDHT+" %");
  OLED.setCursor(0, 30);
  OLED.println("CO: "+(String)coPpm+" ppm");
  OLED.setCursor(0, 40);
  OLED.println(publishState);
  OLED.setCursor(0, 50);
  OLED.println("Next send in: "+(String)formatCountdown(remaining));
  OLED.display();

  delay(1000); // refresh display once per second
}


String formatCountdown(unsigned long remainingMs) {
  unsigned int seconds = remainingMs / 1000;
  unsigned int minutes = seconds / 60;
  seconds = seconds % 60;

  char buffer[8];
  sprintf(buffer, "%02u:%02u", minutes, seconds);
  return String(buffer);
}