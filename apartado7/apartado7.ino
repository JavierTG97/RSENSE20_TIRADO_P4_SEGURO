
// Import required libraries
#include <WiFi.h>
#include "time.h"
#include <ArduinoJson.h>
#include <WiFiClient.h> 
#include <ESP32_FTPClient.h>

// Replace with your network credentials
const char* ssid = "OrdenadorJavier";
const char* password = "holaquetal";

// ------------------- NTP ---------------------------
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
struct tm timeinfo;
char timeHour[3];
char timeMin[3];
char timeSec[3];
char temp[15];
String tempS;
char jsonName[15];

//----------- JSON --------------
//Declaracion (el nº indica el nº de bits que almacena el json)
//Nuestro json contiene 4 objects ("n", "u", "t", "v") y como "v" es un array que contiene 30 elementos, ponemos JSON_ARRAY_SIZE(30)
//const int capacity = 10*JSON_ARRAY_SIZE(30) + JSON_OBJECT_SIZE(4);
StaticJsonDocument<4000> doc;

float valor;
char docChar[200];
bool flag = true;

// --------- SERVER al que volcamos el json --------
char ftp_server[] = "155.210.150.77";
char ftp_user[]   = "rsense";
char ftp_pass[]   = "rsense";

ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass);

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  while (!Serial) continue;
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  //init
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  //Inicializamos algunos valores (de acuerdo con notacion SENML):
  doc["n"] = "temperatura"; //Nombre  
  //JsonArray& v = doc.createNestedArray("v"); //Vector para añadir valores mediante: v.add(valor); (ver loop)
  doc["u"] = "Cel"; //unidades temperatura
  //doc["t"] = 1; // Valor temporal
}

void loop(){

  if (flag) {
  //Generaremos 10 archivos json:
  for (int j = 0; j <= 9; j++) {
  
      //Generamos 20 valores inventados de temperatura:
      for (int i = 0; i <= 19; i++) {
         valor = 25 + random(-1,1) ;
         doc["v"][i] = valor;
         getLocalTime(&timeinfo);
         strftime(timeHour,3, "%H", &timeinfo);
         strftime(timeMin,3, "%M", &timeinfo);
         strftime(timeSec,3, "%S", &timeinfo);
         //Serial.println(timeHour); Serial.println(timeMin); Serial.println(timeSec);
         sprintf(temp, "%s%s%s", timeHour, timeMin, timeSec);
         tempS = String(temp);
         doc["t"][i] = tempS.toInt();
      }
       //Para debuggear, sacamos el Json por puerto serie:
       serializeJsonPretty(doc, Serial);
       sprintf(jsonName, "grupo1_%s%s%s.json", timeHour, timeMin, timeSec);
       //Serial.println(jsonName);
  
      //FTP
      ftp.OpenConnection();
      ftp.ChangeWorkDir("/rsense/jtirado");
      ftp.InitFile("Type A"); //A = ascii (creo que json usa ascii)
      ftp.NewFile(jsonName);
      serializeJson(doc, docChar);
      delay(100);
      ftp.Write(docChar);
      delay(100);
      ftp.CloseFile();
      ftp.CloseConnection();
      //Esperamos 10 segundos para crear un nuevo archivo:  
      delay(9800);
    }
    flag = false;
  }

}
