/*************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  NOTE:
  BlynkTimer provides SimpleTimer functionality:
    http://playground.arduino.cc/Code/SimpleTimer

  App project setup:
    Value Display widget attached to Virtual Pin V5
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLpSuEJLCe"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "RtDp_BWzY5OWt2X1dCPFI4_baXNdlshB"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
//#include <BlynkSimpleEsp32.h>

// get time
// source: https://ziniman.com/2021/07/iot-project-power-detector/
#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  secGMTOffset = 7200; //Set your timezone offset - Israel is GMT+2 or 7200 sec
const int   secDaylightOffset = 3600; //Set your daylight offset
long loopTime, startTime = 0;
char localTime[32] = "";
char* minute_of_hour;

time_t localStamp;

void getLocalTime()
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    strftime(localTime,32,"%d-%m-%Y %H:%M:%S",&timeinfo);
    strftime(minute_of_hour,2,"M",&timeinfo);

    localStamp = mktime(&timeinfo);
    
}

//  END //

String device_MAC = WiFi.macAddress();

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "AAMCAR";
char pass[] = "goodkarma";

//BlynkTimer timer;

void notifyUptime()
{
  long uptime = millis() / 60000L;

  // Actually send the message.
  // Note:
  //   We allow 1 notification per 5 seconds for now.
  //Blynk.notify(String("Running for ") + uptime + " minutes."+"  );
  Blynk.logEvent("alive_ping", (String)("alive ping via notifyUptime interval ",localTime)) ;
  Serial.println("alive_ping " +(String)("alive ping via notifyUptime interval ",localTime)) ;
  // You can also use {DEVICE_NAME} placeholder for device name,
  // that will be replaced by your device name on the server side.
  // Blynk.notify(String("{DEVICE_NAME} running for ") + uptime + " minutes.");
}


// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
//void myTimerEvent()
//{
//  // You can send any value at any time.
//  // Please don't send more that 10 values per second.
////  Blynk.virtualWrite(V5, millis() / 1000);
//}

int minutes_of_the_hour(){
      int minutes=(int)minute_of_hour;
      return minutes;
  }


// void setup()
// {
//   // Debug console
//   Serial.begin(115200);
//   Blynk.begin(auth, ssid, pass);
//   // You can also specify server:
//   //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
//   //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
//   // Notify immediately on startup
//   Blynk.notify("Device started");
//   // Init and get the time
//   Serial.println("Device started");

//   configTime(secGMTOffset, secDaylightOffset, ntpServer);
//   // Setup a function to be called every second
//   // Setup a function to be called every minute
//   //timer.setInterval(10000L, notifyUptime);
//   }

// void loop()
// {
//   Blynk.run();
//   // get local time
//   getLocalTime();
//   // report alive every 5 minutes
//   if (minutes_of_the_hour()%5 ==0){ 
//       Blynk.logEvent("alive_ping", device_MAC+" is alive "+localTime) ;
//       Serial.println("alive_ping " +device_MAC+" is alive "+localTime) ;
//   }
//   if (minutes_of_the_hour()%2 ==0){ 
//       Blynk.logEvent("button_pressed", device_MAC+" button pressed "+localTime) ;
//       Serial.println("alive_ping "+device_MAC+" button pressed "+localTime) ;
//   }

//   //timer.run(); // Initiates BlynkTimer
  
// }
