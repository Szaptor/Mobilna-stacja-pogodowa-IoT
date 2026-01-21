# Mobilna stacja pogodowa - projekt IoT
Niniejsze repozytorium jest poświęcone projektowi IoT mobilnej stacji pogodowej. Projekt składa się z czujników podłączonego do mikrokontrolera, który komunikuje się protokołem MQTT z panel na otwartoźródłowej platformie IoT ThingsBoard.

Do rozpoczęcia pracy z projektem poza później wyszczególnionymi materiałami wymagane jest posiadanie dostępu do platformy ThingsBoard, dane służące połączeniu się z nią przez MQTTT basic; urządzenie z dostępem do WiFi, w celu późniejszego połączenia się z Access Pointem mikrokontrolera oraz komputer z Arduino IDE.

## Przykład poprawnie złożonej i działającej stacji pogodowej

![Zdjęcie prezentujące poprawnie działającą stację](/media/zdjecie_dzialajacej_stacji.jpg)

## Przykład poprawnie skonfigurowanego i działającego panelu na platformie ThingsBoard.

![Zrzut ekranu prezentujący poprawnie działający panel](/media/dzialajacy_panel.png)

## Podział prac w projekcie
| Dominik But | Bartosz Zawadzki |
| ----------- | ---------------- |
|Pomysłodawca, główny monter oraz programista|Pomoc w montowaniu i programowaniu, dokumentacja|

## Struktura niniejszego repozytorium
* W niniejszym pliku README.md znajduje się cała dokumentacja, wraz z poradnikiem jak odtworzyć nasz projekt.
* W folderze media znajdują się wszystkie użyte w tym pliku zdjęcia i zrzuty ekranu.
* Folder src zawiera pliki źródłowe dla mikrokontrolera i panelu. Odnośniki do odpowiednich znajdują się w niniejszym pliku.
* Plik LICENSE zawiera informacje o licencji.
  
## Spis treści

1. [Potrzebne materiały](#potrzebne-materiały)
2. [Schemat połączeń](#schemat-połączeń)
3. [Proces konstrukcji układu](#konstrukcja)
4. [Instalacja środowiska Arduino IDE](#instalacja-środowiska-arduino-ide)
5. [Instalacja dodatku do obsługi ESP32](#instalacja-dodatku-do-obsługi-esp32)
6. [Instalacja wymaganych bibliotek](#instalacja-wymaganych-bibliotek)
7. [Dalsza konfiguracja](#dalsza-konfiguracja)
8. [Wgrywanie szkicu](#wgrywanie-szkicu)
9. [Konfiguracja do połączenia z serwerem MQTT](#konfiguracja-do-połączenia-z-serwerem-mqtt)
10. [Środowisko archiwizacji i wizualizacji danych MQTT – ThingsBoard](#tb)
11. [Konfiguracja środowiska ThingsBoard](#config-tb)
---

<a name="Potrzebne materiały"></a>
## Potrzebne materiały

### ESP32-S3-WROOM-1-N8R8 WiFi/Bluetooth 16MB
![ESP32-WROOM](/media/ESP32-3s-wroom1.jpg)

W projekcie stacji pogodowej wykorzystaliśmy podane niżej piny:
* 5V0, 3V3, GND dla zasilania czujników i ekranu
* GPIO4 jako linia sygnałowa czujnika temperatury (DS18B20)
* GPIO11 jako linia sygnałowa czujnika wilgotności (DHT22)
* GPIO8 jako linia sygnałowa czujnika telenku węgla (MQ7)
* GPIO6 i GPIO7 jako linie sygnałowe wyświetlacza OLED
  
![piny](/media/piny.jpg)

### Wyświetlacz OLED niebieski graficzny 0,96'' 128x64px I2C

![OLED](/media/OLED.jpg)

### Gravity: Analog Carbon Monoxide Sensor (MQ7) - moduł z czujnikiem tlenku węgla

![Czujnik tlenku węgla MQ7](/media/czujnik_tlenku_wegla.jpg)

### Czujnik temperatury DS18B20 TO92 1-Wire Dallas

![Czujnik temperatury DS18B20](/media/czujnik_temperatury.jpg)

### Moduł DHT22 Czujnik Temperatury i Wilgotności Black

![Czujnik wilgotnośc DHT22](/media/czujnik_wilgotnosci.jpg)

### Dodatkowe elementy
Na potrzeby naszego układu należy dodatkowo przygotować:
* płytkę/ki stykową/we
* kilkanaście przwodów male-male oraz kilka female-male zależnie od charakterystyki posiadanych czujników
* 1x rezystor 4,7 kOhm
* 1 x rezystor 10 kOhm
* 1 x rezystor 20 kOhm
---

<a name="Schemat połączeń"></a>
## Schemat połączeń
![SCHEMAT POŁĄCZEŃ](/media/schemat_polaczen.jpg)

---

<a name="konstrukcja"></a>
## Proces konstrukcji układu
Przygotowanie układu najlepiej rozpocząć od umieszczenia na płytce stykowej mikrokontrolera ESP32 oraz czujników i ekranu (jeżeli jest na to miejsce to dokładnie tak jak przedstawiono na schemacie podłączeń). Następnie należy umieścić odpowiednio rezystory. Rezystor 4,7 kOhm należy ustawić na płytce stykowej łącząc nim linie sygnałową (przewód kolorowy najczęściej pomarańczowy/biały) z przewodem zasilania (czerwony +) czujnika temperatury. Rezystory 10 i 20 kOhm należy umieścić tak, aby ich jedna strona znajdowała się na tej samej linii sygnałowej. 

Następnie należy doprowadzić do wszystkich czujników i wyświetlacza masę (czarny -) przewodami z mikrokontrolera ESP32. Dodatkowo do jednej strony rezystora 20 kOhm należy doprowadzić też masę (należy pamiętać o zasadzie działania płytki stykowej!)

Następnym krokiem jest doprowadzenie do wszystkich do wszystkich czujników i wyświetlacza odpowiedniego zasilania. Przewodami (czerwony +) połączyć z mikrokontrolera ESP32, do czujnika wilgotności, temperatury i wyświetlacza OLED – 3V3, a do czujnika tlenku węgla 5V0.

Ostatnim krokiem jest połączenie przewodami wg schematu:
* pin GPIO6 do pinu SDA wyświetlacza, GPIO7 do pinu SCL wyświetlacza, 
* pin GPIO4 do jednej strony rezystora 4,7 kOhm podłączonego do linii sygnałowej czujnika temperatury,
* pin GPIO11 do pinu OUT czujnika wilgotności
* pin GPIO08 do jednej strony rezystora 10 kOhm
* dodatkowo od strony rezystora 20 kOhm, która jest niepodłączona do masy, należy doprowadzić przewód do kolorowego (najczęściej niebieskiego) przewodu czujnika tlenku węgla odpowiadającego za linię sygnałową.

Poniższe zdjęcie prezentuje przykadową ukonczoną konstrukcję układu.

![GRAFIKA Z KOŃCOWYM PROJEKTEM](/media/projekt.jpg)

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

<a name="Instalacja dodatku do obsługi ESP32"></a>
## Instalacja dodatku do obsługi ESP32

W celu instalacji menadżera płytek do obsługi wybranego przez nas mikrokontrolera ESP32, należy z bocznego paska wybrać drugą od góry ikonkę (zaznacz), a nastepnie wyszukać frazę „esp32”. Należy skupić się na drugiej pozycji która została udostępniona przez firmę Espressif Systems. Z rozwijlanego menu należy wybrać conajmniej wersję 3.3.3. W tym momencie można kliknąć przycisk „ZAINSTALUJ”. Sytuację tę prezentuje poniższy zrzut ekranu. Sama instalacja może potrwać kilka minut.

![Zrzut ekranu prezentujący wybór dodatku obsługująćego ESP32](/media/wybor_esp32.png)

---

<a name="Instalacja wymaganych bibliotek"></a>
## Instalacja wymaganych bibliotek
Do poprawnego działania naszej stacji pogodowej wymagane jest również zainstalowanie kilku dodatkowych bibliotek. Schemat ich pobierania jest bardzo podobny do kroku wcześniejszego. Tym razem należy jednak wybrać z bocznego paska trzecią od góry ikonkę (zaznacz), a nastepnie wyszukać frazę zadanej biblioteki.
### dht sensor library
Jak nazwa wsazuje jest do biblioteka czujnika DHT. Program poinformuje nas, że wybrana biblioteka wymaga do działania biblioteki „Adafruit Unified Sensor”. Klikamy przycisk „Zainstaluj wszystko”. Prezentuje to poniższy zrzut ekranu.

![Zrzut ekranu prezentujący instalację biblioteki do DHT](/media/biblioteka_DHT.png)

### mqunifiedsensor
Biblioteka do obsługi czujnika MQ7.

![Zrzut ekranu prezentujący instalację biblioteki do MQ7](/media/biblioteka_mq.png)

### dallastemperature
Biblioteka do obsługi czujnika temperatury DS18B20 TO92 1-Wire Dallas.

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

<a name="Dalsza konfiguracja"></a>
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

---

<a name="Wgrywanie szkicu"></a>
## Wgrywanie szkicu
### Monitor portu szeregowego
Przed rozpoczęciem wgrania szkicu należy jeszcze ustawić odpowiednią prędkość nasłuchiwania danych. Aby to zrobić należy zlokalizować w prawej górnej części okna Arduino IDE przycisk lupy odpowiadającego za monitor portu szeregowego. Prezentuje to poniższy zrzut ekranu.

![Zrzut ekranu prezentujący przycisk monitora](/media/monitor_przycisk.png)

Kliknęcie tego przycisku spowoduje otwarcie dodatkowej zakładki w której będą pojawiać się udostępnione dane diagnostyczne. Teraz należy zmienić wartość prędkości nasłuchiwania danych. Domyślnie jest ona ustawiona na „9600 baud”. Należy zmienić tę wartość na zgodną z zaproponowanym przez nas szkicem projektu czyli na wartość „115200 baud”. Zmianę tę prezentują poniższe zrzuty ekranu.

![Zrzut ekranu prezentujący ekran monitora i domyślną prędkość](/media/monitor_domyslna.png)

![Zrzut ekranu prezentujący przycisk monitora](/media/monitor_zmiana.png)

Aby przetestować działanie monitora należy przykładowo kliknąć przycisk „reset” znajdujący się na płytce ESP32 w okolicach portu USB. Spowoduje to wyświetlenie logów wykonanych przez ESP32. Wynik powinien wyglądać podobnie do załączonego zrzutu ekranu poniżej.

![Zrzut ekranu prezentujący test monitora](/media/monitor_test_reset.png)

### Wgrywanie szkicu
Należy teraz pobrać udostępniony przez nas [plik](src/firmware/esp32-wifi-sensors/esp32-wifi-sensors.ino).

Aby otworzyć wybrany plik w Arduino IDE należy w pasku narzędzi wybrać opcję „Plik”, a następnie „Otwórz”. Teraz należy wybrać pobrany plik .ino z projektem. Wybranie opcji „Otwórz” prezentuje poniższy zrzut ekranu.

![Zrzut ekranu prezentujący wgrywanie pliku](/media/wgrywanie_pliku.png)

W przypadku pojawienia się komunikatu o umieszczenie szkicu w folderze należy wybrać opcję „OK” co prezentuje poniższy zrzut ekranu.

![Zrzut ekranu prezentujący komunikat](/media/komunikat_ok.png)

<a name="poswiadczenia_kod"></a>
W tym momencie powinno uruchomić się nowe okno z załadowanym szkicem.<br>
W celu poprawnego działania należy ustawić wartości pokazanego poniżej fragmentu w kodzie na odpowiednie, które będą zgadzały się z poświadczeniami skonfigurowanymi dla urządzenia na platformie ThingsBoard w celu poprawnej komunikacji z serwerem MQTT. Informacje te będą potrzebne w tym [kroku](#poswiadczenia_panel). Ważne aby zmienić zawartość zmiennej mqtt_server na adres posiadanego serwera! Jeżeli korzysta się z MQTT Basic port pozostaje ten sam.

![Zrzut ekranu prezentujący fragment kodu dotyczący dostosowania](/media/fragment_do_zmiany.png)

Aby upewnić się, że program po zmianie kodu dalej jest poprawny należy go zweryfikować poprzez klinknięcie przycisku ptaszka/haczyka „Weryfikuj”, który pokazuje poniższy zrzut ekranu. Spowoduje to wstępną kompliację kodu projektu.

![Zrzut ekranu prezentujący przycisk do weryfikacji](/media/przycisk_weryfikuj.png)

Sama weryfikacja może potrwać kilka minut. Proces ten prezentują poniższe zrzuty ekranu.

![Zrzut ekranu prezentujący przebieg weryfikacji](/media/weryfikacja.png)

![Zrzut ekranu prezentujący wynik weryfikacji](/media/wynik_weryfikacji.png)

Jeżeli nie pojawiły się żadne błędy można przejść do wgrania szkicu do ESP32 poprzez kliknięcie przycisku strzałki „Prześlij” - znajduje się on obok poprzedniego. Prezentuje go poniższy zrzut ekranu.

![Zrzut ekranu prezentujący przycisk do przesyłania](/media/przycisk_przeslij.png)

Samo przesyłanie również może potrwać kilka minut. Proces ten prezentują poniższe zrzuty ekranu.

![Zrzut ekranu prezentujący przebieg weryfikacji](/media/przesylanie.png)

![Zrzut ekranu prezentujący wynik weryfikacji](/media/wynik_przesylania.png)

Wgranie szkicu spowoduje srestartowanie ESP32.

---
<a name="Konfiguracja do połączenia z serwerem MQTT"></a>
## Konfiguracja do połączenia z serwerem MQTT
Po pierwszym uruchomieniu ESP32 zostanie uruchomiony serwer ze stroną konfiguracyjną wyboru sieci WIFI oraz zmiany opcji MQTT. Należy wtedy połączyć się z właściwym access pointem udostępninoym przez ESP32. Domyślną nazwą tego będzie nazwa: "WeatherStationAP", a hasłem "12345678". Zrzut ekranu poniżej prezentuje tą sieć.

![Zrzut ekranu prezentujący sieć](/media/siec.png)

Po połączeniu do tej sieci zostanie otwarta strona internetowa z której należy skorzystać w cely wyboru sieci WIFI z której ma korzystać ESP32 do wysyłania danych. Przechodzi się na niej w “Configure wifi” a następnie wybiera sieć. Kolejno wpisuje się jej dane i kończy kliknęciem “Save”. ESP32 powinno zamknąć hotspot i spróbować połączyć się z wskazaną siecią WIFI. Poniżej znadują się zrzuty ekranu prezentujące stronę internetową i przykład konfiguracji.

![Zrzut ekranu prezentujący stronę](/media/strona.png)

![Zrzut ekranu prezentujący konfigurację sieci do której ESP32 ma się połączyć](/media/konfiguracja_sieci.png)

---
<a name="tb"></a>
## Środowisko archiwizacji i wizualizacji danych MQTT – ThingsBoard
ThingsBoard to otwartoźródłowa (open-source) platforma IoT (Internet Rzeczy), która służy do łączenia, zarządzania, wizualizacji danych i przetwarzania informacji pochodzących z urządzeń IoT, oferując skalowalną i elastyczną infrastrukturę, dostępną w wersji darmowej (Community Edition) i płatnej (Professional Edition) dla rozwiązań w chmurze lub on-premise, obsługując protokoły takie jak MQTT, CoAP, HTTP. 
Kluczowe funkcje i cechy:
* Zarządzanie urządzeniami: Łączenie, monitorowanie i zdalne sterowanie urządzeniami.
* Gromadzenie i przetwarzanie danych: Zbiera dane, które następnie można przetwarzać za pomocą wbudowanego silnika reguł (rule engine).
* Wizualizacja: Tworzenie pulpitów (dashboards) i wizualizacji danych w czasie rzeczywistym.
* Skalowalność i niezawodność: Zaprojektowana do obsługi dużej liczby urządzeń, z możliwością skalowania poziomego i odpornością na awarie (fault-tolerant).
* Protokoły: Obsługuje popularne protokoły IoT, w tym MQTT, CoAP, HTTP, LWM2M.
* Elastyczność: Umożliwia budowanie zarówno prototypów, jak i produkcyjnych rozwiązań.

### Jak korzystać z środowiska ThingsBoard?
Jak wspomnieliśmy powyżej istnieją dwie opcje korzystania z ThingsBoard - darmowa i płatna. Przy wyborze darmowej należy zainstalować serwer samemu. Najlepiej kierować się przy tym poradnikiem zawartym na tej stronie:

https://thingsboard.io/docs/user-guide/install/ubuntu/

W przypadku rozwiązania płatnego należy skorzystać z gotowego hostingu usługi. Informacje zawarte są na tej stronie:

https://thingsboard.io/pricing/

---
<a name="config-tb"></a>
## Konfiguracja środowiska ThingsBoard dla naszej stacji pogodowej
1. [Konfiguracja urządzenia](#config-tb-dev)
2. [Konfiguracja panelu](#config-tb-panel)
<a name="config-tb-dev"></a>
### Konfiguracja urządzenia
Aby skonfigurować urządzenie należy wejść na adresu swojego panelu ThingsBoard i zalogować się na swoje konto. Po zalogowaniu należy z bocznego paska nawigacyjnego wybrać zakładkę „Obiekty”, a po jej rozwinięciu „Urządzenia”. W panelu tym należy kliknąć znak „+”, a następnie „Dodaj nowe urządzenie”. Poniższe zrzuty ekranu prezentują znalezienie wspomnianej zakładki oraz lokalizację znaku „+”.

![Zrzut ekranu widok panelu](/media/panel.png)

![Zrzut ekranu prezentujący zakładkę Urządzenia](/media/urzadzenia.png)

W otwartym oknie należy wpisać nazwę swojego urządzenia. W tym przypadku jest to stacja pogodowa, dlatego używamy nazwy „Stacja_Pogodowa”. Na razie pozostałe parametry nie są potrzebne, dlatego można je pominąć i kliknąć przycisk „Dalej: Dane uwierzytelniające”. Zrzut z okna dodawania urządzenia znajduje się poniżej.

![Zrzut ekranu prezentujący okno dodawania urządzenia](/media/dodawanie_urzadzenia.png)

<a name="poswiadczenia_panel"></a>
Teraz w kreatorze dodawania urządzenia wyświetli się kolejna zakładka, w której należy wybrać protokół „MQTT Basic”. W wyniku tego działania pokaże się formularz konfiguracyjny poświadczeń dostępu dla urządzenia do serwera ThingsBoard poprzez protokół MQTT. Pole te można wypełnić według uznania, ALE CO WAŻNE **poświadczenia te muszą się zgadzać z poświadczeniami które zostały lub będą wpisane w odpowiednie zmienne w wgrywanym szkicu Arduino**. Były one pokazane w tym [kroku](#poswiadczenia_kod). W celu zapisania poświadczeń i dodania nowego urządzenia należy kliknąć przycisk „Dodaj”. Poniższe zrzuty ekranu prezentują wyżej wspomniane czynności.

![Zrzut ekranu prezentujący okno poświadczeń](/media/poswiadczenia.png)

![Zrzut ekranu prezentujący przykładowe poświadczenia](/media/przyklad_poswiadczen.png)

Nowo dodane urządzenie powinno pojawić się na liście urządzeń co pokazuje poniższy zrzut ekranu.

![Zrzut ekranu prezentujący pojawienie się urządzenie na liście](/media/lista_urzadzen.png)

---
<a name="config-tb-panel"></a>
### Konfiguracja panelu
Kolejnym krokiem jest utworzenie panelu. Panel ten umożliwi wyświetlanie informacji przesyłanych przez urządzenie. Oczywiście można utworzyć własny wygląd panelu, jednak można skorzystać z wcześniej przygotowanego przez nas wzoru. Aby to zrobić należy z panelu nawigacyjnego z boku wybrać zakładkę „Panele”, a następnie kliknąć znak „+” i wybrać opcję „Importuj panel”. Prezentuj to poniższy zrzut ekranu.

![Zrzut ekranu prezentujący zakładkę Panele](/media/panele.png)

Należy teraz pobrać udostępniony przez nas [plik](src/dashboard/panel_stacji_pogodowej_esp32.json) panelu dla stacji pogodowej, a następnie przeciągnąć go lub wybrać go z okna importu panelu. Po jego pojawieniu się należy kliknąć przycisk „Importuj”. Poniżej znajduje się zrzut ekranu pokazujący to okno.

![Zrzut ekranu prezentujący okno importu panelu](/media/import_panelu.png)

Po zaimportowaniu panelu pojawi się on na liście jako nowa pozycja co prezentuje poniższy zrzut ekranu.

![Zrzut ekranu prezentujący listę paneli](/media/lista_paneli.png)

Należy teraz kliknąć w ten panel i **GOTOWE!** Od tego momentu można z niego korzystać do śledzenia na żywo pomiarów nowo utworzonej stacji pogodowej. Można teraz włączyć stację pogodową i oczekiwać na uzyskanie od niej pierwszych pomiarów w oknie panelu. Zrzut ekranu poniżej prezentuje poprawnie działający panel.

![Zrzut ekranu prezentujący poprawnie działający panel](/media/dzialajacy_panel.png)

#### $${\color{red}* UWAGA}$$  - JEŻELI  USTAWIONO  INNĄ  NAZWĘ  URZĄDZENIA
Jeżeli ustawiono inną nazwę urządzenia należy kliknąć w przycisk „Edit mode” otwartego już gotowego panelu i kolejno zmieniać w każdym z widżetów nazwę urządzenia na swoje. Aby to wykonwać należy wybierać ikonkę ołówka. Lokalizację przycisku „Edit mode” pokazuje poniższy zrzut ekranu.

![Zrzut ekranu prezentujący przycisk Edit mode](/media/edit_mode.png)

# DZIĘKUJEMY ZA ŚLEDZENIE NINIEJSZEGO PORADNIKA I ŻYCZYMY POWODZENIA!
