/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "time.h"

// Replace with your network credentials
const char* ssid = "OrdenadorJavier";
const char* password = "holaquetal";

// Stores LED state
String ledState;

// ------------------- NTP --------------
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
struct tm timeinfo;
int h, m, s, h_reset=0, m_reset=0, s_reset =0;     //Hora, minuto y segundo
String hora_S;   //String para mostrar en la web la hora
bool flag;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    //Hora actual:
    getLocalTime(&timeinfo);
    h = timeinfo.tm_hour;
    m = timeinfo.tm_min;
    s = timeinfo.tm_sec;

    //Si se ha pulsado reset -> nos guardamos la hora para restarla:
    if (flag) {
      h_reset = h;
      m_reset = m;
      s_reset = s;
      flag = false;
    }

    //Hora reseteada (si no se ha pulsado reset, es la hora normal):
    h = h - h_reset;
    m = m - m_reset;
    s = s - s_reset;
    if (s<0) {
      s = s +60;
      m = m - 1;
    }
    
    //Por si acaso la hora o min o seg son menores que 9, anhadimos 0's:
    if (s<10) {
      hora_S = String(":0" + String(s));
    }
    else{
      hora_S = String(":" + String(s));
    }
    if (m<10) {
      hora_S = String(":0" + String(m) + hora_S);
    }
    else{
      hora_S = String(":" + String(m) + hora_S);
    }
    if (h<10) {
      hora_S = String("0" + String(h) + hora_S);
    }
    else{
      hora_S = String(String(h) + hora_S);
    }    
    return hora_S;
  }
  return String();
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  //Poner en hora el reloj según NTP:
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to set GPIO to HIGH
  server.on("/resete", HTTP_GET, [](AsyncWebServerRequest *request){   
    request->send(SPIFFS, "/index.html", String(), false, processor);
    flag = true;
  });

  // Start server
  server.begin();
}
 
void loop(){
}
