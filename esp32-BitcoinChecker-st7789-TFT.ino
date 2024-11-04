#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Definovanie pinov pre ST7789
#define TFT_CS   15
#define TFT_RST  4
#define TFT_DC   2
#define BuzzerPin 12

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Wi-Fi údaje
const char* ssid = "YourSSID";  // Zmeňte na vašu Wi-Fi SSID
const char* password = "YourPASS";  // Zmeňte na vaše Wi-Fi heslo

// URL na API pre cenu Bitcoinu
const char* bitcoinApiUrl = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=eur";

float previousPrice = 0.0; // Premenná pre uchovávanie predchádzajúcej ceny Bitcoinu

void setup() {
  Serial.begin(115200);
  tft.init(240, 320);
  tft.invertDisplay(0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 10);
  tft.println("Bitcoin Kontrola");
  tft.setCursor(10, 60);
  tft.print("Pripajam sa WiFi");
  tft.setCursor(20, 100);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(ssid);

  pinMode(BuzzerPin, OUTPUT);

  // Pripojenie na Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Pripojene k WiFi :D");
  
  // Načítanie a zobrazenie hodnoty Bitcoinu
  getBitcoinPrice();
}

void loop() {
  delay(60000);  // Obnovovanie každej minúty
  getBitcoinPrice();
}

void getBitcoinPrice() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(BuzzerPin, HIGH);
    delay(100);
    digitalWrite(BuzzerPin, LOW);
    delay(100);

    HTTPClient http;
    http.begin(bitcoinApiUrl);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
      parseAndDisplayPrice(payload);
    } else {
      Serial.printf("Error on HTTP request: %s\n", http.errorToString(httpCode).c_str());

      digitalWrite(BuzzerPin, HIGH);
      delay(100);
      digitalWrite(BuzzerPin, LOW);
      delay(100);
      
    }
    http.end();
  } else {
    Serial.println("WiFi NEpripojena !");
  }
}

void parseAndDisplayPrice(String json) {
  // Získanie ceny BTC z JSON odpovede
  int eurIndex = json.indexOf("eur");
  int priceStart = json.indexOf(":", eurIndex) + 1;
  int priceEnd = json.indexOf("}", priceStart); // Zmena tu na '}' namiesto ','

  String btcPrice = json.substring(priceStart, priceEnd);
  
  // Odstránenie bielych znakov, ak existujú
  btcPrice.trim();
  float currentPrice = btcPrice.toFloat(); // Preveďte cenu na float

  // Zobrazte cenu na displeji
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(40, 120);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_GREEN);
  tft.println("Cena Bitcoinu");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(70, 160);
  tft.print(btcPrice);
  tft.print(" Eur");

  // Zobrazovanie predchádzajúcej ceny
  tft.setTextSize(2);
  tft.setCursor(10, 190); // 30 px pod aktuálnou cenou
  float change = currentPrice - previousPrice; // Výpočet zmeny

  if (previousPrice > 0) { // Kontrola, či je predchádzajúca cena nastavená
    if (change > 0) {
      tft.setTextColor(ST77XX_GREEN); // Zelená pre nárast
      tft.print("HORE o ");
      tft.print(String(change, 2)); // Zobrazenie zmeny s dvoma desatinnými miestami
      tft.print(" Eur");

      for (int i = 0 ; i < 4; i++) {
        digitalWrite(BuzzerPin, HIGH);
        delay(1000);
        digitalWrite(BuzzerPin, LOW);
        delay(1000);
      }

    } else if (change < 0) {
      tft.setTextColor(ST77XX_RED); // Červená pre pokles
      tft.print("DOLE o ");
      tft.print(String(abs(change), 2)); // Zobrazenie absolútnej hodnoty poklesu
      tft.print(" Eur");

      for (int i = 0 ; i < 2; i++) {
        digitalWrite(BuzzerPin, HIGH);
        delay(1000);
        digitalWrite(BuzzerPin, LOW);
        delay(1000);
      }

    } else {
      tft.setTextColor(ST77XX_YELLOW); // Žltá pre nezmenu
      tft.setCursor(60,190);
      tft.print("Nezmenene !");
    }
  }

  previousPrice = currentPrice; // Uloženie aktuálnej ceny ako predchádzajúcej
}
