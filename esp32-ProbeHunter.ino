#include <WiFi.h>
#include "esp_wifi.h"

#define MAX_CHANNEL 12
int currentChannel = 1;
unsigned long lastChannelSwitch = 0;
const unsigned long channelSwitchInterval = 500; // ms

const int allowedSignalStrength = -40;  // Do -40 dBm
bool showDistance = true;

// Pomocná funkcia na prepočet MAC výrobcu
String getVendor(String mac) {
  mac.toUpperCase();
  if (mac.startsWith("DC:44:6D")) return "Apple";
  if (mac.startsWith("FC:C2:DE")) return "Samsung";
  if (mac.startsWith("00:9A:CD")) return "Huawei";
  if (mac.startsWith("E8:AB:FA")) return "Xiaomi";
  if (mac.startsWith("44:65:0D")) return "OnePlus";
  if (mac.startsWith("3C:5A:B4")) return "Motorola";
  if (mac.startsWith("AC:37:43")) return "Sony";
  if (mac.startsWith("A4:77:33")) return "HTC";
  if (mac.startsWith("00:14:22")) return "Dell";
  if (mac.startsWith("00:1A:79")) return "ASUS";
  if (mac.startsWith("F8:27:93")) return "Lenovo";
  return "Neznámy výrobca";
}

// Prevod MAC do stringu
String macToString(const uint8_t* mac) {
  char buf[18];
  snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

// Prepočet RSSI -> približná vzdialenosť
float calculateDistance(int rssi) {
  int txPower = -40; // typická hodnota
  return pow(10, (txPower - rssi) / 20.0);
}

// Callback
void snifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return;

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t*)buf;
  const uint8_t *payload = ppkt->payload;
  int len = ppkt->rx_ctrl.sig_len;
  int rssi = ppkt->rx_ctrl.rssi;

  // Kontrola minimálnej dĺžky rámca
  if (len < 26) return;

  // 0x40 = Probe Request
  if (payload[0] != 0x40) return;

  if (rssi >= allowedSignalStrength) {
    String mac = macToString(payload + 10);
    String vendor = getVendor(mac);
    String ssid = "";

    uint8_t ssidLen = payload[25];
    if (ssidLen > 0 && ssidLen < 32 && (26 + ssidLen) <= len) {
      for (int i = 0; i < ssidLen; i++) {
        ssid += (char)payload[26 + i];
      }
    } else {
      ssid = "(skryté SSID)";
    }

    Serial.println(F("--------------------------------------------------"));
    Serial.println(F("[+] Zachytený Probe Request:"));
    Serial.print("Zariadenie: "); Serial.print(vendor);
    Serial.print(", MAC: "); Serial.println(mac);
    Serial.print("Hľadá sieť SSID: "); Serial.println(ssid);
    Serial.print("RSSI: "); Serial.print(rssi); Serial.print(" dBm");

    if (showDistance) {
      float distance = calculateDistance(rssi);
      Serial.print(", Odhad vzdialenosti: ~");
      Serial.print(distance, 1);
      Serial.println(" m");
    } else {
      Serial.println();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_MODE_NULL);
  esp_wifi_set_promiscuous(true);
  wifi_promiscuous_filter_t filter = { .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT };
  esp_wifi_set_promiscuous_filter(&filter);
  esp_wifi_set_promiscuous_rx_cb(&snifferCallback);
  esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);

  Serial.println(F("🛰️ Skenovanie zariadení pomocou Probe Requestov..."));
}

void loop() {
  unsigned long now = millis();
  if (now - lastChannelSwitch > channelSwitchInterval) {
    currentChannel++;
    if (currentChannel > MAX_CHANNEL) currentChannel = 1;
    esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
    lastChannelSwitch = now;
    Serial.print("➡️  Prepnutý na kanál: ");
    Serial.println(currentChannel);
  }
  delay(10);
}
