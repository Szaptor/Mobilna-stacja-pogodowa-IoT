# Mobilna-stacja-pogodowa-IoT
Projekt mobilnej stacji pogodowej na przedmiot „Budowa systemów internetu rzeczy” by Dominik But oraz Bartosz Zawadzki
# Projekt ESP8266

## Spis treści

1. [Potrzebne materiały](#potrzebne-materiały)
2. [Schemat połączeń](#schemat-połączeń)
3. [Instalacja środowiska Arduino IDE](#instalacja-środowiska-arduino-ide)
4. [Instalujemy dodatek do obsługi ESP8266](#instalujemy-dodatek-do-obslugi-esp8266)
5. [Instalujemy potrzebne biblioteki](#instalujemy-potrzebne-biblioteki)
6. [Wgrywanie szkicu do ESP8266](#wgrywanie-szkicu-do-esp8266)

---

## Potrzebne materiały

### ESP32-S3-WROOM-1-N8R8 WiFi/Bluetooth 16MB
![ESP32-WROOM](/media/ESP32-3s-wroom1.jpg)
![piny](/media/piny.jpg)
### Wyświetlacz OLED niebieski graficzny 0,96'' 128x64px I2C
![OLED](/media/OLED.jpg)
### Gravity: Analog Carbon Monoxide Sensor (MQ7) - moduł z czujnikiem tlenku węgla
![Czujnik tlenku węgla MQ7](/media/czujnik_tlenku_wegla.jpg)
### Czujnik temperatury DS18B20 TO92 1-Wire Dallas
![Czujnik temperatury DS18B20](/media/czujnik_temperatury.jpg)
### Moduł DHT22 Czujnik Temperatury i Wilgotności Black
![Czujnik wilgotnośc DHT22](/media/czujnik_wilgotnosci.jpg)

---

## Schemat połączeń
![ROBOCZY SCHEMAT POŁĄCZEŃ](/media/schemat_polaczen.jpg)
---

## Instalacja środowiska Arduino IDE
1. [Pobieramy Arduino IDE z tej lokalizacji](https://www.arduino.cc/en/software/)
![Strona internetowa](/media/instalacjaIDE.jpg)
2. W trakcie instalacji zalecamy pozostawienie opcji domyślnych.
3. Podczas pierwszego uruchomienia programu należy zezwolić na zainstalowanie oprogramowania oraz poczekać na instalacje wszystkich sterowników USB i dodatków wymaganych przez IDE.
![Zezwolenie](/media/zezwol.jpg)
4. Zalecamy również zmianę języka na polski. Można tego dokonać poprzez kliknięcie zakładki „File”, a następnie wybrać „Preferenes”, a w nich „Settings” - poniższy zrzut ekranu prezentuje wspominane miejsce.
![Polski język](/media/zmiana_jezyka.jpg)
---

## Instalujemy dodatek do obsługi ESP32

