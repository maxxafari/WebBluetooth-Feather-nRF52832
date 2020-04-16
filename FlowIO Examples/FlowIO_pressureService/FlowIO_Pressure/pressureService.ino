
float maxPressureLimits[5] = {0,0,0,0,0};
float minPressureLimits[5] = {0,0,0,0,0};

void createPressureService(void) {
  uint8_t pressureServiceUUID[16]     = {0x05,0xaa,0x00,0x00,0x00,0x00,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b}; //"0b0b0b0b-0b0b-0b0b-0b0b-00000000aa05"
  uint16_t chrPressureValueUUID = 0X2A6D; //"0X2A6D"
  uint8_t chrPressureRequestUUID[16]  = {0x05,0xaa,0x00,0x00,0x00,0xc2,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b}; //"0b0b0b0b-0b0b-0b0b-0b0b-c2000000aa05"
  uint8_t chrMaxPressureLimitsUUID[16]= {0x05,0xaa,0x00,0x00,0x00,0xc3,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b}; //"0b0b0b0b-0b0b-0b0b-0b0b-c3000000aa05"
  uint8_t chrMinPressureLimitsUUID[16]= {0x05,0xaa,0x00,0x00,0x00,0xc4,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b}; //"0b0b0b0b-0b0b-0b0b-0b0b-c4000000aa05"
  
  pressureService = BLEService(pressureServiceUUID);
  pressureService.begin();

  chrPressureValue = BLECharacteristic(chrPressureValueUUID);
  chrPressureValue.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  chrPressureValue.setPermission(SECMODE_ENC_NO_MITM, SECMODE_ENC_NO_MITM);
  chrPressureValue.setFixedLen(4);
  chrPressureValue.begin();

  chrPressureRequest = BLECharacteristic(chrPressureRequestUUID);
  chrPressureRequest.setProperties(CHR_PROPS_WRITE);
  chrPressureRequest.setPermission(SECMODE_ENC_NO_MITM, SECMODE_ENC_NO_MITM);
  chrPressureRequest.setFixedLen(1);
  chrPressureRequest.setWriteCallback(onWrite_chrPressureRequest);
  chrPressureRequest.begin();

  chrMaxPressureLimits = BLECharacteristic(chrMaxPressureLimitsUUID);
  chrMaxPressureLimits.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  chrMaxPressureLimits.setPermission(SECMODE_ENC_NO_MITM, SECMODE_ENC_NO_MITM);
  chrMaxPressureLimits.setFixedLen(20);
  chrMaxPressureLimits.setWriteCallback(onWrite_chrMaxPressureLimits);
  chrMaxPressureLimits.begin();

  chrMinPressureLimits = BLECharacteristic(chrMinPressureLimitsUUID);
  chrMinPressureLimits.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  chrMinPressureLimits.setPermission(SECMODE_ENC_NO_MITM, SECMODE_ENC_NO_MITM);
  chrMinPressureLimits.setFixedLen(20);
  chrMinPressureLimits.setWriteCallback(onWrite_chrMinPressureLimits);
  chrMinPressureLimits.begin();
}

//This is executed when a central device writes to the characteristic.
void onWrite_chrPressureRequest(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  Serial.println("Value Written");  
  if(len==1){
    float num = flowio.getPressure(PSI);
    chrPressureValue.notify(&num,sizeof(num));
  }
}
//I need to first find out how to send a single float value TO the characteristic and store it.
//The data array is 8-bit so I need to read 4 bytes and do some bit shifting. I  may have done
//this already in the flowio library for reading the sensor data.
void onWrite_chrMaxPressureLimits(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  Serial.println("Max Pressure: ");
  if(len==4){
    uint32_t rawData;
    rawData = data[0];
    rawData << 8;
    rawData = rawData | data[1];
    rawData << 8;
    rawData = rawData | data[2];
    rawData << 8;
    rawData = rawData | data[3];
    Serial.println(rawData);
    Serial.println((float) rawData);
  }
}

void onWrite_chrMinPressureLimits(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  if(len==4){
    uint32_t rawData;
    rawData = data[0];
    rawData << 8;
    rawData = rawData | data[1];
    rawData << 8;
    rawData = rawData | data[2];
    rawData << 8;
    rawData = rawData | data[3];
    Serial.println(rawData);
    Serial.println((float) rawData);
  }
}
