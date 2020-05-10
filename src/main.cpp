#include <mbed.h>

#include "MicroBit.h"
#include "BLETestService.h"

BLETestService *bleTest;
MicroBit uBit;

void onConnected(MicroBitEvent) {
    uBit.display.scroll("O");
}

void onDisconnected(MicroBitEvent) {
    uBit.display.scroll("X");
}

int main() {
    uBit.init();
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    bleTest = new BLETestService(*uBit.ble);
    uBit.bleManager.advertise();
    uBit.display.scroll("X");
    // uBit.sleep(2000);
    // uBit.serial.printf("Hello7\n");
    // uBit.display.printChar('H');
    // uBit.bleManager.advertise();
    //  for(int i=0;i<100;i++)
    //    uBit.serial.send("OK!!!!\n");

// TODOs:  Figure out task problem;  Figure out crash / halt issue; 


    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}