
#ifndef TEST_SERVICE_H
#define TEST_SERVICE_H

#include "MicroBitConfig.h"
#include "ble/BLE.h"
#include "EventModel.h"


/**
  * Class definition for a MicroBit BLE Button Service.
  * Provides access to live button data via BLE, and provides basic configuration options.
  */
class BLETestService
{
    public:

    /**
      * Constructor.
      * Create a representation of the ButtonService
      * @param _ble The instance of a BLE device that we're running on.
      */
    BLETestService(BLEDevice &_ble);


    private:

    // Bluetooth stack we're running on.
    BLEDevice           &ble;

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t readDataCharacteristicHandle;
};


#endif
