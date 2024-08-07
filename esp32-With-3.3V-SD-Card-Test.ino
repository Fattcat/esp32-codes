#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Tested with esp32 and esp8266 D1 Mini SD Card Module (that is 3.3V logic I think)
// Tested and UNSUCESSFULLY (DIDNT WORK) WITH Classic Arduino 5V SD Card Module

// CONNECTIONS (for esp8266 D1 Mini)
// ------------------------------------
// esp32 --> D1 Mini
// 3.3V  --> 3.3V
// GND --> GND
// D18 --> SCK (or Pin CLK)
// D19 --> MOSI
// D23 --> MISO
// D5  --> CS (or Pin SS)
// ------------------------------------

#define LED_BUILTIN 2
#define CS_PIN 5 // Define CS pin

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize the SD card
  if (!SD.begin(CS_PIN)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  const char* filename = "/0111.txt";

  // Check if file exists
  if (!SD.exists(filename)) {
    Serial.println("File does not exist, creating file.");
    File file = SD.open(filename, FILE_WRITE);
    if (file) {
      file.println("Testing testing 123 123");
      file.close();
      Serial.println("File created and data written.");
    } else {
      Serial.println("Failed to create file.");
      return;
    }
  } else {
    Serial.println("File already exists, opening file.");
    File file = SD.open(filename, FILE_WRITE);
    if (file) {
      file.println("Testing testing 123 123");
      file.close();
      Serial.println("Data written to existing file.");
    } else {
      Serial.println("Failed to open existing file.");
      return;
    }
  }

  // Blink the built-in LED 3 times rapidly
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);  // 100 ms ON
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);  // 100 ms OFF
  }
}

void loop() {
  // Nothing to do here
}
