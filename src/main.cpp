#include <Arduino.h>
#include <KeyboardOutputCallbacks.h>
#include <BleConnectionStatus.h>
#include <BleKeyboard.h>
// #include <Keyboard.h>
//
// Debounced button task
//
// "Don't use D6 to D11"

// 27 ...
// 14 ...
// 12 - volts (pulldown)
// 13 - volts (pulldown) <--
// 9 - !!
// 10 - ground (pullup)

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

unsigned long lastDetection = 0;
unsigned long debounceTime = 150;

// Declaring the semaphores
SEM_INIT(bigLeft, 4);
SEM_INIT(bigRight, 15);

SEM_INIT(midA, 17);
SEM_INIT(midB, 16);

SEM_INIT(dpadUp, 13);
SEM_INIT(dpadRight, 5);
SEM_INIT(dpadDown, 18);
SEM_INIT(dpadLeft, 19);

void setup()
{
  // Keyboard.begin();
  Serial.begin(115200);
  Serial.println("Hi :-)");

  SEM_ATTACH(bigLeft);
  SEM_ATTACH(bigRight);

  SEM_ATTACH(midA);
  SEM_ATTACH(midB);

  SEM_ATTACH(dpadUp);
  SEM_ATTACH(dpadRight);
  SEM_ATTACH(dpadDown);
  SEM_ATTACH(dpadLeft);
}

void bigRight_handler()
{
  Serial.println("BigRight handler called");
}

void bigLeft_handler()
{
  Serial.println("BigLeft handler called");
}

void midA_handler()
{
  Serial.println("midA handler called");
}

void midB_handler()
{
  Serial.println("midB handler called");
}

void dpadUp_handler()
{
  Serial.println("dpad up handler called");
}

void dpadRight_handler()
{
  Serial.println("dpad right handler called");
}

void dpadDown_handler()
{
  Serial.println("dpad down handler called");
}

void dpadLeft_handler()
{
  Serial.println("dpad left handler called");
}

void loop()
{
  HANDLER_DISPATCH(bigLeft, 0, bigLeft_handler);
  HANDLER_DISPATCH(bigRight, 0, bigRight_handler);

  HANDLER_DISPATCH(midA, 0, midA_handler);
  HANDLER_DISPATCH(midB, 0, midB_handler);

  HANDLER_DISPATCH(dpadUp, 0, dpadUp_handler);
  HANDLER_DISPATCH(dpadRight, 0, dpadRight_handler);
  HANDLER_DISPATCH(dpadDown, 0, dpadDown_handler);
  HANDLER_DISPATCH(dpadLeft, 0, dpadLeft_handler);
}
