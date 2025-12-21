# Mobilna-stacja-pogodowa-IoT
Projekt mobilnej stacji pogodowej na przedmiot „Budowa systemów internetu rzeczy” by Dominik But oraz Bartosz Zawadzki

## Spis treści

1. [Potrzebne materiały](#potrzebne-materiały)
2. [Schemat połączeń](#schemat-połączeń)
3. [Instalacja środowiska Arduino IDE](#instalacja-środowiska-arduino-ide)
4. [Instalacja dodatku do obsługi ESP32](#instalacja-dodatku-do-obsługi-esp32)
5. [Instalacja wymaganych bibliotek](#instalacja-wymaganych-bilbliotek)
6. [Dalsza konfiguracja](#konfiguracja)


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
---

<a name="Instalacja wymaganych bibliotek]"></a>
## Instalacja wymaganych bibliotek
Do poprawnego działania naszej stacji pogodowej wymagane jest również zainstalowanie kilku dodatkowych bibliotek. Schemat ich pobierania jest bardzo podobny do kroku wcześniejszego. Tym razem należy jednak wybrać z bocznego paska trzecią od góry ikonkę (zaznacz), a nastepnie wyszukać frazę zadanej biblioteki.
### dht sensor library
Jak nazwa wsazuje jest do biblioteka czujnika DHT. Program poinformuje nas, że wybrana biblioteka wymaga do działania biblioteki „Adafruit Unified Sensor”. Klikamy przycisk „Zainstaluj wszystko”. Prezentuje to poniższy zrzut ekranu.
![Zrzut ekranu prezentujący instalację biblioteki do DHT](/media/biblioteka_DHT.png)

### mqunifiedsensor
Bibliotek do obsługi czujnika MQ7.
![Zrzut ekranu prezentujący instalację biblioteki do MQ7](/media/biblioteka_mq.png)
### dallastemperature
Bibliotek do obsługi czujnika temperatury DS18B20 TO92 1-Wire Dallas.
![Zrzut ekranu prezentujący instalację biblioteki Dallas](/media/biblioteka_dallas.png)
### onewire
Kolejna biblioteka służąca do obłśugi czujnika temperatury.
![Zrzut ekranu prezentujący instalację biblioteki Onewire](/media/biblioteka_onewire.png)
### adafruit ssd1306
W tym przypadku podobnie jak z biblioteką „dht sensor library” pojawi się informacja, że wymagane są biblioteki „adafruit busio” i „adafruit gfx library”. Tak samo jak wcześniej klikamu przycisk „Zainstaluj wszystko”.
![Zrzut ekranu prezentujący instalację biblioteki Adafruit](/media/biblioteka_adafruit.png)
### wifimanager
Biblioteka ta jest potrzeba do komunikacji przy użyciu wifi oraz do miniswerwera konfiguracyjnego wifi.
![Zrzut ekranu prezentujący instalację biblioteki wifimanager](/media/biblioteka_wifi.png)
### pubsubclient
Biblioteka jest potrzebna dla klienta mqtt.
![Zrzut ekranu prezentujący instalację biblioteki pubsubclient](/media/biblioteka_pubsubclient.png)
### arduinojson
Biblioteka ta jest potrzebna do łatwiejszego formatowania danych przesyłanych później na serwer.
![Zrzut ekranu prezentujący instalację biblioteki arduinojson](/media/biblioteka_arduinojson.png)
---

<a name="konfiguracja]"></a>
## Dalsza konfiguracja
Użyta przez nas płytka posiada wbudowany konwerter portu szeregowego, dlatego możena podłączyć ją do komputera poprzez port USB i poczekać na instalację sterowników.
Przed wgraniem szkicu do podłączonego mikrokontrolera należy ustawić w Arduino IDE kilka ważnych ustawień.
<br>Pierwszym z tych ustawień jest wybranie odpowiedniej płytki. Aby to zrobić należy kliknąć na „Wybierz płytkę” na pasku wstążki a następnie wybrać „ESP32 Family Device”. Prezentuje to poniższy zrzut ekranu.
![Zrzut ekranu prezentujący wybór płytki](/media/wybor_plytki.png)
<br>
Kolejnym krokiem jest kliknęcie zakładki „Narzędzia”, następnie wybranie opcji: płytka „ESP32 family device”, a następnie „esp32”. Z wyświetlonej listy należy wybrać wyświetlone na jej początku „ESP32S3 Dev Module”. Pdojęty wybór prezentuje poniższy zrzut ekranu.
![Zrzut ekranu prezentujący odpowiedni wybór w zakładce narzędzia](/media/wybor_narzedzia.png)
Poprawne wybranie w poprzednim kroku spowoduje zmianę nazwy wybranej płytki. Zmianę tę prezentuje zrzut ekranu poniżej.
![Zrzut ekranu prezentujący zmianę nazwy](/media/zmiana_nazwy.png)
Następnym krokiem jest wybranie poprawnej ilości pamięci jaką posiada płytka. W naszym zestawie ma ona 16MB. Aby to zmienić należy ponownie kliknąć zakładkę „Narzędzia”, a następnie wybrać opcję „Flash Size: XMB”. Z rozwiniętej listy należy wybrać opcję „16MB (128Mb)”.  Poniżej znajduje się zrzut ekranu prezentujący zmianę tej opcji.
![Zrzut ekranu prezentujący zmianę pamięci RAM](/media/zmiana_ramu.png)
Ostatnim krokiem przed wgraniem szkicu należy ustawić prędkość jego wgrywania. Aby to zrobić należy podobnie jak w poprzednich krokach klikąć zakładkę „Narzędzia”, a następnie wybrać opcję „Upload Speed: X”. Z tej listy najlepiej wybrać jak największą wartość, którą jest w stanie obsłużyć podłączona płytka. W przypadku tego projektu należy wybrać opcję „512000”. Dokonywanie tej zmiany prezentuje poniższy zrzut ekranu.
![Zrzut ekranu prezentujący zmianę prędkości wgrywania](/media/zmiana_predkosci.png)
Dla dodatkowej pewności można sprawdzić jeszcze, czy program poprawnie wykrywa podłączoną płytkę. Aby to zrobić, należy ponownie wybrać zakładkę „Narzędzia”, a następnie wybrać opcję „Pobierz informacje o płytce”. Wybranie tej opcji spowoduje wyświetlenie komunikatu. Poniżej znajdują się zrzuty ekranu prezentujące tę czynność.
![Zrzut ekranu prezentujący pobieranie ifnormacji o płytce](/media/info1.png)
![Zrzut ekranu prezentujący komunikat po pobraniu](/media/info2.png)
