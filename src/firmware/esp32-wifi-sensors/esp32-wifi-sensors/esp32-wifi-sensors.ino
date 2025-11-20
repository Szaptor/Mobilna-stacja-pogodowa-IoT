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
//WiFiManager 2.0.17


#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>


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


// // ==== WIFI CONFIG ====
// const char* ssid = "Note 12 Pro";
// const char* password = "123456789";

// // ==== MQTT CONFIG ====
// const char* mqtt_server = "iot.marekurbaniak.pl";  // or IP address
// const int mqtt_port = 1883;                         // TCP port (not TLS)
// const char* mqtt_user = "WeatherStation";       // optional
// const char* mqtt_pass = "iot2025projekt";       // optional
// const char* mqtt_topic = "v1/devices/me/telemetry";        // your topic name

// ==== DATA ====
float tempDS = 0;
float humDHT = 0;
float coPpm = 0;

// ==== CLIENTS ====
WiFiClient espClient;
PubSubClient client(espClient);


// ==== MQTT CONFIG (set via WiFiManager) ====
const char* mqtt_topic = "v1/devices/me/telemetry"; // stays constant
String mqtt_server = "iot.marekurbaniak.pl";
int mqtt_port = 1883;
String mqtt_user = "WeatherStation";
String mqtt_pass = "iot2025projekt";
char mqtt_server_buf[40];
char mqtt_port_buf[6];
char mqtt_user_buf[40];
char mqtt_pass_buf[40];


bool configureWiFiAndMQTT() {
	OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("Opening wifi setup...");
  OLED.display();
  strncpy(mqtt_server_buf, mqtt_server.c_str(), sizeof(mqtt_server_buf));
  strncpy(mqtt_port_buf,  String(mqtt_port).c_str(), sizeof(mqtt_port_buf));
  strncpy(mqtt_user_buf,  mqtt_user.c_str(), sizeof(mqtt_user_buf));
  strncpy(mqtt_pass_buf,  mqtt_pass.c_str(), sizeof(mqtt_pass_buf));
  WiFiManager wm;

  // Custom parameters
  WiFiManagerParameter p_server("server", "MQTT Server", mqtt_server_buf, 40);
  WiFiManagerParameter p_port("port", "MQTT Port", mqtt_port_buf, 6);
  WiFiManagerParameter p_user("user", "MQTT User", mqtt_user_buf, 40);
  WiFiManagerParameter p_pass("pass", "MQTT Password", mqtt_pass_buf, 40);

  wm.addParameter(&p_server);
  wm.addParameter(&p_port);
  wm.addParameter(&p_user);
  wm.addParameter(&p_pass);

  wm.setTimeout(180);

  // Try to connect, or open AP
  if (!wm.autoConnect("WeatherStationAP", "12345678")) {
    Serial.println("WiFiManager Timeout → Restarting");
    OLED.clearDisplay();
    OLED.setCursor(0, 10);
    OLED.println("WiFiManager Timeout → Restarting");
    OLED.display();
    delay(3000);
    ESP.restart();
    return false; // unreachable, but consistent
  }

  Serial.println("WiFi connected!");
  Serial.println(WiFi.localIP());

  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("WiFi connected!");
  OLED.setCursor(0, 20);
  OLED.println(WiFi.localIP());
  OLED.display();
  // Save MQTT params
    mqtt_server = p_server.getValue();
    mqtt_port   = atoi(p_port.getValue());
    mqtt_user   = p_user.getValue();
    mqtt_pass   = p_pass.getValue();
  // Apply server settings
  client.setServer(mqtt_server.c_str(), mqtt_port);

  // -------------------------
  // NEW: Check MQTT NOW
  // -------------------------
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("Checking MQTT...");
  OLED.display();

  String cid = "SIMA7670E";
  
  if (!client.connect(cid.c_str(), mqtt_user.c_str(), mqtt_pass.c_str())) {
    Serial.println("MQTT check failed → reopening WiFiManager");
    OLED.clearDisplay();
    OLED.setCursor(0, 10);
    OLED.println("MQTT FAILED!");
    OLED.setCursor(0, 20);
    OLED.println("Reopen config...");
    OLED.display();
    delay(2000);
    
    // Open AP immediately (NO wifi erase)
    wm.startConfigPortal("WeatherStationAP", "12345678");
   
  }
  delay(1000);
  // Save MQTT params
        mqtt_server = p_server.getValue();
        mqtt_port   = atoi(p_port.getValue());
        mqtt_user   = p_user.getValue();
        mqtt_pass   = p_pass.getValue();
     
    Serial.println("MQTT Server: " + mqtt_server);
    Serial.println("MQTT Port: " + String(mqtt_port));
    Serial.println("MQTT User: " + mqtt_user);
  OLED.clearDisplay();
  OLED.setCursor(0, 30);
  OLED.println("MQTT Server: " + mqtt_server);
  OLED.setCursor(0, 40);
  OLED.println("MQTT Port: " + String(mqtt_port));
  OLED.setCursor(0, 40);
  OLED.println("MQTT User: " + mqtt_user);
  OLED.display();
  delay(1000);
  client.disconnect();
  delay(1000);
  client.setServer(mqtt_server.c_str(), mqtt_port);
  Serial.println("MQTT OK after WiFi setup!");
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("MQTT OK!");
  OLED.display();
  delay(800);
  return true;
}

int mqttFailCount = 0;
const int mqttFailLimit = 5;   // After 5 failures → reopen WiFiManager

bool reconnectMQTT() {
  if (client.connected()) {
    mqttFailCount = 0;   // reset counter
    return true;
  }

  Serial.println("Attempting MQTT connection...");
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("Attempting MQTT connection...");
  OLED.display();
  String clientId = "SIMA7670E";
  if (client.connect(clientId.c_str(), mqtt_user.c_str(), mqtt_pass.c_str())) {
    Serial.println("MQTT connected");
    OLED.clearDisplay();
    OLED.setCursor(0, 10);
    OLED.println("MQTT connected");
    OLED.display();
    mqttFailCount = 0;   // success → reset
    return true;
  }

  // FAILED
  mqttFailCount++;
  Serial.print("MQTT failed (");
  Serial.print(mqttFailCount);
  Serial.println(")");
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("MQTT failed ("+(String)mqttFailCount+")");
  OLED.display();

  if (mqttFailCount >= mqttFailLimit) {
    Serial.println("Too many MQTT failures → reopening WiFiManager");
    OLED.clearDisplay();
    OLED.setCursor(0, 10);
    OLED.println("Too many MQTT failures → reopening WiFiManager");
    OLED.display();
    // Run full WiFiManager + re-download MQTT params
    configureWiFiAndMQTT();

    mqttFailCount = 0;
  }

  return false;
}

String publishState="";
void publishData() {
  
  // // --- Read humDHT and tempDS readings from DHT22
  // // tempDS = dht.readTemperature(); // read DHT22 temp. - C by default //??
  // humDHT = dht.readHumidity(); // read DHT22 humDHT - % by default
  // humDHT = roundf(humDHT * 100) / 100.0;
  Serial.println(humDHT);
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("Temp: "+(String)tempDS);
  // // --- Read tempDS readings from DS18B20
  // DS18B20.requestTemperatures(); // Send command to get tempDSs
  // tempDS = DS18B20.getTempCByIndex(0); // Read the first sensor
  // tempDS = roundf(tempDS * 100) / 100.0;
  Serial.println(tempDS);
  OLED.setCursor(0, 20);
  OLED.println("Humidity: "+(String)humDHT);
  //  // --- Gas/alcohol level readings from MQ3 ---
  // MQ7.update();                // odczyt nowej próbki
  // coPpm = MQ7.readSensor(); // automatyczne przeliczenie
  // coPpm = roundf(coPpm * 100) / 100.0;
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
bool configRunning = false;
void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  
   if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed"); 
    for(;;);
  }

  OLED.clearDisplay();
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0, 10);
  OLED.println("Starting setup...");
  OLED.display();
  delay(500);
  
  
// RUN ONLY THE EXTRACTED WIFIMANAGER LOGIC

  configureWiFiAndMQTT();
 

  dht.begin();
  MQ7.setRegressionMethod(1);
  MQ7.setA(99.042);
  MQ7.setB(-1.518);
  MQ7.init();

  float calcR0 = 0;
  for (int i = 0; i < 10; i++) {
    MQ7.update();
    calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
    delay(500);
  }
  MQ7.setR0(calcR0 / 10);
  DS18B20.begin();

}

unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 60000; // 60 seconds

void loop() {
  reconnectMQTT();   // will auto-reopen WiFiManager when needed
  
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
  // --- Read humDHT and tempDS readings from DHT22
  // tempDS = dht.readTemperature(); // read DHT22 temp. - C by default //??
  humDHT = dht.readHumidity(); // read DHT22 humDHT - % by default
  humDHT = roundf(humDHT * 100) / 100.0;
  // --- Read tempDS readings from DS18B20
  DS18B20.requestTemperatures(); // Send command to get tempDSs
  tempDS = DS18B20.getTempCByIndex(0); // Read the first sensor
  tempDS = roundf(tempDS * 100) / 100.0;
   // --- Gas/alcohol level readings from MQ3 ---
  MQ7.update();                // odczyt nowej próbki
  coPpm = MQ7.readSensor(); // automatyczne przeliczenie
  coPpm = roundf(coPpm * 100) / 100.0;
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

  delay(500); // refresh display once per second
}


String formatCountdown(unsigned long remainingMs) {
  unsigned int seconds = remainingMs / 1000;
  unsigned int minutes = seconds / 60;
  seconds = seconds % 60;

  char buffer[8];
  sprintf(buffer, "%02u:%02u", minutes, seconds);
  return String(buffer);
}