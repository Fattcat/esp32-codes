#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEHIDDevice.h>
#include <HIDTypes.h>
#include <BLEKeyboard.h>

// Vlastná MAC adresa
const uint8_t customMacAddr[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

// BLE Keyboard
BLEKeyboard bleKeyboard("Pro Speaker", "Custom Low Paid Manufacturer", 100);

bool deviceConnected = false;

// Callback pre detekciu pripojenia
class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Zariadenie pripojené");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Zariadenie odpojené");
  }
}

void setup() {
  Serial.begin(115200);

  // Nastavenie vlastnej MAC adresy
  esp_base_mac_addr_set(customMacAddr);
  Serial.print("Vlastná MAC adresa nastavená na: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", customMacAddr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  // Inicializácia BLE Keyboard
  BLEDevice::init("JBL Xtreme");
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  bleKeyboard.begin();
}

void loop() {
  if (deviceConnected && bleKeyboard.isConnected()) {
    // Odoslanie WWW_HOME mediálneho tlačidla
    Serial.println("Odosielam tlačidlo: WWW_HOME");
    bleKeyboard.write(KEY_MEDIA_WWW_HOME);
    delay(5000); // Odoslanie každých 5 sekúnd
  }

  delay(100);
}