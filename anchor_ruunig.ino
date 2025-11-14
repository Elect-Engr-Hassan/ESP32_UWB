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

// Unique ID for Anchor
#define ANCHOR_UID "83:17:5B:D5:A9:9A:E2:9C"

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Start SPI bus
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  
  // Initialize DW1000 as Anchor
  DW1000Ranging.initCommunication(PIN_RST, DW_CS, PIN_IRQ);
  
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachBlinkDevice(newBlink);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  DW1000Ranging.startAsAnchor(ANCHOR_UID, DW1000.MODE_LONGDATA_RANGE_ACCURACY, false);
//   DW1000Ranging.startAsAnchor(ANCHOR_UID, DW1000.MODE_SHORTDATA_FAST_LOWPOWER, false);

  Serial.println("### Anchor Started ###");
}

void loop() {
  DW1000Ranging.loop();
}

// Callback when a new range is received from a Tag
void newRange() {
  Serial.print("Tag Short Address: ");
  Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\t Distance: ");
  Serial.print(DW1000Ranging.getDistantDevice()->getRange());
  Serial.println(" m");
}

// Callback when a new device (Tag) blinks
void newBlink(DW1000Device* device) {
  Serial.print("New Tag detected: ");
  Serial.println(device->getShortAddress(), HEX);
}

// Callback when device goes inactive
void inactiveDevice(DW1000Device* device) {
  Serial.print("Inactive Tag removed: ");
  Serial.println(device->getShortAddress(), HEX);
}
