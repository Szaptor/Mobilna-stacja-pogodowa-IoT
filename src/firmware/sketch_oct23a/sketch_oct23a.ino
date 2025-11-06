

// --- DHT22 sensor humidity + temperature ---
#include <DHT.h>
#define DHTPIN 3      // Digital Pin connected to the DHT22 (humidity) data pin
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// --- DS18B20 sensor temperature ---
#include <OneWire.h> // For communication with sensor via digital on 1 wire of sensor
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2  // Digital Pin where the DS18B20 is connected
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

// --- MQ-3 dfrobot sensor alcohol/gas ---
#include <MQUnifiedsensor.h>
#define Board "Arduino UNO"
#define Pin A0         // Analog Pin input
#define Type "MQ-7"    // sensor type
#define Voltage_Resolution 5 // power voltage
#define ADC_Bit_Resolution 10 // sensor bit resolution
#define RatioMQ7CleanAir 27.5 // Rs/R0 w czystym powietrzu

// RL wartość (zależna od modułu)
// #define ResistanceLoad 10000.0 // 10 kΩ RL on most boards
MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);

// --- SSD1306 OLED connected to I2C (SDA, SCL pins) ---
#include <Wire.h> //I2C
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1 // No reset pin so -1
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup()
{
  Serial.begin(115200); // Start Serial USB interface
  while (!Serial) {
    ;
  }
   
  dht.begin(); // Start communication with DHT22
  
  DS18B20.begin(); // Start communication with DS18B20

  if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Test communication with OLED via I2C
    Serial.println("SSD1306 allocation failed"); 
      for(;;){;}
  }
  // --- Start and cofigure display ---
  OLED.display();
  delay(1000);
  OLED.clearDisplay();
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0, 10);
  OLED.println("Startowanie...");
  OLED.display();
  Serial.println("Startowanie...");
  delay(1000);

  // --- Gas sensor config ---
  MQ7.setRegressionMethod(1); // 1 = Exponential (dla MQ-3)
  MQ7.setA(99.042);           // współczynnik z krzywej z dokumentacji
  MQ7.setB(-1.518);           // współczynnik z krzywej z dokumentacji
  MQ7.init();

  Serial.println("Kalibracja - czyste powietrze");
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("Kalibracja - \nczyste powietrze");
  OLED.display();

  float calcR0 = 0;
  for (int i = 0; i < 10; i++) {
    MQ7.update();
    calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
    delay(500);
    
  }
  MQ7.setR0(calcR0 / 10);
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("R0 = "+(String)(calcR0 / 10));
  OLED.setCursor(0, 20);
  OLED.display();
  Serial.print("R0 = "+(String)(calcR0 / 10));
  Serial.println(calcR0 / 10);

  Serial.println("Kalibracja gotowa!");
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("Kalibracja gotowa!");
  OLED.display();
}

void loop() {
  // --- Read humidity and temperature readings from DHT22
  float tempDHT = dht.readTemperature(); // read DHT22 temp. - C by default //??
  float hum  = dht.readHumidity(); // read DHT22 humidity - % by default
  if (isnan(tempDHT) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!"); // Check for errors in readings
    OLED.clearDisplay();
    OLED.setCursor(0, 10);
    OLED.println("Failed to read from DHT sensor!");
    return;
  }
  // --- Read temperature readings from DS18B20
  DS18B20.requestTemperatures(); // Send command to get temperatures
  float tempDS = DS18B20.getTempCByIndex(0); // Read the first sensor
  
  // --- Gas/alcohol level readings from MQ3 ---
  MQ7.update();                // odczyt nowej próbki
  float ppmCO = MQ7.readSensor(); // automatyczne przeliczenie

  // --- Display readings ---
  delay(1000);
  Serial.println("DHT22 Temp: "+ (String)tempDHT+" C"+"\tHum: "+ (String)hum+" %");
  Serial.println("DS18B20 Temp: "+ (String)tempDS+" C");
  Serial.println("MQ3 level: "+ (String)ppmCO+" (ppm)");
  OLED.clearDisplay();
  OLED.setCursor(0, 10);
  OLED.println("DHT22 Temp: "+ (String)tempDHT+" C");
  OLED.setCursor(0, 20);
  OLED.println("DHT22 Hum: "+ (String)hum+" %");
  OLED.setCursor(0, 30);
  OLED.println("DS18B20 Temp: "+ (String)tempDS+" C");
  OLED.setCursor(0, 40);
  OLED.println("CO Level: "+ (String)ppmCO+" (ppm)");
  OLED.display();
  delay(5000);
}