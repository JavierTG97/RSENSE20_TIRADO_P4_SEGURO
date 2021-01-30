#include <WiFi.h>
#include "time.h"
//#include <HTTPClient.h>

// ---------- Datos RED Wi-Fi  -----------------------
//Red WiFi a la que nos conectamos:
const char* ssid       = "OrdenadorJavier";
const char* password   = "holaquetal";
//const char* ssid       = "RCasas";
//const char* password   = NULL;

// ---------- Datos del HOST (Servidor) --------------
// ------ y conexion del ESP32 (cliente) con él ------
// Puerto y dirección IP
const uint16_t port = 21;
//const char * host = "192.168.1.39";
const char * host = "192.168.137.1";
//Generamos el cliente que se conecta al la IP del host:
WiFiClient client;

// ------------------- NTP ---------------------------
const char* ntpServer = "pool.ntp.org";
//Madrid: UTC + 1h -> offset = 3600 segundos.
const long  gmtOffset_sec = 3600;
//Como España tiene periodos en los que se adelanta/retrasa la hora en 1,
//Esto se indica a continuacion:
const int   daylightOffset_sec = 3600;
//Struct para captar el tiempo:
struct tm timeinfo;

// ------------ Otras variables utiles -------------
String socket;     // String que contiene el socket
bool flag = false; // Para decidir si mandar o no el socket

// ----- Funcion para leer los SOCKETs -------------
void leerSocket(){
  char orden[5]; //5 caracteres
  int i=0;
  
  while (client.available()>0){
      orden[i] = client.read();
      i=i+1;
  }
  socket = String(orden);
}


void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password); // Cuando nos conectamos con contraseña
  //WiFi.begin(ssid); // Cuando nos conectamos sin contraseña
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());

  if (!client.connect(host, port)) {
    Serial.println("Connection to host failed");
    delay(1000);
    return;
  }

  //disconnect WiFi as it's no longer needed
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
}

void loop()
{
  // Esperamos a que se reciba alguna orden del host:
  if (client.available()>0){   
    Serial.println("entramos\n");
  
    //Recogemos todos los caracteres de la orden:
    leerSocket();
    
    //Comprobamos que socket es el que se ha enviado:
    if (socket.substring(0,5) == "start"){
        flag = true;
    } else if (socket.substring(0,4) == "stop"){
        flag = false;
    }
  }
  
  if (flag) {  
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      return;
    }    
    client.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  }   

  delay(1000);
  //Serial.println(WiFi.localIP());   
}
