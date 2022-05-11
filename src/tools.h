// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

int btnA, btnL;

// Get Battery level
int8_t getBatteryLevel(bool type)
{
  return M5.Power.getBatteryLevel();
}

// Control if charging
bool isCharging()
{
  return M5.Power.isCharging();
}

// Get button
void getButton()
{
  M5.update();

  btnA = M5.BtnA.wasPressed();
  btnL = M5.BtnA.pressedFor(1000);

  //Serial.print(btnA);
  //Serial.println(btnL);
}

// Manage brightness
void setBrightness(uint8_t value)
{
  display.setBrightness(value);
}

// Shutdown
void shutdown()
{
  M5.Power.powerOff();
}