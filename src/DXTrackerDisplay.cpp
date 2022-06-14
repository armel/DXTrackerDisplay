// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "settings.h"
#include "DXTrackerDisplay.h"
#include "map.h"
#include "font.h"
#include "tools.h"
#include "functions.h"
#include "tasks.h"

// Setup
void setup()
{
  // Init M5
  auto cfg = M5.config();
  M5.begin(cfg);

  display.begin();

  Serial.println(display.width());

  // Preferences
  preferences.begin("DXTracker");

  size_t n = sizeof(config) / sizeof(config[0]);
  n = (n / 4) - 1;

  //preferences.putUInt("config", 0);

  configCurrent = preferences.getUInt("config", 0);

  if(configCurrent > n) {
    configCurrent = 0;
    preferences.putUInt("config", configCurrent);
  }

  // LCD
  display.fillScreen(display.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
  screensaver = millis(); // Screensaver update !!!

  // SPIFFS
  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS Mount Failed");
    Serial.println("SPIFFS Formating...");
    SPIFFS.format(); // Format SPIFFS...
  }

  // Title
  display.setFont(&rounded_led_board_730pt7b);
  display.setTextColor(TFT_WHITE, display.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
  display.setTextDatum(CC_DATUM);
  display.drawString(String(NAME), WIDTH / 2, 100);
  display.setFont(&Rounded_Elegance14pt7b);
  display.drawString("Version " + String(VERSION) + " by F4HWN", WIDTH / 2, 200);

  // QRCode
  display.qrcode("https://github.com/armel/DXTracker", 312, 308, 400, 6);

  // We start by connecting to the WiFi network
  display.setTextPadding(WIDTH);

  while(true)
  {
    uint8_t attempt = 1;
    display.drawString(String(config[(configCurrent * 4)]), WIDTH / 2, 240);
    WiFi.begin(config[(configCurrent * 4)], config[(configCurrent * 4) + 1]);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      if(attempt % 2 == 0)
      {
        display.drawString("Connecting in progress", WIDTH / 2, 280);
      }
      else 
      {
        display.drawString(" ", WIDTH / 2, 280);
      }
      attempt++;
      if(attempt > 10) {
        break;
      }
    }
    if(WiFi.status() != WL_CONNECTED)
    {
      configCurrent += 1;
      if(configCurrent > n) {
        configCurrent = 0;
        preferences.putUInt("config", configCurrent);
      }
    }
    else {
      break;    
    }
  }

  display.drawString(String(WiFi.localIP().toString().c_str()), WIDTH / 2, 280);

  // Init and get time
  configTzTime(ntpTimeZone, ntpServer);
  updateLocalTime();

  // Scroll
  posA = display.width() - 98;
  imgA.createSprite(posA, 40);

  posB = display.width() - 98;
  imgB.createSprite(posB, 28);

  delay(500);

  // Clear screen
  for (uint16_t i = 0; i <= (HEIGHT / 2); i++)
  {
    display.drawFastHLine(0, i, WIDTH, TFT_BLACK);
    display.drawFastHLine(0, HEIGHT - i, WIDTH, TFT_BLACK);
    delay(2);
  }

  // Select map
  greylineSelect = preferences.getUInt("map", 0);

  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setFont(&Rounded_Elegance14pt7b);
  display.setTextDatum(CC_DATUM);
  display.setTextPadding(WIDTH / 2);

  display.drawJpg(map_greyline, sizeof(map_greyline), 128, 20, 320, 240);
  display.drawJpg(map_sunmap, sizeof(map_sunmap), 576, 20, 320, 240);

  temporisation = millis();
  while(millis() - temporisation < TIMEOUT_MAP) {
    getButton();
    if(btnA == 1) {
      if(greylineSelect == 0)
        greylineSelect = 1;
      else if (greylineSelect == 1)
        greylineSelect = 0;
    }
    else if(btnL) {
      break;
    }

    if(greylineSelect == 0) {
        display.drawString("Current map", 288, 240);
        display.drawString("", 736, 240);
    } else {
        display.drawString("", 288, 240);
        display.drawString("Current map", 736, 240);
    }
    delay(100);   
  }

  if(preferences.getUInt("map", 0) != greylineSelect)
  {
    preferences.putUInt("map", greylineSelect);
  }

  // Let's go after temporisation
  delay(250);

  // Waiting for data
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setFont(&Rounded_Elegance14pt7b);
  display.setTextDatum(CC_DATUM);
  display.setTextPadding(320);

  while(greylineData == "" || hamQSLData == "" || hamQTHData == "" || satData == "") 
  {
    display.drawString("Loading datas", WIDTH / 2, 380);
    delay(250);
    display.drawString(" ", WIDTH / 2, 280);
    delay(250);
    display.drawString("It takes a while, so please wait !", WIDTH / 2, 440);

    if(greylineData == "")
    {
      getGreyline();
      if(greylineData != "")
      {
        display.drawString("Greyline Ok", WIDTH / 2, 500);
      }
    }

    if(hamQSLData == "")
    {
      getHamQSL();
      if(hamQSLData != "")
      {
        display.drawString("Solar Ok", WIDTH / 2, 540);
      }
    }

    if(hamQTHData == "")
    {
      getHamQTH();
      if(hamQTHData != "")
      {
        display.drawString("Cluster Ok", WIDTH / 2, 580);
      }
    }

    if(satData == "")
    {
      getHamSat();
      if(satData != "")
      {
        display.drawString("Sat Ok", WIDTH / 2, 620);
      }
    }
  }
  
  // Multitasking task for retreive propag data
  xTaskCreatePinnedToCore(
      hamdata,        // Function to implement the task
      "hamdata",      // Name of the task
      16384,          // Stack size in words
      NULL,           // Task input parameter
      1,              // Priority of the task
      &hamdataHandle, // Task handle
      0);             // Core where the task should run

  // Multitasking task for retreive button
  xTaskCreatePinnedToCore(
      button,         // Function to implement the task
      "button",       // Name of the task
      8192,           // Stack size in words
      NULL,           // Task input parameter
      1,              // Priority of the task
      &buttonHandle,  // Task handle
      1);             // Core where the task should run

  delay(500);

  // Clear screen
  for (uint16_t i = 0; i <= (HEIGHT / 2); i++)
  {
    display.drawFastHLine(0, i, WIDTH, TFT_BLACK);
    display.drawFastHLine(0, HEIGHT - i, WIDTH, TFT_BLACK);
    delay(2);
  }

  // And clear
  screenRefresh = 1;
}

// Main loop
void loop()
{
  // Let's clean if necessary
  clear();

  // View propag datas
  propagData();

  // Prepare cluster and sat scroll message
  clusterAndSatMessage();

  // Prepare propag scroll message
  propagMessage();

  // View greyline
  greyline();

  // Manage scroll
  scroll();

  // Manage alternance
  if(millis() - temporisation > TIMEOUT_TEMPORISATION) {
    temporisation = millis();
    alternance = (alternance++ > 10) ? 0 : alternance;
    if(alternance == 0) {
      reload = 0;
      updateLocalTime(); // Update local time
      Serial.println(dateString);
      messageCurrent = (messageCurrent++ < 3) ? messageCurrent : 0;
    }
    //Serial.println(alternance);
    //Serial.println(messageCurrent);
  }
}