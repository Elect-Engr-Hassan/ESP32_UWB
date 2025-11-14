#include <SPI.h>
#include "DW1000Ranging.h"

// SPI pins for ESP32
#define SPI_SCK  18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS    5

// DW1000 connection pins
const uint8_t PIN_RST = 27;
const uint8_t PIN_IRQ = 34;

// Unique ID for Tag
#define TAG_UID "7D:00:22:EA:82:60:3B:9C"

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Start SPI bus
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  // Initialize DW1000 as Tag
  DW1000Ranging.initCommunication(PIN_RST, DW_CS, PIN_IRQ);

  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  DW1000Ranging.startAsTag(TAG_UID, DW1000.MODE_LONGDATA_RANGE_ACCURACY);
 // DW1000Ranging.startAsTag(TAG_UID, DW1000.MODE_SHORTDATA_FAST_LOWPOWER);

  Serial.println("### Tag Started ###");
}

void loop() {
  DW1000Ranging.loop();
}

// Callback when a new range from Anchor is received
void newRange() {
  Serial.print("Distance to Anchor: ");
  Serial.print(DW1000Ranging.getDistantDevice()->getRange());
  Serial.println(" m");
}

// Callback when a new Anchor is detected
void newDevice(DW1000Device* device) {
  Serial.print("New Anchor detected: ");
  Serial.println(device->getShortAddress(), HEX);
}

// Callback when device goes inactive
void inactiveDevice(DW1000Device* device) {
  Serial.print("Anchor removed: ");
  Serial.println(device->getShortAddress(), HEX);
}
