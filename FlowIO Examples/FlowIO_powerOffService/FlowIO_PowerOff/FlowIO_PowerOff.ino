/*This example implements a service with one characteristic for powering off the FlowIO.
*/

#include <bluefruit.h>
#include <FlowIO.h>

#define DEVICE_NAME "nrf52 PWR Control"// Device Name: Maximum 30 bytes

FlowIO flowio;

BLEService powerOffService;
BLECharacteristic chrPowerOffNow;
BLECharacteristic chrPowerOffWhenInactiveFor;
BLECharacteristic chrPowerOffRemainingTime;

void setup() {
  Serial.begin(9600);
  flowio = FlowIO(GENERAL);
  Bluefruit.begin();
  Bluefruit.setName(DEVICE_NAME);
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  createPowerOffService(); //This is defined in the file "powerService.ino"
  startAdvertising();
}

void startAdvertising(void) {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(powerOffService);
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.start(0);
}

void loop() {
  delay(1000);
  powerOffIfInactiveTimeLimitReached();
}

void connect_callback(uint16_t conn_handle){ // callback invoked when central connects  
  resetOffTimer();
}
void disconnect_callback(uint16_t conn_handle, uint8_t reason){
  resetOffTimer();
}
