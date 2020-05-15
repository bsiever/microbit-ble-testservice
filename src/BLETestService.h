
#ifndef TEST_SERVICE_H
#define TEST_SERVICE_H

#include "MicroBitConfig.h"
#include "ble/BLE.h"
#include "EventModel.h"
#include "ble/UUID.h"  // BSIEVER: UUID class was modified to return LSB first for long UUIDs

#define DEBUG 1

#ifdef DEBUG
#include "MicroBit.h"
extern MicroBit uBit;
#endif


const int numTimers = 4;


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

  void run(); 

private:

  // Bluetooth stack we're running on.
  BLEDevice           &ble;

  GattAttribute::Handle_t timerPeriodHandles[numTimers];
  GattAttribute::Handle_t timerUpdateHandles[numTimers];
  uint32_t timerLastUpdate[numTimers];
  uint32_t timerCounts[numTimers];


  // Misc event handlers
  void onDataWritten(const GattWriteCallbackParams *params);
  static void _monitorFiber(void *service);
};


#endif
