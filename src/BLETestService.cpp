
/**
  * Class definition for the custom MicroBit Button Service.
  * Provides a BLE service to remotely read the state of each button, and configure its behaviour.
  */
#include "MicroBitConfig.h"
#include "ble/UUID.h"  // BSIEVER: UUID class was modified to return LSB first for long UUIDs

#include "BLETestService.h"


/*
UUIDs: Type 1 generated from https://www.uuidgenerator.net/ ~4:15pm CDT MAy 9th, 2020
Service:  
    1d93af38-9239-11ea-bb37-0242ac130002

Read Data: 
    1d93b2f8-9239-11ea-bb37-0242ac130002:  Conatins lower case english alphabet: "abcdefghijklmnopqrstuvwxyz"  (26 bytes)

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

/**
  * Constructor.
  * Create a representation of the BLETestService
  * @param _ble The instance of a BLE device that we're running on.
  */
BLETestService::BLETestService(BLEDevice &_ble) :
        ble(_ble)
{
    UUID serviceUUID("1d93af38-9239-11ea-bb37-0242ac130002");
    UUID readDataUUID("1d93b2f8-9239-11ea-bb37-0242ac130002");
    // Create the data structures that represent each of our characteristics in Soft Device.
    GattCharacteristic  readCharacteristic(
                                readDataUUID.getBaseUUID(), 
                                (uint8_t *)"abcdefghijklmnopqrstuvwxyz", 
                                26,
                                26, 
                                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);



    GattCharacteristic *characteristics[] = {&readCharacteristic};
    GattService         service(serviceUUID.getBaseUUID(), characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

    readDataCharacteristicHandle = readCharacteristic.getValueHandle();

    ble.gattServer().write(readDataCharacteristicHandle,(uint8_t *)"abcdefghijklmnopqrstuvwxyz", 26);
}

