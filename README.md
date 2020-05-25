# microbit-ble-testservice

Micro:bit Bluetooth Test Service project for the micro:bit (and compiled on Platform IO)

The service can be used to test BLE libraries on different platforms.

Be sure to use a recent version of the Micro:bit firmware (>=0253). See [https://microbit.org/get-started/user-guide/firmware/](https://microbit.org/get-started/user-guide/firmware/)

# For Future Modifications

* Update `lib/microbit-dal_ID360/source/bluetooth/MicroBitBLEManager.cpp` for
  * Advertising
  * GAP Name
* Update `src/BLETestService.cpp` (and possibly `src/BLETestService.h`) for newer services
* Update `lib/ble-nrf51822/source/btle/btle.cpp` for long writes
  * `MAX_ATTR_SIZE = 80;` defines size of max attribute
  

# Misc. Notes

* Platform IO Project 
* Modified `lib/ble/UUID.h` for proper byte ordering in UUIDs for services (LSB first).  New constructor to allow either LSB or MSB depending on need. (Some BTLE stack things need one order while mbed things need another)
* Modified `lib/microbit-dal_ID360/source/bluetooth/MicroBitBLEManager.cpp` to:
  * Change device name in GAP
  * Change advertising as needed
  * `.hex` file will be built  in `.pio/build/bbcmicrobit`
  * Currently requires older version of PlatformIO's nordicnrf51 library (3.0.1; newer versions seem to have compatibility problems, probably due to use of newer and incompatible mbed versions)
* Added `src/BLETestService.cpp` and `src/BLETestService.h` for the test service.
* `platformio.ini` specifies baud rate / etc.
* UGHHH.  Some weird bug prevents the 2nd of a series of notify/indicates from notifying/indicating using the normal "update" to a characteristic, but an explicit call only to notify only if enabled seems to fix it. 
* 