#ifndef __TRIGGER_WIFI_H__
#define __TRIGGER_WIFI_H__
// #include <ESP8266WiFi.h>
#include <WiFi.h>
//#endif
//
//#ifndef  <WiFiClientSecure.h>
#include <WiFiClientSecure.h>
//#endif


/********** wifi creds ***********/
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "AAMCAR";
char password[] = "goodkarma";
/*********************************************/
WiFiClientSecure client;

#define Debug_Serial_Mon

String device_MAC = WiFi.macAddress();

void WIFI_Connect(const char* wifi_id, const char *wifi_password)
{
  #ifdef Debug_Serial_Mon
	  Serial.print("connecting to ");
	  Serial.println(wifi_id);
  #endif
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_id, wifi_password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  #ifdef Debug_Serial_Mon
	  Serial.println("");
	  Serial.println("WiFi connected");
	  Serial.println("IP address: ");
	  Serial.println(WiFi.localIP());
  #endif
}

#endif
