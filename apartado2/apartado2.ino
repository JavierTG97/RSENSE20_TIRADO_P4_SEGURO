#include <WiFi.h>
#include "time.h"
//#include <HTTPClient.h>

const char* ssid       = "OrdenadorJavier";
const char* password   = "holaquetal";

const char* ntpServer = "pool.ntp.org";

//Madrid: UTC + 1h -> offset = 3600 segundos.
const long  gmtOffset_sec = 3600;

//Como España tiene periodos en los que se adelanta/retrasa la hora en 1,
//Esto se indica a continuacion:
const int   daylightOffset_sec = 3600;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(1000);
  printLocalTime();
  Serial.println(WiFi.localIP());
}
