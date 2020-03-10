#include <Arduino.h>
#include <KeyboardOutputCallbacks.h>
#include <BleConnectionStatus.h>
#include <BleKeyboard.h>
#include <BLEUtils.h>
#include <BLEDevice.h>

// "Don't use D6 to D11"
////// LEFT SIDE
// 27 ...
// 14 ...
// 12 - volts (pulldown)
// 13 - volts (pulldown) <--
// 9 - !!
// 10 - ground (pullup)
////// RIGHT SIDE
// 23 - volts (pulldown)
// 22 - volts (pulldown)
// 1 - ground (pullup) but weird!
// 3 - ground (pullup)
// ---<
// 21 - volts (pulldown)
// 19 - volts (pulldown)
// 18 - volts (pulldown)
// 5 - volts (pulldown)
// 17 - volts (pulldown)
// 16 - volts (pulldown)
// 4 - volts (pulldown)
// 15 - volts (pulldown)
// --->
// 8 :(
// 6 !!
// 7 - volts (pulldown)

// #define DEVICE_NAME "LINE Things Trial ESP32"
// #define USER_SERVICE_UUID "91E4E176-D0B9-464D-9FE4-52EE3E9F1552"

#define SEM_INIT(INPUTNAME, PIN)     \
  const byte INPUTNAME##_pin = PIN;  \
  SemaphoreHandle_t INPUTNAME##_sem; \
  void IRAM_ATTR INPUTNAME##_interrupt_handler() { xSemaphoreGiveFromISR(INPUTNAME##_sem, NULL); }

#define SEM_ATTACH(INPUTNAME)                 \
  INPUTNAME##_sem = xSemaphoreCreateBinary(); \
  pinMode(INPUTNAME##_pin, INPUT_PULLDOWN);   \
  attachInterrupt(digitalPinToInterrupt(INPUTNAME##_pin), INPUTNAME##_interrupt_handler, RISING);

#define HANDLER_DISPATCH(INPUTNAME, MAX_WAIT, HANDLER)                                                             \
  if (xSemaphoreTake(INPUTNAME##_sem, (TickType_t)MAX_WAIT) != pdFALSE && millis() - lastDetection > debounceTime) \
  {                                                                                                                \
    lastDetection = millis();                                                                                      \
    HANDLER();                                                                                                     \
  }

BleKeyboard bleKeyboard("!!! :-)", "Bluetooth Device Manufacturer", 100);

unsigned long lastDetection = 0;
unsigned long debounceTime = 200;

// Declaring the semaphores
SEM_INIT(bigB, 19); // ++
SEM_INIT(bigA, 21); // ++

SEM_INIT(midSelect, 5); // ++
SEM_INIT(midStart, 18); // ++

SEM_INIT(dpadUp, 4);     // ++
SEM_INIT(dpadRight, 17); // ++
SEM_INIT(dpadDown, 16);  // ++
SEM_INIT(dpadLeft, 15);  // ++

// BLEServer *thingsServer;
// BLESecurity *pSecurity;
// BLEService *userService;

// class serverCallbacks : public BLEServerCallbacks
// {
//   void onConnect(BLEServer *pServer)
//   {
//     Serial.println("Connected");
//   };

//   void onDisconnect(BLEServer *pServer)
//   {
//     Serial.println("Disconnected");
//   }
// };

// void startAdvertising(void)
// {
//   // Start Advertising
//   BLEAdvertisementData scanResponseData = BLEAdvertisementData();
//   scanResponseData.setFlags(0x06); // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
//   scanResponseData.setName(DEVICE_NAME);

//   thingsServer->getAdvertising()->addServiceUUID(userService->getUUID());
//   thingsServer->getAdvertising()->setScanResponseData(scanResponseData);
//   thingsServer->getAdvertising()->start();
// }

void setup()
{
  Serial.begin(115200);
  // Serial.println("befoasdasdasasdasdre ad");

  // thingsServer = BLEDevice::createServer();
  // Serial.println("nim]]] ad");

  // thingsServer->setCallbacks(new serverCallbacks());
  // Serial.println("beforeasdasd ad");

  // // Setup User Service
  // userService = thingsServer->createService(USER_SERVICE_UUID);
  // Serial.println("before ad");

  // startAdvertising();

  // pSecurity = new BLESecurity();
  // pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
  // pSecurity->setCapability(ESP_IO_CAP_IN);
  // pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
  // Serial.println("Hiii");

  bleKeyboard.begin();

  Serial.println("Hi :-)");

  SEM_ATTACH(bigB);
  SEM_ATTACH(bigA);

  SEM_ATTACH(midSelect);
  SEM_ATTACH(midStart);

  SEM_ATTACH(dpadUp);
  SEM_ATTACH(dpadRight);
  SEM_ATTACH(dpadDown);
  SEM_ATTACH(dpadLeft);
}
void bigB_handler()
{
  Serial.println("BigB handler called");
  if (bleKeyboard.isConnected())
  {
    Serial.println("Connected...");
    bleKeyboard.write(KEY_RETURN);
  }
}

void bigA_handler()
{
  Serial.println("bigA handler called");
}

void midSelect_handler()
{
  Serial.println("midSelect handler called");
  if (bleKeyboard.isConnected())
  {
    bleKeyboard.write('a');
  }
}

void midStart_handler()
{
  Serial.println("midStart handler called");
  if (bleKeyboard.isConnected())
  {
    bleKeyboard.write('b');
  }
}

void dpadUp_handler()
{
  Serial.println("dpad up handler called");
  if (bleKeyboard.isConnected())
  {
    bleKeyboard.write(KEY_UP_ARROW);
  }
}

void dpadRight_handler()
{
  Serial.println("dpad right handler called");
  if (bleKeyboard.isConnected())
  {
    bleKeyboard.write(KEY_RIGHT_ARROW);
  }
}

void dpadDown_handler()
{
  Serial.println("dpad down handler called");
  if (bleKeyboard.isConnected())
  {
    bleKeyboard.press(KEY_DOWN_ARROW);
    bleKeyboard.releaseAll();
  }
}

void dpadLeft_handler()
{
  Serial.println("dpad left handler called");
  if (bleKeyboard.isConnected())
  {
    bleKeyboard.press(KEY_LEFT_ARROW);
    bleKeyboard.releaseAll();
  }
}

void loop()
{
  HANDLER_DISPATCH(bigB, 0, bigB_handler);
  HANDLER_DISPATCH(bigA, 0, bigA_handler);

  HANDLER_DISPATCH(midSelect, 0, midSelect_handler);
  HANDLER_DISPATCH(midStart, 0, midStart_handler);

  HANDLER_DISPATCH(dpadUp, 0, dpadUp_handler);
  HANDLER_DISPATCH(dpadRight, 0, dpadRight_handler);
  HANDLER_DISPATCH(dpadDown, 0, dpadDown_handler);
  HANDLER_DISPATCH(dpadLeft, 0, dpadLeft_handler);
}
