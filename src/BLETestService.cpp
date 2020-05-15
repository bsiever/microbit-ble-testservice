
/**
  * Class definition for the custom MicroBit Button Service.
  * Provides a BLE service to remotely read the state of each button, and configure its behaviour.
  */

#include "BLETestService.h"


/*
UUIDs: Type 1 generated from https://www.uuidgenerator.net/ ~4:15pm CDT May 9th, 2020

Advertising: 


Service:  1d93af38-9239-11ea-bb37-0242ac130002
Characteristics: 

Reads & Writes 
 
R = Read; 
Wn = Write without response
Wr = Write with response (ack if less than limit; Nack if over limit)
N = Notifiable
I = Indicatable

| Props | Short desc | UUID | Long Description |
|-------|------------|------|------------------|
| R     |  Data Short    | 1d93b2f8-9239-11ea-bb37-0242ac130002 |  Contains ASCII digits 0-9: "0123456789"  (10 bytes) | 
| R     |  Data Packet   | 1d93b488-9239-11ea-bb37-0242ac130002 | Contains 20 bytes:"abcdefghijklmnopqrst" (full BLE packet) |
| R     |  Data Long     | 1d93b56e-9239-11ea-bb37-0242ac130002 | Contains 62 bytes: "abcdefghijklmnopqrstuvwzyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" (multiple packets) |
| RWn    | Short R/Wn Data  | 1d93b636-9239-11ea-bb37-0242ac130002 | For testing writes up to 100 bytes (readback to confirm) (mbed doesn't do long writes)|
| RWr    | Short R/W Data  | 1d93b942-9239-11ea-bb37-0242ac130002 | For testing writes up to 100 bytes (readback to confirm) (mbed doesn't do long writes)|
| RWn    | Short R Data  | 1d93b942-9239-11ea-bb37-0242ac130002 | Only 1 byte of data ("-"); For testing Descriptors  ; |
| RWr | Notifiable counter1 period | 1d93b942-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Notifiable counter1 | 1d93bb2c-9239-11ea-bb37-0242ac130002| 2 byte counter; Starts at 1 on enable and counts up |
| RWr | Notifiable counter2 period | 1d93bbea-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Notifiable counter2 | 1d93bc9e-9239-11ea-bb37-0242ac130002| 2 byte counter; Starts at 1 on enable and counts up |
| RWr | Indicatable counter1 period | 1d93bd52-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Indicatable counter1 | 1d93be06-9239-11ea-bb37-0242ac130002| 2 byte counter; Starts at 1 on enable and counts up |
| RWr | Indicatable counter2 period | 1d93bec4-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Indicatable counter2 | 1d93bf82-9239-11ea-bb37-0242ac130002| 2 byte counter; Starts at 1 on enable and counts up |




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



typedef struct  {
  const char* periodUUID;
  const char* updateUUID;
  GattCharacteristic::Properties_t props; 
} timer_def;

static const timer_def timer_defs[] = {
  {"1d93b942-9239-11ea-bb37-0242ac130002", "1d93bb2c-9239-11ea-bb37-0242ac130002", GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY},
  {"1d93bbea-9239-11ea-bb37-0242ac130002", "1d93bc9e-9239-11ea-bb37-0242ac130002", GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY},
  {"1d93bd52-9239-11ea-bb37-0242ac130002", "1d93be06-9239-11ea-bb37-0242ac130002", GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE},
  {"1d93bec4-9239-11ea-bb37-0242ac130002", "1d93bf82-9239-11ea-bb37-0242ac130002", GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE}
};

static const int numTimers = sizeof(timer_defs)/sizeof(timer_def);

// Not very OOP, but I'm assuming this service is a singleton
static GattAttribute::Handle_t timerPeriodHandles[numTimers];
static GattAttribute::Handle_t timerUpdateHandles[numTimers];

static uint32_t timerLastUpdate[numTimers] = {0};
static uint16_t timerCounts[numTimers] = {0};

void BLETestService::onDataWritten(const GattWriteCallbackParams *params) {
#ifdef DEBUG
    uBit.serial.printf("Data Written to handle: %x\n", params->handle);
#endif 
    for(int i=0;i<numTimers;i++) {
      if(params->handle == timerPeriodHandles[i]) {
        uint32_t newValue = *((uint32_t*)(params->data));
#if DEBUG
        uBit.serial.printf("Timer %d updated to Val: %x\n", i, newValue);
#endif
        // Reset corresponding last update to "now" and count to 0
        timerLastUpdate[i] = system_timer_current_time_us()/1000;
        timerCounts[i] = 0;
      }
    }
}

void BLETestService::_monitorFiber(void *service_) {
  BLETestService *service = (BLETestService*)service_;
  // The actual fiber that monitors the service. 
  while(true) { 
    // uBit.serial.printf(".\n");
    fiber_sleep(20);
    // Check any running timers and update them
    uint32_t now = system_timer_current_time_us()/1000;
    // Check all timers
    for(int i=0;i<numTimers;i++) {
      uint32_t period;
      uint16_t length = 4;
      service->ble.readCharacteristicValue(timerPeriodHandles[i], (uint8_t*)&period, &length);

      if(now-timerLastUpdate[i] > period) {
        // Update counter 
        timerCounts[i]++;
        service->ble.updateCharacteristicValue(timerUpdateHandles[i], (uint8_t*)&(timerCounts[i]), 2);
        timerLastUpdate[i] = now;
      }
    }
  }
}

void BLETestService::run() {
      (void)invoke(&BLETestService::_monitorFiber, (void*)this); // create fiber and schedule it.
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
                                              (uint8_t *)"-", 1, 100, 
                                              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);

    GattCharacteristic rwrChar(UUID("1d93b942-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                              (uint8_t *)"-", 1, 100, 
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

// TODO
// Disconnect service

// Authorized writes 



    GattCharacteristic *timerPeriods[numTimers];
    GattCharacteristic *timerNotOrInds[numTimers];
    uint32_t defTime = 500;
    uint16_t updateVal = 0;
    for(int i=0;i<numTimers;i++) {
      // Create each timer's period characteristic
      timerPeriods[i] = new GattCharacteristic( UUID(timer_defs[i].periodUUID).getBaseUUID(),  
                                                (uint8_t*)&defTime, 4, 4, 
                                                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);
      // And notify/indicate characteristic
      timerNotOrInds[i] = new   GattCharacteristic( UUID(timer_defs[i].updateUUID).getBaseUUID(), 
                                                    (uint8_t*)&updateVal, 2, 2, 
                                                    timer_defs[i].props);
    }    

    GattCharacteristic *characteristics[6+2*numTimers] = { &readShortChar, 
                                              &readPacketChar, 
                                              &readLongUUIDChar,
                                              &rwnChar, 
                                              &rwrChar, 
                                              &descChar};
    // Add in the timers
    for(int i=0;i<numTimers;i++) {
      characteristics[6+2*i] = timerPeriods[i];
      characteristics[6+2*i+1] = timerNotOrInds[i];
    }
    GattService         service(serviceUUID.getBaseUUID(), characteristics, 6+2*numTimers);
  
    // Get updates on data writes
    ble.onDataWritten(this, &BLETestService::onDataWritten);

    ble.addService(service);

    // AFTER adding to service handles are finalized....So get them
    // And delete mem no longer needed
    for(int i=0;i<numTimers;i++) {
      timerPeriodHandles[i] = timerPeriods[i]->getValueHandle();
      timerUpdateHandles[i] = timerNotOrInds[i]->getValueHandle();
      // uBit.serial.printf("Timer %d: Value Handle: %x\n",i, timerPeriodHandles[i]);
      delete timerPeriods[i];
      delete timerNotOrInds[i];
    }

    // Create monitor task??? How???
}

