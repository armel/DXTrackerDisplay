// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Multitasking task for retreive propag data and greyline

void hamdata(void *pvParameters)
{
  for (;;)
  {
    if(alternance == 0 && reload == 0)
    {
      switch (messageCurrent)
      {
        case 0: getGreyline(); break;
        case 1: getHamSat(); getHamQTH(); break;
        case 2: getGreyline(); break;
        //case 3: getHamQSL(); break;
      }
      reload = 1;
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

// Manage buttons
void button(void *pvParameters)
{
  int8_t change;

  for (;;)
  {
    getButton();

    if (btnA || btnL)
    {
      change = alternance;
      
      if(btnL)
      {
        messageCurrent = (messageCurrent++ < 3) ? messageCurrent : 0;
        posA = 80;
        posB = 80;
      }
      else if(btnA)
      {
        change = (change++ > 10) ? 0 : change;
        alternance = change;
        temporisation = millis(); // Temporisation update !!!
        screenRefresh = 1;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}