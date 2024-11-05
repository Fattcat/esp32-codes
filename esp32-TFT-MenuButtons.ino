#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Nastavenie rozmerov obrazovky
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Nastavenie pinov pre displej
#define TFT_CS        15  // Chip select pin
#define TFT_RST       4  // Reset pin
#define TFT_DC        2   // Data/command pin

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Nastavenie pinov pre tlačidlá a bzučiak
#define BUTTON_UP     14
#define BUTTON_DOWN   27
#define BUTTON_ENTER  25
#define BUTTON_BACK   26
#define BUZZER_PIN    12

// Premenné pre menu
int menuIndex = 0;
const int menuItems = 4;
bool beepState = false;

// Funkcia na aktualizáciu zobrazenia
void displayMenu() {
  display.fillScreen(ST77XX_BLACK);
  display.setTextSize(2);
  display.setTextColor(ST77XX_WHITE);

  // Nadpis
  display.setCursor(60, 10);
  display.print("Mini PC");

  // Zobrazenie položiek menu s indikátorom aktuálnej polozky
  display.setTextSize(2);
  
  display.setCursor(10, 50);
  display.setTextColor(menuIndex == 0 ? ST77XX_YELLOW : ST77XX_WHITE);
  display.print("Calculator");
  
  display.setCursor(10, 90);
  display.setTextColor(menuIndex == 1 ? ST77XX_YELLOW : ST77XX_WHITE);
  display.print("Timer");
  
  display.setCursor(10, 130);
  display.setTextColor(menuIndex == 2 ? ST77XX_YELLOW : ST77XX_WHITE);
  display.print("Games");
  
  display.setCursor(10, 170);
  display.setTextColor(menuIndex == 3 ? ST77XX_YELLOW : ST77XX_WHITE);
  display.print("Calendar");

  // Zobrazenie stavu bzučiaka
  display.setCursor(10, 210);
  display.setTextColor(ST77XX_WHITE);
  display.print("Beep: ");
  display.print(beepState ? "ON" : "OFF");
}

void setup() {
  // Inicializácia pinov tlačidiel
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_ENTER, INPUT_PULLUP);
  pinMode(BUTTON_BACK, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Inicializácia displeja
  display.init(240, 320);  // Nastavenie rozlíšenia displeja
  display.invertDisplay(0);
  display.setRotation(0);  // Otočenie displeja, ak je potrebné

  display.fillScreen(ST77XX_BLACK);
  delay(2000); // Počiatočná obrazovka
  displayMenu();
}

void loop() {
  // Kontrola tlačidla UP na posúvanie hore
  if (digitalRead(BUTTON_UP) == LOW) {
    delay(200); // Debounce
    menuIndex = (menuIndex - 1 + menuItems) % menuItems;
    displayMenu();
  }

  // Kontrola tlačidla DOWN na posúvanie dole
  if (digitalRead(BUTTON_DOWN) == LOW) {
    delay(200); // Debounce
    menuIndex = (menuIndex + 1) % menuItems;
    displayMenu();
  }

  // Kontrola tlačidla ENTER na potvrdenie výberu
  if (digitalRead(BUTTON_ENTER) == LOW) {
    delay(200); // Debounce

    if (menuIndex == 0) {
      // Calculator - tu môžeš implementovať kalkulačku
    } else if (menuIndex == 1) {
      // Timer - tu môžeš implementovať časovač
    } else if (menuIndex == 2) {
      // Games - tu môžeš implementovať hry
    } else if (menuIndex == 3) {
      // Calendar - tu môžeš implementovať kalendár
    }

    // Prepínanie stavu bzučiaka
    beepState = !beepState;
    digitalWrite(BUZZER_PIN, beepState ? HIGH : LOW);
    
    displayMenu();
  }

  // Kontrola tlačidla BACK na návrat späť alebo reset
  if (digitalRead(BUTTON_BACK) == LOW) {
    delay(200); // Debounce
    // Možnosť implementovať návrat na predchádzajúcu obrazovku alebo reset
    displayMenu();
  }
}
