#include <BleKeyboard.h>

// BLE Keyboard
BleKeyboard bleKeyboard("JBL Xtreme", "Custom Manufacturer", 100);

// Vlastná MAC adresa
const uint8_t customMacAddr[] = {0xF4, 0xF3, 0xAA, 0x07, 0xE4, 0xF2};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Nastavenie vlastnej MAC adresy
  esp_base_mac_addr_set(customMacAddr);
  Serial.print("Vlastná MAC adresa nastavená na: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", customMacAddr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  // Inicializácia BLE Keyboard
  bleKeyboard.begin();
}

void loop() {
  // Skontrolujeme, či je zariadenie pripojené
  if (bleKeyboard.isConnected()) {
    Serial.println("BLE zariadenie pripojené!");

    // Odoslanie mediálneho tlačidla WWW_HOME
    Serial.println("Odosielam tlačidlo: WWW_HOME");
    bleKeyboard.write(KEY_MEDIA_WWW_HOME);

    // Počkajte 5 sekúnd pred ďalším odoslaním
    delay(600);
  } else {
    Serial.println("Čakám na pripojenie BLE zariadenia...");
  }

  delay(100);
}
