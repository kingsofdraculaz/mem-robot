#include <Arduino.h>
#include <USB.h>
#include <USBHIDGamepad.h>

USBHIDGamepad gamepad;

const int LX = 5;  
const int LY = 6;  
const int RX = 7;  
const int RY = 8; 

const int BTN1 = 10;
const int BTN2 = 11;
const int BTN3 = 12;
const int BTN4 = 13;

const int adcMax = 4095;
const int deadzone = 120;

int applyDeadzoneAndMap(int val, int center) {
  int delta = val - center;
  if (abs(delta) < deadzone) delta = 0;
  long out = (long)delta * 32767 / (adcMax / 2);
  if (out > 32767) out = 32767;
  if (out < -32768) out = -32768;
  return (int)out;
}

int xCenter, yCenter, rxCenter, ryCenter;

void setup() {
  analogReadResolution(12);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);

  USB.begin();
  gamepad.begin();

  delay(300);
  xCenter  = analogRead(LX);
  yCenter  = analogRead(LY);
  rxCenter = analogRead(RX);
  ryCenter = analogRead(RY);
}

void loop() {
  int lx = applyDeadzoneAndMap(analogRead(LX), xCenter);
  int ly = applyDeadzoneAndMap(analogRead(LY), yCenter);
  int rx = applyDeadzoneAndMap(analogRead(RX), rxCenter);
  int ry = applyDeadzoneAndMap(analogRead(RY), ryCenter);

  bool b1 = digitalRead(BTN1) == LOW;
  bool b2 = digitalRead(BTN2) == LOW;
  bool b3 = digitalRead(BTN3) == LOW;
  bool b4 = digitalRead(BTN4) == LOW;

  uint16_t buttons = 0;
  if (b1) buttons |= (1 << 0);
  if (b2) buttons |= (1 << 1);
  if (b3) buttons |= (1 << 2);
  if (b4) buttons |= (1 << 3);

  gamepad.setAxes(lx, ly, rx, ry);
  gamepad.setButtons(buttons);
  gamepad.sendReport();

  delay(5);
}