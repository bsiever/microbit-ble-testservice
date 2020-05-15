
/**
  * Class definition for the custom MicroBit Button Service.
  * Provides a BLE service to remotely read the state of each button, and configure its behaviour.
  */

#include "BLETestService.h"


/*
UUIDs: Type 1 generated from https://www.uuidgenerator.net/ ~4:15pm CDT MAy 9th, 2020

Advertising: 


Service:  1d93af38-9239-11ea-bb37-0242ac130002
Characteristics: 

Reads & Writes 
 
R = Read; 
Wn = Write without response
Wr = Write with response (ack if less than limit; Nack if over limit)

| Props | Short desc | UUID | Long Description |
|-------|------------|------|------------------|
| R     |  Data Short    | 1d93b2f8-9239-11ea-bb37-0242ac130002 |  Contains ASCII digits 0-9: "0123456789"  (10 bytes) | 
| R     |  Data Packet   | 1d93b488-9239-11ea-bb37-0242ac130002 | Contains 20 bytes:"abcdefghijklmnopqrst" (full BLE packet) |
| R     |  Data Long     | 1d93b56e-9239-11ea-bb37-0242ac130002 | Contains 62 bytes: "abcdefghijklmnopqrstuvwzyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" (multiple packets) |
| RWn    | Short R/Wn Data  | 1d93b636-9239-11ea-bb37-0242ac130002 | For testing writes up to 20 bytes (readback to confirm) (mbed doesn't do long writes)|
| RWr    | Short R/W Data  | 1d93b942-9239-11ea-bb37-0242ac130002 | For testing writes up to 20 bytes (readback to confirm) (mbed doesn't do long writes)|

| RWn    | Short R Data  | 1d93b942-9239-11ea-bb37-0242ac130002 | Only 1 byte of data ("-"); For testing Descriptors  ; |



Notifications
Indications

Read / Write Descriptors

Authorization / Authentication


Long writes?



Client disconnect (clean / requested)
Client disconnect (reboot / abrupt)

Misc Advertising:
Scan with name 
Scan with UUID

| R |  | 1d93b6fe-9239-11ea-bb37-0242ac130002 |  Checksum and size (bytes) of above |
| RW |  | 1d93b7c6-9239-11ea-bb37-0242ac130002 | For testing reads & writes up to 100 bytes|
| R | | 1d93b884-9239-11ea-bb37-0242ac130002 | Checksum and size (bytes) of above |



Reference: https://www.oreilly.com/library/view/getting-started-with/9781491900550/ch04.html


Write with response? 
Notify?
Indicate? 
Signed Write? 
Queued Write? 
Writeable Auxiliries ? 
Force disconnect? 



1d93b942-9239-11ea-bb37-0242ac130002
1d93bb2c-9239-11ea-bb37-0242ac130002
1d93bbea-9239-11ea-bb37-0242ac130002
1d93bc9e-9239-11ea-bb37-0242ac130002
1d93bd52-9239-11ea-bb37-0242ac130002
1d93be06-9239-11ea-bb37-0242ac130002
1d93bec4-9239-11ea-bb37-0242ac130002
1d93bf82-9239-11ea-bb37-0242ac130002
1d93c1e4-9239-11ea-bb37-0242ac130002
1d93c2c0-9239-11ea-bb37-0242ac130002
1d93c374-9239-11ea-bb37-0242ac130002
1d93c432-9239-11ea-bb37-0242ac130002
1d93af38-9239-11ea-bb37-0242ac130002
1d93b2f8-9239-11ea-bb37-0242ac130002
1d93b488-9239-11ea-bb37-0242ac130002
1d93b56e-9239-11ea-bb37-0242ac130002
1d93b636-9239-11ea-bb37-0242ac130002
1d93b6fe-9239-11ea-bb37-0242ac130002
1d93b7c6-9239-11ea-bb37-0242ac130002
1d93b884-9239-11ea-bb37-0242ac130002
1d93b942-9239-11ea-bb37-0242ac130002
1d93bb2c-9239-11ea-bb37-0242ac130002
1d93bbea-9239-11ea-bb37-0242ac130002
1d93bc9e-9239-11ea-bb37-0242ac130002
1d93bd52-9239-11ea-bb37-0242ac130002
1d93be06-9239-11ea-bb37-0242ac130002
1d93bec4-9239-11ea-bb37-0242ac130002
1d93bf82-9239-11ea-bb37-0242ac130002
1d93c1e4-9239-11ea-bb37-0242ac130002
1d93c2c0-9239-11ea-bb37-0242ac130002
1d93c374-9239-11ea-bb37-0242ac130002
1d93c432-9239-11ea-bb37-0242ac130002
*/





void BLETestService::onDataWritten(const GattWriteCallbackParams *params) {
#ifdef DEBUG
    uBit.serial.printf("Data Written......\n");
#endif 
//     if (params->handle == rwrChar->getValueAttribute().getHandle()) {
//         /* Do something here if the new value is 1; else you can override this method by
//           * extending this class.
//           * @NOTE: If you are extending this class, be sure to also call
//           * ble.onDataWritten(this, &ExtendedHRService::onDataWritten); in
//           * your constructor.
//           */
// #ifdef DEBUG
//     uBit.serial.printf("RWR Char......\n");
// #endif 
//     }
}



/**
  * Constructor.
  * Create a representation of the BLETestService
  * @param _ble The instance of a BLE device that we're running on.
  */
BLETestService::BLETestService(BLEDevice &_ble) :
        ble(_ble)
{
#ifdef DEBUG
    uBit.serial.printf("BLETestService Constructor\n");
#endif

    UUID serviceUUID("1d93af38-9239-11ea-bb37-0242ac130002");

    GattCharacteristic  readShortChar( UUID("1d93b2f8-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                            (uint8_t *)"0123456789", 10, 10, 
                                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

    GattCharacteristic  readPacketChar( UUID("1d93b488-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                            (uint8_t *)"abcdefghijklmnopqrst", 20, 20, 
                                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

    GattCharacteristic  readLongUUIDChar( UUID("1d93b56e-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                            (uint8_t *)"abcdefghijklmnopqrstuvwzyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 62, 62, 
                                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

    GattCharacteristic  rwnChar( UUID("1d93b636-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                              (uint8_t *)"-", 1, 20, 
                                              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);

   GattCharacteristic rwrChar(UUID("1d93b942-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                              (uint8_t *)"-", 1, 20, 
                                              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    // Include extended properties and designating the description to be writeable
    uint16_t writeAux = 0x0002;
    GattAttribute  ep( BLE_UUID_DESCRIPTOR_CHAR_EXT_PROP, (uint8_t*)&writeAux, 2, 2); // 0x2900

    // User Description 
    GattAttribute  desc1( BLE_UUID_DESCRIPTOR_CHAR_USER_DESC, (uint8_t*)"Test", 4, 20);

    // Server Config
    uint16_t sval = 0;
    GattAttribute  serv( BLE_UUID_DESCRIPTOR_SERVER_CHAR_CONFIG, (uint8_t*)&sval, 2, 2);  // 0x2903

    // Presentation format
    // format, exponent, unit, namespace, desc  //2904
    GattCharacteristic::PresentationFormat_t pres = { GattCharacteristic::BLE_GATT_FORMAT_SINT16, 3, GattCharacteristic::BLE_GATT_UNIT_AREA_SQUARE_METRES, 1, 0};
    GattAttribute  desc2( BLE_UUID_DESCRIPTOR_CHAR_PRESENTATION_FORMAT, (uint8_t*)&pres, sizeof(pres), sizeof(pres));

    // Misc. special attribute
    GattAttribute  misc( UUID((UUID::ShortUUIDBytes_t)0x2929), (uint8_t*)&sval, 2, 2);  

    GattAttribute   *allDescs[] = { &ep, &desc1, &desc2, &serv, &misc};
    uint16_t value = 0x0004;
    GattCharacteristic  descChar( UUID("1d93b942-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                              (uint8_t*)&value, 2, 2, 
                                              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_BROADCAST,
                                              allDescs, sizeof(allDescs)/sizeof(GattAttribute*));


    GattCharacteristic *characteristics[] = {&readShortChar, &readPacketChar, &readLongUUIDChar, &rwnChar, &rwrChar, &descChar};
    GattService         service(serviceUUID.getBaseUUID(), characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *));
  //  ble.onDataWritten(this, &BLETestService::onDataWritten);

    ble.addService(service);
}

