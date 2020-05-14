# microbit-ble-testservice

Micro:bit Bluetooth Test Service project for the micro:bit (and compiled on Platform IO)

The service can be used to test BLE libraries on different platforms.  



Modified Files to build test services and devices using Micro:bits

Requires installation of 

Misc. Notes: 

* Platform IO Project 
* Modified `lib/ble/UUID.h` for proper byte ordering in UUIDs for services (LSB first)
* Modified `lib/microbit-dal_ID360/source/bluetooth/MicroBitBLEManager.cpp` to:
  * remove unneeded services (Events and DFU)
  * Change device name in GAP
  * TODO: Change advertising as needed
  * Bluetooth options are in `mbed_app.json`
  * `platformio.ini` specifies baud rage / etc.
  * `.hex` file will be built  in `.pio/build/bbcmicrobit`
  * Currently requires older version of PlatformIO's nordicnrf51 library (3.0.1; newer versions seem to have compatibility problems, probably due to use of newer and incompatible mbed versions)
* Added `src/BLETestService.cpp` and `src/BLETestService.h` for the test service.
