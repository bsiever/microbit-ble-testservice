#include <mbed.h>

#include "MicroBit.h"
#include "BLETestService.h"

BLETestService *bleTest;
MicroBit uBit;

void onConnected(MicroBitEvent) {
    uBit.display.printChar('O');
    uBit.serial.printf("Connected\n");
}

void onDisconnected(MicroBitEvent) {
    uBit.display.printChar('X');
    uBit.serial.printf("Disconnected\n");
}

int main() {
    uBit.init();

    uBit.display.printChar('-');
    uBit.serial.printf("Start......\n");

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
    bleTest = new BLETestService(*uBit.ble);
    uBit.bleManager.advertise();

    bleTest->run();  // Run the fiber to manage the test service
    // int i=0;
//     while(true) {
//         fiber_sleep(1000);
// //        uBit.display.scroll(i);  // Scrolling takes time...
//         uBit.serial.printf("Count 3: %d\n",i++);
//     }

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}