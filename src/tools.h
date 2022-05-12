// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

int btnA, btnL;

// Get button
void getButton()
{
  M5.update();

  btnA = M5.BtnA.wasPressed();
  btnL = M5.BtnA.pressedFor(1000);

  //Serial.print(btnA);
  //Serial.println(btnL);
}

// Shutdown
void shutdown()
{
  M5.Power.powerOff();
}