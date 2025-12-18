# Mobilna-stacja-pogodowa-IoT
Projekt mobilnej stacji pogodowej na przedmiot „Budowa systemów internetu rzeczy” by Dominik But oraz Bartosz Zawadzki

## Spis treści

1. [Potrzebne materiały](#potrzebne-materiały)
2. [Schemat połączeń](#schemat-połączeń)
3. [Instalacja środowiska Arduino IDE](#instalacja-środowiska-arduino-ide)
4. [Instalacja dodatku do obsługi ESP32](#instalacja-dodatku-do-obsługi-esp32)


---
<a name="Potrzebne materiały"></a>
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
<a name="Schemat połączeń]"></a>
## Schemat połączeń
![SCHEMAT POŁĄCZEŃ](/media/schemat_polaczen.jpg)
---

<a name="Instalacja środowiska Arduino IDE]"></a>
## Instalacja środowiska Arduino IDE
[Pobieramy Arduino IDE z tej lokalizacji](https://www.arduino.cc/en/software/)
   
![Zrzut ekranu prezentujący stronę](/media/instalacjaIDE.png)
W trakcie instalacji zalecamy pozostawienie opcji domyślnych.

Podczas pierwszego uruchomienia programu należy zezwolić na zainstalowanie oprogramowania oraz poczekać na instalacje wszystkich sterowników USB i dodatków wymaganych przez IDE.
   
![Zapytanie o zezwolenie](/media/zezwol.png)
Zalecamy również zmianę języka na polski. Można tego dokonać poprzez kliknięcie zakładki „File”, a następnie wybrać „Preferenes”, a w nich „Settings” - poniższy zrzut ekranu prezentuje wspominane miejsce.

![Okienko z ustawieniami umożliwiającymi zmianę języka](/media/zmiana_jezyka.png)
---

<a name="Instalacja dodatku do obsługi ESP32]"></a>
## Instalacja dodatku do obsługi ESP32

W celu instalacji menadżera płytek do obsługi wybranego przez nas mikrokontrolera ESP32, należy z bocznego paska wybrać drugą od góry ikonkę (zaznacz), a nastepnie wyszukać frazę „esp32”. Należy skupić się na drugiej pozycji która została udostępniona przez firmę Espressif Systems. Z rozwijlanego menu należy wybrać conajmniej wersję 3.3.3. W tym momencie można kliknąć przycisk „ZAINSTALUJ”. Sytuację tę prezentuje poniższy zrzut ekranu. Sama instalacja może potrwać kilka minut.
![Zrzut ekranu prezentujący wybór dodatku obsługująćego ESP32](/media/wybor_esp32.png)
