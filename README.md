# microbit-ble-testservice

Micro:bit Bluetooth Test Service project for the micro:bit (and compiled on Platform IO)

The service can be used to test BLE libraries on different platforms.

# For Future Modifications

* Update `lib/microbit-dal_ID360/source/bluetooth/MicroBitBLEManager.cpp` for
  * Advertising
  * GAP Name
* Update `src/BLETestService.cpp` (and possibly `src/BLETestService.h`) for newer services

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
