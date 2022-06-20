// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#define VERSION "0.0.5"
#define AUTHOR "F4HWN"
#define NAME "DXTrackerDisplay"

#define DEBUG 0

#define TIMEOUT_MAP                5 * 1000 // 5 sec
#define TIMEOUT_TEMPORISATION      5 * 1000 // 5 sec

#define WIDTH 1024
#define HEIGHT 768

#define M5ATOMDISPLAY_LOGICAL_WIDTH  WIDTH    // width
#define M5ATOMDISPLAY_LOGICAL_HEIGHT  HEIGHT  // height
#define M5ATOMDISPLAY_REFRESH_RATE 60         // refresh rate

#undef min

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <M5AtomDisplay.h>
#include <M5Unified.h>

M5AtomDisplay display(WIDTH, HEIGHT);

// Wifi
WiFiClient client;
HTTPClient http;

// Preferences
Preferences preferences;

// Color
typedef struct __attribute__((__packed__))
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} colorType;

colorType TFT_BACK = {48, 48, 48};
colorType TFT_GRAY = {128, 128, 128};

// Timezone
const char* ntpServer = "pool.ntp.org";
const char* ntpTimeZone = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"; // For Europe/Paris
//const char* ntpTimeZone = "CET-1CEST,M3.5.0,M10.5.0/3"; // For Europe/Brussels
//const char* ntpTimeZone = "EET-2EEST,M3.5.0/3,M10.5.0/4"; // For Europe/Sofia 
//const char* ntpTimeZone = "EST5EDT,M3.2.0,M11.1.0"; // For America/Montreal
//const char* ntpTimeZone = "AST4"; // For America/Martinique
//const char* ntpTimeZone = "AST4"; // For America/Guadeloupe
//const char* ntpTimeZone = "NCT-11"; // For Pacific/Noumea

int utc = 1;

// HTTP endpoint
const String endpointHamQSL = "http://www.hamqsl.com/solarxml.php";
const String endpointSat = "http://rrf2.f5nlg.ovh:8080/cgi-bin/DXSat.py";
const String endpointHamQTH = "http://rrf2.f5nlg.ovh:8080/cgi-bin/DXCluster.py";

const String endpointGreyline[2] = {
  "http://rrf2.f5nlg.ovh:8080/greylinebig.jpg",
  "http://rrf2.f5nlg.ovh:8080/sunmapbig.jpg"
};

// Scroll
LGFX_Sprite imgA(&display); // Create Sprite object "img" with pointer to "tft" object
String messageA = "";
int16_t posA;

LGFX_Sprite imgB(&display); // Create Sprite object "img" with pointer to "tft" object
String messageB = "";
int16_t posB;

// Propag data
String solarData[] = {
  "SFI", "Sunspots", "A-Index", "K-Index", 
  "X-Ray", "Helium Line", "Proton Flux", "Electron Flux", 
  "Aurora", "Solar Wind", "Magnetic Field", "Signal Noise"
};

const String solarKey[] = {
  "solarflux", "sunspots", "aindex", "kindex", 
  "xray", "heliumline", "protonflux", "electonflux", 
  "aurora", "solarwind", "magneticfield", "signalnoise"
};

const String skipData[] = {
  "E-Skip North America",
  "E-Skip Europe",
  "E-Skip Europe 4m",
  "E-Skip Europe 6m",
};

const String skipKey[] = {
  "location=\"north_america\">", 
  "location=\"europe\">", 
  "location=\"europe_4m\">",  
  "location=\"europe_6m\">" 
};

const String propagKey[] = {
  "80m-40m\" time=\"day\">", 
  "30m-20m\" time=\"day\">", 
  "17m-15m\" time=\"day\">", 
  "12m-10m\" time=\"day\">", 
  "80m-40m\" time=\"night\">",
  "30m-20m\" time=\"night\">",
  "17m-15m\" time=\"night\">",
  "12m-10m\" time=\"night\">"    
};

String cluster, call, frequency, band, country;

// Task Handle
TaskHandle_t hamdataHandle;
TaskHandle_t buttonHandle;

// Miscellaneous
String tmpString;
String dateString;
String greylineData = "", hamQSLData = "", hamQTHData = "", satData = "";
String reloadState = "";

boolean reload = 0;
boolean greylineRefresh = 0;
boolean greylineSelect = 0;

uint8_t screenRefresh = 0;
uint8_t alternance = 0;
uint8_t configCurrent = 0;
uint8_t messageCurrent = 0;

int16_t parenthesisBegin = 0;
int16_t parenthesisLast = 0;

uint32_t temporisation;
long frequencyExclude[] = {
  1840, 1842, 3573, 5357,	
  7056, 7071, 7074, 7078,
  10130, 10132, 10133, 10136, 
  14071, 14074, 14078, 14090,
  18100, 18104, 21074, 21078,
  21091, 24915, 28074, 28078,
  50310, 50313, 50328, 50323,
  70100, 144174, 222065, 432065
};