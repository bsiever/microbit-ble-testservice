# microbit-ble-testservice
Micro:bit Bluetooth Test Service project for Platform IO

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
  * Added a `build_flags = -D NDEBUG` in `platformio.ini` due to weird crashes. 
  
Added `src/BLETestServie.cpp` and `src/BLETestService.h` for the test service.
