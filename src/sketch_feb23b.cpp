#include <Arduino.h>
#include <KeyboardOutputCallbacks.h>
#include <BleConnectionStatus.h>
#include <BleKeyboard.h>
// #include <Keyboard.h>
//
//
// Debounced button task
//
// Don't use D6 to D11

// 18
// 5 - volts (pulldown)
// 17 - volts (pulldown)
// 16 - volts (pulldown)
// 4 - volts (pulldown)
// 15 - volts (pulldown)
// 8 :(
// 6 !!
// 7 - volts (pulldown)

//
const byte interruptPin = 18;
const byte interruptPin2 = 18;

unsigned long lastDetection = 0;
unsigned long debounceTime = 250;

SemaphoreHandle_t syncSemaphore, syncSemaphore2;

void IRAM_ATTR handleInterrupt()
{
  xSemaphoreGiveFromISR(syncSemaphore, NULL);
}

void IRAM_ATTR handleInterrupt2()
{
  xSemaphoreGiveFromISR(syncSemaphore, NULL);
}

void setup()
{
  // Keyboard.begin();
  Serial.begin(115200);
  Serial.println("Hi :-)");

  syncSemaphore = xSemaphoreCreateBinary();
  syncSemaphore2 = xSemaphoreCreateBinary();

  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), handleInterrupt2, RISING);
}

void loop()
{
  if (xSemaphoreTake(syncSemaphore, (TickType_t)100) && millis() - lastDetection > debounceTime)
  {
    Serial.println("press detected");
    lastDetection = millis();
  }
  if (xSemaphoreTake(syncSemaphore2, (TickType_t)100) && millis() - lastDetection > debounceTime)
  {
    Serial.println("press detected on second button");

    lastDetection = millis();
  }
}

// struct Button
// {
//   const uint8_t PIN;
//   volatile bool pressed;
// };

// Button button1 = {BONE, false};

// void IRAM_ATTR isr()
// {
//   Serial.println("button pressed");
//   digitalWrite(PIN2, HIGH);
//   digitalWrite(PIN2, LOW);
//   button1.pressed = true;
// }

// BleKeyboard bleKeyboard;

// bool isOn = false;

// void setup()
// {
//   Serial.begin(115200);
//   Serial.println("Setting things up...");

//   Serial.println("Starting BLE work!");
//   pinMode(BONE, INPUT_PULLDOWN);
//   pinMode(PIN2, OUTPUT);
//   attachInterrupt(button1.PIN, isr, RISING);
//   bleKeyboard.begin();
// }

// void loop()
// {
//   if (button1.pressed) {
//     bleKeyboard.press('n');
//     Serial.printf("Button 1 has been pressed some times\n");
//     button1.pressed = false;
//   }

//  if(!digitalRead(B1)) {
//    isOn = !isOn;
//  }
//
//  if (isOn) {
//    digitalWrite(PIN2, HIGH);
//    bleKeyboard.print("test!");
//  } else {
//    digitalWrite(PIN2, LOW);
//  }

//Detach Interrupt after 1 Minute
// static uint32_t lastMillis = 0;
// if (millis() - lastMillis > 60000)
// {
//   lastMillis = millis();
//   detachInterrupt(button1.PIN);
//   Serial.println("Interrupt Detached!");
// }
//  } else {
//    Serial.println("Disconnected!");
//    delay(1000);
//  }
//    if (bleKeyboard.isConnected()) {
//      Serial.println("Sending 'Hello world'...");
//      bleKeyboard.print("Hello world");
//
//      delay(1000);
//
//      Serial.println("Sending Enter key...");
//      bleKeyboard.write(KEY_RETURN);
//
//      delay(1000);
//
//      Serial.println("Sending Play/Pause media key...");
//      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
//
//      delay(1000);
//
//      Serial.println("Sending Ctrl+Alt+Delete...");
//      bleKeyboard.press(KEY_LEFT_CTRL);
//      bleKeyboard.press(KEY_LEFT_ALT);
//      bleKeyboard.press(KEY_DELETE);
//      delay(100);
//      bleKeyboard.releaseAll();
//  }
// }
