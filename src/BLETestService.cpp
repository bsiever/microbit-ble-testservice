
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

## Property Abbreviations

| Abbreviation | Meaning | 
|:------------:|:--------|
| R            | Read |
| Wn           | Write without response |
| Wr           | Write with response (ack if less than limit; Nack if over limit) |
| Wa           | Authorized Write (may be rejected) |
| N            | Notifiable |
| I            | Indicatable |


## Service 

| Props | Short desc | UUID | Long Description |
|-------|------------|------|------------------|
| R     |  Data Short    | 1d93b2f8-9239-11ea-bb37-0242ac130002 |  Contains ASCII digits 0-9: "0123456789"  (10 bytes) | 
| R     |  Data Packet   | 1d93b488-9239-11ea-bb37-0242ac130002 | Contains 20 bytes:"abcdefghijklmnopqrst" (full BLE packet) |
| R     |  Data Long     | 1d93b56e-9239-11ea-bb37-0242ac130002 | Contains 62 bytes: "abcdefghijklmnopqrstuvwzyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" (multiple packets) |
| RWn    | Short R/Wn Data  | 1d93b636-9239-11ea-bb37-0242ac130002 | For testing writes up to 20 bytes (readback to confirm)|
| RWr    | Short R/W Data  | 1d93b942-9239-11ea-bb37-0242ac130002 | For testing writes up to 80 bytes (readback to confirm) |
| RWr | Client Disconnect | 1d93c1e4-9239-11ea-bb37-0242ac130002 | Time (in ms) until client will disconnect intentionally |
| RWr | Client Reset (hard disconnect) | 1d93c2c0-9239-11ea-bb37-0242ac130002| Time (in ms) until client will disconnect intentionally |
| RW | Auth Permissions | 1d93b7c6-9239-11ea-bb37-0242ac130002 | 4 ASCII bytes; including an "R" allows Read and "W" allows write |
| RaWa | Auth Data | 1d93b884-9239-11ea-bb37-0242ac130002 | Data for authorization test (8 bytes) |
| RWr | Notifiable counter1 period | 1d93b6fe-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| RWn    | Short R Data  |  1d93c432-9239-11ea-bb37-0242ac130002  | Only 1 byte of data ("-"); For testing Descriptors  ; |
| N | Notifiable counter1 | 1d93bb2c-9239-11ea-bb37-0242ac130002| 4 byte counter; Starts at 1 on enable and counts up |
| RWr | Notifiable counter2 period | 1d93bbea-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Notifiable counter2 | 1d93bc9e-9239-11ea-bb37-0242ac130002| 4 byte counter; Starts at 1 on enable and counts up |
| RWr | Indicatable counter1 period | 1d93bd52-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Indicatable counter1 | 1d93be06-9239-11ea-bb37-0242ac130002| 4 byte counter; Starts at 1 on enable and counts up |
| RWr | Indicatable counter2 period | 1d93bec4-9239-11ea-bb37-0242ac130002 | 4 byte value in ms indicating the period of updated to the notifications of counter 1; 500ms initially|
| N | Indicatable counter2 | 1d93bf82-9239-11ea-bb37-0242ac130002| 4 byte counter; Starts at 1 on enable and counts up |


Todo:
2nd test device for descriptors  & Auth read/write 
| RWr    | Short R/W Data (identical ids)  | 1d93c374-9239-11ea-bb37-0242ac130002 | For testing writes up to 4 bytes (readback to confirm) |
| RWr    | Short R/W Data (identical ids) | 1d93c374-9239-11ea-bb37-0242ac130002 | For testing writes up to 4 bytes (readback to confirm) |

Misc Advertising:
  Scan with name 
  Scan with UUID

Reference: https://www.oreilly.com/library/view/getting-started-with/9781491900550/ch04.html

*/



typedef struct  {
  const char* periodUUID;
  const char* updateUUID;
  GattCharacteristic::Properties_t props; 
} timer_def;

static const timer_def timer_defs[] = {
  {"1d93b6fe-9239-11ea-bb37-0242ac130002", "1d93bb2c-9239-11ea-bb37-0242ac130002", GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY},
  {"1d93bbea-9239-11ea-bb37-0242ac130002", "1d93bc9e-9239-11ea-bb37-0242ac130002", GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY},
  {"1d93bd52-9239-11ea-bb37-0242ac130002", "1d93be06-9239-11ea-bb37-0242ac130002", GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE},
  {"1d93bec4-9239-11ea-bb37-0242ac130002", "1d93bf82-9239-11ea-bb37-0242ac130002", GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE}
};


void BLETestService::onDataWritten(const GattWriteCallbackParams *params) {
#ifdef DEBUG
    uBit.serial.printf("Data Written to handle: %x\n", params->handle);
#endif 
    for(int i=0;i<numTimers;i++) {
      if(params->handle == timerPeriodHandles[i]) {
        // Reset corresponding last update to "now" and count to 0
        timerLastUpdate[i] = system_timer_current_time_us()/1000;
        timerCounts[i] = 0;
      }
    }
    if(params->handle == resetHandle) {
        // Disconnect was written
        resetTimerStart = system_timer_current_time_us()/1000;
        resetTimerPeriod = *((uint32_t *)params->data);
        uBit.serial.printf("New reset period: %d\n", resetTimerPeriod);
    } 
    if(params->handle == disconnectHandle) {
        disconnectTimerStart = system_timer_current_time_us()/1000;
        disconnectTimerPeriod = *((uint32_t *)params->data);
        uBit.serial.printf("New disconnect period: %d\n", disconnectTimerPeriod);
    }
}

void BLETestService::_monitorFiber(void *service_) {
  BLETestService *service = (BLETestService*)service_;

  uBit.serial.printf("Running Fiber!");

  // The actual fiber that monitors the service. 
  while(true) { 
    // uBit.serial.printf(".");
    fiber_sleep(50);
    // Check any running timers and update them
    uint32_t now = system_timer_current_time_us()/1000;
    // Check all timers
    for(int i=0;i<numTimers;i++) {
      uint32_t period;
      uint16_t length = 4;
      service->ble.readCharacteristicValue(service->timerPeriodHandles[i], (uint8_t*)&period, &length);

      if(now-service->timerLastUpdate[i] > period) {
        // Update counter 
        service->timerCounts[i]++;
        uint32_t newVal = service->timerCounts[i];

        // Notify only if someone's listening:
        bool sendUpdate = false;
        service->ble.gattServer().areUpdatesEnabled(*service->timers[i], &sendUpdate);
        if(sendUpdate) {
          // service->ble.gattServer().notify(service->timers[i]->getValueHandle(), &newVal, 4);
          service->ble.updateCharacteristicValue(service->timerUpdateHandles[i], (uint8_t*)&newVal, 4);
          uBit.serial.printf("Updating timer: %d (%X) to %d\n", i, service->timerUpdateHandles[i], service->timerCounts[i]);
        }
        // service->ble.updateCharacteristicValue(service->timerUpdateHandles[i], (uint8_t*)&newVal, 4);
        service->timerLastUpdate[i] = now;
        break;  // Wait until the next time slot for any other updates. 
      }

      if(service->resetTimerPeriod != 0) {
        uint32_t now = system_timer_current_time_us()/1000;

        if(now-service->resetTimerStart > service->resetTimerPeriod) {
          service->resetTimerPeriod = 0;
          // Reset
          uBit.serial.printf("RESET!\n");
          NVIC_SystemReset();
        }
      }

      if(service->disconnectTimerPeriod != 0) {
        uint32_t now = system_timer_current_time_us()/1000;

        if(now-service->disconnectTimerStart > service->disconnectTimerPeriod) {
        uBit.serial.printf("Disconnecting!\n");
          service->disconnectTimerPeriod = 0;
          // Disconnect
          service->ble.disconnect(Gap::LOCAL_HOST_TERMINATED_CONNECTION);
        }
      }
    }
  }
}

void BLETestService::run() {
  (void)invoke(&BLETestService::_monitorFiber, (void*)this); // create fiber and schedule it.
}


void BLETestService::authorizeRead(GattReadAuthCallbackParams *readAuth) {
  char data[] = "\0\0\0\0\0";
  uint16_t length = 4;
    uBit.serial.printf("Read Perm!");
  ble.readCharacteristicValue(authPermisHandle, (uint8_t*)data, &length);
  if(strchr(data, 'R')) {
    readAuth->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
    uBit.serial.printf("Read OK!");
  } else {
    readAuth->authorizationReply =  AUTH_CALLBACK_REPLY_ATTERR_READ_NOT_PERMITTED;
    uBit.serial.printf("Read NACK!");
  }
}

void BLETestService::authorizeWrite(GattWriteAuthCallbackParams *writeAuth) {
  char data[] = "\0\0\0\0\0";
    uBit.serial.printf("Write Perm!");

  uint16_t length = 4;
  ble.readCharacteristicValue(authPermisHandle, (uint8_t*)data, &length);
  if(strchr(data, 'W')) {
    writeAuth->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
    uBit.serial.printf("Write OK!");
  } else {
    writeAuth->authorizationReply =  AUTH_CALLBACK_REPLY_ATTERR_WRITE_NOT_PERMITTED;
    uBit.serial.printf("Write NACK!");
  }  
}

/**
  * Constructor.
  * Create a representation of the BLETestService
  * @param _ble The instance of a BLE device that we're running on.
  */
BLETestService::BLETestService(BLEDevice &_ble) :
        ble(_ble), disconnectTimerStart(0), disconnectTimerPeriod(0), resetTimerStart(0), resetTimerPeriod(0)
{
#ifdef DEBUG
    uBit.serial.printf("BLETestService Constructor\n");
#endif

    UUID serviceUUID("1d93af38-9239-11ea-bb37-0242ac130002");

    readShortChar = new GattCharacteristic( UUID("1d93b2f8-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                            (uint8_t *)"0123456789", 10, 10, 
                                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

   readPacketChar = new  GattCharacteristic( UUID("1d93b488-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                            (uint8_t *)"abcdefghijklmnopqrst", 20, 20, 
                                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

    readLongUUIDChar = new GattCharacteristic( UUID("1d93b56e-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                            (uint8_t *)"abcdefghijklmnopqrstuvwzyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 62, 62, 
                                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

    rwnChar = new GattCharacteristic( UUID("1d93b636-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                              (uint8_t *)"-", 1, 20, 
                                              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);

    rwrChar = new GattCharacteristic( UUID("1d93b942-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                              (uint8_t *)"-", 1, 80, 
                                              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    // Include extended properties and designating the description to be writeable
    uint16_t writeAux = 0x0002;
    ep = new GattAttribute( BLE_UUID_DESCRIPTOR_CHAR_EXT_PROP, (uint8_t*)&writeAux, 2, 2); // 0x2900

    // User Description 
    desc1 = new GattAttribute( BLE_UUID_DESCRIPTOR_CHAR_USER_DESC, (uint8_t*)"Test Test Test Test ", 20, 20, true);

    // Server Config
    uint16_t sval = 0;
    serv = new GattAttribute( BLE_UUID_DESCRIPTOR_SERVER_CHAR_CONFIG, (uint8_t*)&sval, 2, 2);  // 0x2903

    // Presentation format
    // format, exponent, unit, namespace, desc  //2904
    GattCharacteristic::PresentationFormat_t pres = { GattCharacteristic::BLE_GATT_FORMAT_SINT16, 3, GattCharacteristic::BLE_GATT_UNIT_AREA_SQUARE_METRES, 1, 0};
    desc2 = new GattAttribute( BLE_UUID_DESCRIPTOR_CHAR_PRESENTATION_FORMAT, (uint8_t*)&pres, sizeof(pres), sizeof(pres));

    // Misc. special attribute
    misc = new GattAttribute( UUID((UUID::ShortUUIDBytes_t)0x2929), (uint8_t*)&sval, 2, 2);  

    allDescs[0] = ep;
    allDescs[1] = desc1;
    allDescs[2] = desc2;
    allDescs[3] = serv;
    allDescs[4] = misc;
    uint16_t value = 0x0004;
    descChar = new GattCharacteristic( UUID("1d93c432-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                              (uint8_t*)&value, 2, 2, 
                                              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_BROADCAST,
                                              allDescs, 5);


    authPermis = new GattCharacteristic( UUID("1d93b7c6-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                                  (uint8_t *)"NONE", 4, 4, 
                                                  GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    authData = new GattCharacteristic(UUID("1d93b884-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                                  (uint8_t *)"Granted!", 8, 8, 
                                                  GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);


    //   // Set callbacks on Permissions / Data
    authData->setWriteAuthorizationCallback(this, &BLETestService::authorizeWrite);
    authData->setReadAuthorizationCallback(this, &BLETestService::authorizeRead);

    // // Identical IDs
    // ident1 = new GattCharacteristic(UUID("1d93c374-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
    //                             (uint8_t *)"a", 1, 4, 
    //                             GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    // // Identical IDs
    // ident2 = new GattCharacteristic(UUID("1d93c374-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
    //                             (uint8_t *)"b", 1, 4, 
    //                             GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    // Disconnect chars
    uint32_t disconnect = 0;
    discon = new GattCharacteristic(UUID("1d93c1e4-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                (uint8_t *)&disconnect, 4, 4, 
                                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    reset = new GattCharacteristic(UUID("1d93c2c0-9239-11ea-bb37-0242ac130002").getBaseUUID(), 
                                (uint8_t *)&disconnect, 4, 4, 
                                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);


    GattCharacteristic *timerPeriods[numTimers];
    uint32_t defTime = 1000;
    uint32_t updateVal = 0;
    for(int i=0;i<numTimers;i++) {
      // Create each timer's period characteristic
      timerPeriods[i] = new GattCharacteristic( UUID(timer_defs[i].periodUUID).getBaseUUID(),  
                                                (uint8_t*)&defTime, 4, 4, 
                                                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);
      // And notify/indicate characteristic
      timers[i] = new   GattCharacteristic( UUID(timer_defs[i].updateUUID).getBaseUUID(), 
                                                    (uint8_t*)&updateVal, 4, 4, 
                                                   timer_defs[i].props);
    }    

    GattCharacteristic *characteristics[] = { 
                                    readShortChar, 
                                    readPacketChar, 
                                    readLongUUIDChar,
                                    rwnChar, 
                                    rwrChar, 
                                    descChar,
                                    // ident1,
                                    // ident2,
                                    discon, 
                                    reset, 
                                    authPermis, 
                                    authData,
                                    timerPeriods[0], timers[0],
                                    timerPeriods[1], timers[1],
                                    timerPeriods[2], timers[2],
                                    timerPeriods[3], timers[3],
                                    };

    service = new GattService(serviceUUID.getBaseUUID(), characteristics, sizeof(characteristics)/sizeof(GattCharacteristic *));
  
    // Get updates on data writes
    ble.onDataWritten(this, &BLETestService::onDataWritten);

    ble.addService(*service);

    // AFTER adding to service handles are finalized....So get them
    // And delete mem no longer needed
    for(int i=0;i<numTimers;i++) {
      timerPeriodHandles[i] = timerPeriods[i]->getValueHandle();
      timerUpdateHandles[i] = timers[i]->getValueHandle();
      timerLastUpdate[i] = 0;
      timerCounts[i] = 0;
      uBit.serial.printf("Timer %d: Value Handle: %x\n",i, timerPeriodHandles[i]);
//      delete timerPeriods[i];
    }
    disconnectHandle = discon->getValueHandle();
    uBit.serial.printf("Disconnect handle: %X\n", disconnectHandle);
    resetHandle = reset->getValueHandle();
    uBit.serial.printf("Reset handle: %X\n", resetHandle);
    authPermisHandle = authPermis->getValueHandle();
    uBit.serial.printf("Auth Permis handle 6: %X\n", authPermisHandle);

}

