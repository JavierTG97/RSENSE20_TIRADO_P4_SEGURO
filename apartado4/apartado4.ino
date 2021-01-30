#include <WiFi.h>
#include "time.h"
//#include <HTTPClient.h>

//Red WiFi a la que nos conectamos:
const char* ssid       = "OrdenadorJavier";
const char* password   = "holaquetal";
// Puerto y dirección IP
const uint16_t port = 21;
const char * host = "192.168.137.1";
//Generamos el cliente que se conecta al la IP del host:
WiFiClient client;

const char* ntpServer = "pool.ntp.org";
//Madrid: UTC + 1h -> offset = 3600 segundos.
const long  gmtOffset_sec = 3600;
//Como España tiene periodos en los que se adelanta/retrasa la hora en 1,
//Esto se indica a continuacion:
const int   daylightOffset_sec = 3600;
//Struct para captar el tiempo:
struct tm timeinfo;


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

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());

  //disconnect WiFi as it's no longer needed
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
}

void loop()
{ 
  if (!client.connect(host, port)) {
      Serial.println("Connection to host failed");
      delay(1000);
      return;
  }

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  client.print(&timeinfo);
  delay(1000);
  //printLocalTime();
  //Serial.println(WiFi.localIP()); 
}
