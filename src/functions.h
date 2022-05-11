// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Parse data
String getValue(String data, char separator, uint16_t index)
{
  int16_t strIndex[] = {0, -1};
  int16_t found = 0;
  int16_t maxIndex = data.length() - 1;

  for (uint16_t i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Clear screen
void clear()
{
  if(screenRefresh == 1) {
    display.clear();
    display.fillRect(0, 0, WIDTH, 145, display.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
    display.drawFastHLine(  0, 0, WIDTH, TFT_WHITE);
    display.drawFastHLine(  0, 145, WIDTH, TFT_WHITE);
    display.drawFastHLine(  0, 289, WIDTH, TFT_WHITE);
  }
}

// Manage message cycle
String binarise()
{
  switch(messageCurrent)
  {
    case 0: return "00"; break;
    case 1: return "01"; break;
    case 2: return "10"; break;
    case 3: return "11"; break;
  }

  return "00";
}

// Get local time
void updateLocalTime()
{
  char timeStringBuff[20];  //20 chars should be enough
  char utcStringBuff[20];   //20 chars should be enough

  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S %d-%m-%y", &timeinfo);
  strftime(utcStringBuff, sizeof(utcStringBuff), "%z", &timeinfo);

  sscanf(utcStringBuff, "%d", &utc);
  utc = utc / 100;

  //Serial.println(utc);

  dateString = String(timeStringBuff);
}

/*
// Build scroll A
void buildScrollA()
{
  int16_t h = 40;
  int16_t w;

  imgA.setFont(&Rounded_Elegance20pt7b); 
  w = imgA.textWidth(messageA) + 80;
  // We could just use fillSprite(color) but lets be a bit more creative...
  while (h--)
    imgA.drawFastHLine(0, h, w, TFT_BLUE);

  // Now print text on top of the graphics
  imgA.setTextColor(TFT_WHITE); // White text, no background colour
  //imgA.setTextWrap(false);      // Turn of wrap so we can print past end of sprite

  // Need to print twice so text appears to wrap around at left and right edges
  imgA.drawString(messageA, posA, 2);
  imgA.drawString(messageA, posA - w, 2);
}

// Scroll A
void scrollA(uint8_t pause)
{
  // Sprite for scroll
  buildScrollA();
  imgA.pushSprite(0, 160);

  posA -= 1;
  if (posA == 0)
  {
    //posA = display.width();
    imgA.setFont(&Rounded_Elegance20pt7b); 
    posA = imgA.textWidth(messageA) + 80;
  }

  vTaskDelay(pdMS_TO_TICKS(pause));
}

// Build scroll B
void buildScrollB()
{
  int16_t h = 20;
  int16_t w;

  imgB.setTextSize(1);          // Font size scaling is x1
  imgB.setTextFont(2);          // Font 2 selected
  w = imgB.textWidth(messageB) + 80;
  // We could just use fillSprite(color) but lets be a bit more creative...
  while (h--)
    imgB.drawFastHLine(0, h, w, TFT_RED);

  // Now print text on top of the graphics
  imgB.setTextColor(M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b)); // Gray text, no background colour
  imgB.setTextWrap(false);      // Turn of wrap so we can print past end of sprite

  // Need to print twice so text appears to wrap around at left and right edges
  imgB.drawString(messageB, posB, 2);
  imgB.drawString(messageB, posB - w, 2);
}

// Scroll B
void scrollB(uint8_t pause)
{
  // Sprite for scroll
  buildScrollB();
  imgB.pushSprite(0, 250);

  posB -= 1;
  if (posB == 0)
  {
    //posB = M5.Lcd.width();
    imgB.setTextSize(1);          // Font size scaling is x1
    imgB.setTextFont(2);          // Font 2 selected
    posB = imgB.textWidth(messageB) + 80;
  }

  vTaskDelay(pdMS_TO_TICKS(pause));
}
*/

// Build scroll A
void buildScrollA()
{
  int16_t h = 30;
  int16_t w;

  imgA.setFont(&Rounded_Elegance14pt7b); 
  w = imgA.textWidth(messageA) + 80;
  // We could just use fillSprite(color) but lets be a bit more creative...
  while (h--)
    imgA.drawFastHLine(0, h, w, TFT_BLACK);

  // Now print text on top of the graphics
  imgA.setTextColor(TFT_WHITE); // White text, no background colour
  imgA.setTextWrap(false);      // Turn of wrap so we can print past end of sprite

  // Need to print twice so text appears to wrap around at left and right edges
  imgA.drawString(messageA, posA, 2);
  imgA.drawString(messageA, posA - w, 2);
}

// Scroll A
void scrollA(uint8_t pause)
{
  // Sprite for scroll
  buildScrollA();
  imgA.pushSprite(0, 180);

  posA -= 2;
  if (posA < 0)
  {
    //posA = M5.Lcd.width();
    imgA.setFont(&Rounded_Elegance14pt7b); 
    posA = imgA.textWidth(messageA) + 80;
  }

  vTaskDelay(pdMS_TO_TICKS(pause));
}

// Build scroll B
void buildScrollB()
{
  int16_t h = 30;
  int16_t w;

  imgB.setFont(&Rounded_Elegance14pt7b); 
  w = imgB.textWidth(messageB) + 80;
  // We could just use fillSprite(color) but lets be a bit more creative...
  while (h--)
    imgB.drawFastHLine(0, h, w, TFT_BLACK);

  // Now print text on top of the graphics
  imgB.setTextColor(M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b)); // Gray text, no background colour
  imgB.setTextWrap(false);      // Turn of wrap so we can print past end of sprite

  // Need to print twice so text appears to wrap around at left and right edges
  imgB.drawString(messageB, posB, 2);
  imgB.drawString(messageB, posB - w, 2);
}

// Scroll B
void scrollB(uint8_t pause)
{
  // Sprite for scroll
  buildScrollB();
  imgB.pushSprite(0, 240);

  posB -= 2;
  if (posB < 0)
  {
    //posB = M5.Lcd.width();
    imgB.setFont(&Rounded_Elegance14pt7b); 
    posB = imgB.textWidth(messageB) + 80;
  }
  
  vTaskDelay(pdMS_TO_TICKS(pause));
}

// Draw title
void title(String title)
{
  static String titleOld;
  static String baselineOld;
  static String reloadStateOld;

  if(screenRefresh == 1 || screenRefresh == 2) {
    titleOld = "";
    baselineOld = "";
    reloadStateOld = "";
    screenRefresh = 0;
  }

  // Title
  if(title != titleOld) { // Refresh
    titleOld = title;
    reloadStateOld = " ";

    display.setTextColor(TFT_WHITE, display.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
    display.setFont(&charlie_dotted49pt7b);
    display.setTextDatum(CC_DATUM);
    display.setTextPadding(WIDTH);
    display.drawString(title, WIDTH / 2, 50);
  }

  if(alternance % 2 == 0)
  {
    tmpString = "Update at " + dateString;
  }
  else if(alternance == 5)
  {
    tmpString = String(WiFi.localIP().toString().c_str());
  }
  else
  {
    tmpString = String(NAME) + " Version " + String(VERSION);
  }

  if(tmpString != baselineOld) { // Refresh
    baselineOld = tmpString;

    display.setTextColor(TFT_WHITE, display.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
    display.setFont(&Rounded_Elegance20pt7b);
    display.setTextDatum(CC_DATUM);
    display.setTextPadding(WIDTH);
    display.drawString(tmpString, WIDTH / 2, 120);
  }

  // On right, view reload data
  tmpString = reloadState;
  
  if(tmpString != reloadStateOld) { // Refresh
    reloadStateOld = tmpString;

    if(tmpString != "") {
      display.drawFastHLine(2, 35, 10, display.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
      display.drawLine(12, 35, 8, 31, display.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
      display.drawLine(12, 35, 8, 39, display.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
    }
    else {
      display.drawFastHLine(2, 35, 10, display.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
      display.drawLine(12, 35, 8, 31, display.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
      display.drawLine(12, 35, 8, 39, display.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
    }

    display.setTextColor(display.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b), display.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
    display.setFont(0);
    display.setTextDatum(ML_DATUM);
    display.setTextPadding(50);
    display.drawString(tmpString, 18, 36);
  }
}

// Draw Propag Data
void propagData()
{
  // Title
  solarData[alternance].toUpperCase();

  // Current value
  tmpString = hamQSLData;
  tmpString.replace("<" + solarKey[alternance] + ">", "(");
  tmpString.replace("</" + solarKey[alternance] + ">", ")");

  parenthesisBegin = tmpString.indexOf("(");
  parenthesisLast = tmpString.indexOf(")");
  if (parenthesisBegin > 0)
  {
    tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
  }

  tmpString.trim();

  title(solarData[alternance] + " " + tmpString);
}

// Draw Propag Message
void propagMessage()
{
  if(binarise().charAt(1) == '0')
  {
    messageB = "VHF Conditions -- ";

    for (uint8_t i = 0; i < 4; i++)
    {
      // Current propagation 50 MHz
      tmpString = hamQSLData;
      tmpString.replace(skipKey[i], "(");
      tmpString.replace("</phenomenon>", ")");
      parenthesisBegin = tmpString.indexOf("(");
      parenthesisLast = tmpString.indexOf(")", parenthesisBegin);
      if (parenthesisBegin > 0)
      {
        tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
      }

      tmpString.trim();

      messageB += skipData[i] + " : " + tmpString;
      messageB += " -- ";
    }

    messageB = messageB.substring(0, messageB.length() - 4);
  }
  else if(binarise().charAt(1) == '1')
  {
    messageB = "HF Conditions -- ";

    // Day
    for(uint8_t i = 0; i <= 3; i += 1)
    {
      tmpString = hamQSLData;
      tmpString.replace(propagKey[i], "(");
      tmpString.replace("</band>", ")");
      parenthesisBegin = tmpString.indexOf("(");
      parenthesisLast = tmpString.indexOf(")", parenthesisBegin);
      if (parenthesisBegin > 0)
      {
        tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
      }
      tmpString.trim();
      //tmpString.toUpperCase();

      messageB += propagKey[i].substring(0, 7) + " Day " + tmpString + " -- ";
    }

    // Night
    for(uint8_t i = 4; i <= 7; i += 1)
    {
      tmpString = hamQSLData;
      tmpString.replace(propagKey[i], "(");
      tmpString.replace("</band>", ")");
      parenthesisBegin = tmpString.indexOf("(");
      parenthesisLast = tmpString.indexOf(")", parenthesisBegin);
      if (parenthesisBegin > 0)
      {
        tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
      }
      tmpString.trim();
      //tmpString.toUpperCase();

      messageB += propagKey[i].substring(0, 7) + " Night " + tmpString + " -- ";
    }
    messageB = messageB.substring(0, messageB.length() - 4);
  }
}

// Draw Cluster Message
void clusterAndSatMessage()
{
  boolean exclude = 0;
  uint8_t counter = 0;
  uint32_t tmp = 0;

  if(binarise().charAt(0) == '0')
  {
    size_t n = sizeof(frequencyExclude)/sizeof(frequencyExclude[0]);

    messageA = "";
    hamQTHData.replace("\n", "|");

    for (uint8_t i = 0; i < 50; i++)
    {
      cluster[i] = getValue(hamQTHData, '|', i);
      frequency[i] = getValue(cluster[i], '^', 1);
      tmp = frequency[i].toInt();
      
      exclude = 0;

      for (uint8_t j = 0; j < n; j++)
      {
        if(abs(tmp - frequencyExclude[j]) <= 2 || tmp > 470000)
        {
          exclude = 1;
          break;
        }
      }

      if(exclude == 0)
      {    
        call[i] = getValue(cluster[i], '^', 0);
        band[i] = getValue(cluster[i], '^', 8);
        country[i] = getValue(cluster[i], '^', 9);

        messageA += call[i] + " " + band[i] + " " + frequency[i] + " " + country[i] + " -- ";
        counter += 1;
      }

      if(counter == 10) 
      {
        break;
      }
    }
    if(messageA != "")
    {
      messageA = "DX Cluster -- " + messageA;
      messageA = messageA.substring(0, messageA.length() - 4);
    }
    else
    {
      messageA = "DX Cluster -- Data acquisition on the way, please wait...";
    }
  }
  else if(binarise().charAt(0) == '1')
  {
    messageA = "";
    if(satData.length() > 32)
    {
      messageA = satData.substring(15, satData.length() - 3);
    }
    if(messageA != "")
    {
      messageA = "Satellites Passes -- " + messageA;
    }
    else
    {
      messageA = "Satellites Passes -- Data acquisition on the way, please wait...";
    }
  }
}

// Draw Greyline
void greyline()
{  
  if(greylineRefresh == 1)
  {
    // Draw greyline
    decoded = JpegDec.decodeFsFile("/greyline.jpg");
    if (decoded) {
      //display.drawJpgFile(SPIFFS, "/greyline.jpg", 0, 101, 320, 139, 0, 11);
      display.drawJpgFile(SPIFFS, "/greyline.jpg", 0, 290, 1024, 512, 0, 34);
      greylineRefresh = 0;
    }
  }
}

// Manage scroll
void scroll()
{
  if(screenRefresh == 1)
  {
    return;
  }

  for(uint16_t i = 0; i < 10; i += 1)
  {
    scrollA(5);
    scrollB(5);
  }
}

// Manage screensaver
void wakeAndSleep()
{
  if (screensaverMode == 0 && millis() - screensaver > TIMEOUT_SCREENSAVER)
  {
    for (uint8_t i = brightnessCurrent; i >= 1; i--)
    {
      setBrightness(i);
      scrollA(0);
      scrollB(0);
      delay(10);
    }
    screensaverMode = 1;
    display.sleep();
  }
  else if (screensaverMode == 1 && millis() - screensaver < TIMEOUT_SCREENSAVER)
  {
    display.wakeup();
    screensaverMode = 0;
    for (uint8_t i = 1; i <= brightnessCurrent; i++)
    {
      setBrightness(i);
      scrollA(0);
      scrollB(0);
      delay(10);
    }
  }
}