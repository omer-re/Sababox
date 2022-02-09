#include "IRremote.h"
#include "TRIGGER_GOOGLESHEETS.h"       /*Library file for Google Sheets, has to be used after Wi-Fi Client Secure declaration, here everything is in Trigger_WIFI.h, so using it after Trigger_WIFI.h*/ 
#include "TRIGGER_WIFI.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include "time.h"

/**********Google Sheets Definations***********/ // source: https://github.com/NishantSahay7/Nodemcu-to-Google-Sheets
char column_name_in_sheets[ ][6] = {"Device_MAC","Event_id","Event_content"};                        /*1. The Total no of column depends on how many value you have created in Script of Sheets;2. It has to be in order as per the rows decided in google sheets*/
String Sheets_GAS_ID = "AKfycbwOt5DYOEOGQfJzc8IpKdLViQMV00Q50-DPsRQbsod9krDqKgeq";                                         /*This is the Sheets GAS ID, you need to look for your sheets id*/
int No_of_Parameters = 3;                                                                /*Here No_of_Parameters decides how many parameters you want to send it to Google Sheets at once, change it according to your needs*/
/*********************************************/


/********** GPIO ***********/
#define IR_SEND_PIN 3
#define SIG_PIN 2
#define LED_PIN 2 //NOTE: this is just a regular indication led- has nothing to the with IR nor with the actual operation

#define TV_ONOFF_PIN 4 //ENABLED
#define FIX_ME_PIN 5 //DISABLED

#define TV_CH_PLUS_PIN 6 //ENABLED
#define TV_CH_MINUS_PIN 7 //ENABLED

#define TV_VOL_PLUS_PIN 8 //ENABLED
#define TV_VOL_MINUS_PIN  9 //ENABLE

#define TV_CH1_PIN 10 //ENABLED
#define TV_CH12_PIN 11 //ENABLED -MOCKUP-ELADS TV CHANEL 2 
#define TV_CH13_PIN 12 //ENABLED -MOCKUP-ELADS TV CHANEL 3 

#define AC_HEAT_ANALOG_PIN 13 //ENABLED
#define AC_COOL_ANALOG_PIN 14 //ENABLED
#define NETFLIX 15 //ENABLED
#define YOUTUBE 16 //ENABLED

#define DELAY_BETWEEN_PRESSES 500
/****************************/

/********** signal raw vectors ***********/
// PARTNER TV
const  uint16_t CH1[67] =    {8930,4470, 580,520, 580,1670, 580,570, 530,1670, 580,1670, 580,570, 530,1670, 580,1670, 580,520, 580,570, 580,570, 530,570, 580,570, 530,570, 580,1670, 580,1670, 530,1670, 580,570, 580,520, 580,570, 580,1670, 530,570, 580,570, 580,520, 580,570, 580,1670, 530,1670, 580,1670, 580,570, 530,1670, 580,1670, 580,1670, 530};  // Protocol=NEC Address=0xC0DA Command=0x11 Raw-Data=0xEE11C0DA 32 bits LSB first 
const  uint16_t CH2[67] =    {8930, 4470, 530, 570, 580, 570, 530, 570, 580, 570, 530, 570, 530, 570, 580, 570, 530, 570, 580, 1670, 580, 1670, 530, 1670, 580, 1720, 530, 1670, 580, 1670, 580, 1670, 580, 520, 580, 570, 530, 1720, 530, 570, 580, 570, 530, 570, 530, 570, 580, 570, 530, 570, 580, 1670, 580, 570, 530, 1670, 580, 1670, 580, 1670, 530, 1720, 530, 1720, 530, 1720, 530}; // Protocol=NEC Address=0x7F00 Command=0x2 Raw-Data=0xFD027F00 32 bits LSB first
const  uint16_t CH3[67] =    {8930,4470, 580,520, 580,1670, 580,570, 530,1670, 580,1670, 580,570, 530,1670, 580,1670, 580,520, 580,570, 580,570, 530,570, 580,570, 530,570, 580,1670, 580,1670, 530,1670, 580,570, 580,520, 580,570, 580,1670, 530,570, 580,570, 580,520, 580,570, 580,1670, 530,1670, 580,1670, 580,570, 530,1670, 580,1670, 580,1670, 530};  // Protocol=NEC Address=0xC0DA Command=0x11 Raw-Data=0xEE11C0DA 32 bits LSB first
const  uint16_t chPlus[67] =   {8980,4370, 630,520, 630,1620, 580,520, 630,1620, 580,1620, 630,520, 630,1620, 580,1670, 580,520, 630,520, 580,570, 580,520, 580,520, 630,520, 630,1620, 580,1670, 580,1620, 580,520, 630,520, 630,520, 580,570, 580,520, 580,570, 580,520, 630,520, 580,1670, 580,1620, 580,1620, 630,1670, 580,1620, 580,1670, 580,1620, 630};  // Protocol=NEC Address=0xC0DA Command=0x1 Raw-Data=0xFE01C0DA 32 bits LSB first
const  uint16_t youTube[67] =   {8980,4370, 630,520, 630,1620, 580,520, 630,1620, 580,1620, 630,520, 630,1620, 580,1670, 580,520, 630,520, 580,570, 580,520, 580,520, 630,520, 630,1620, 580,1670, 580,1620, 580,520, 630,520, 630,520, 580,570, 580,520, 580,570, 580,520, 630,520, 580,1670, 580,1620, 580,1620, 630,1670, 580,1620, 580,1670, 580,1620, 630};  // Protocol=NEC Address=0xC0DA Command=0x1 Raw-Data=0xFE01C0DA 32 bits LSB first
const  uint16_t netFlix[67] =   {8980,4370, 630,520, 630,1620, 580,520, 630,1620, 580,1620, 630,520, 630,1620, 580,1670, 580,520, 630,520, 580,570, 580,520, 580,520, 630,520, 630,1620, 580,1670, 580,1620, 580,520, 630,520, 630,520, 580,570, 580,520, 580,570, 580,520, 630,520, 580,1670, 580,1620, 580,1620, 630,1670, 580,1620, 580,1670, 580,1620, 630};  // Protocol=NEC Address=0xC0DA Command=0x1 Raw-Data=0xFE01C0DA 32 bits LSB first


const  uint16_t chMinus[67] =  {8930,4470, 530,570, 580,570, 580,520, 580,1620, 580,1620, 580,1670, 530,1670, 580,520, 580,1670, 530,1670, 580,1620, 580,570, 530,570, 580,520, 580,570, 530,1670, 580,570, 530,520, 630,1620, 580,1670, 530,570, 530,570, 580,1670, 530,570, 580,1620, 580,1670, 530,570, 530,570, 580,1670, 530,1670, 580,520, 580,1670, 580};  // Protocol=NEC Address=0x78 Command=0x4C Raw-Data=0xB34C8778 32 bits LSB first

// AC
const  uint16_t ACon[99] =     {4380, 4370, 480, 1670, 480, 620, 480, 1670, 480, 1670, 530, 570, 480, 620, 480, 1670, 480, 620, 530, 520, 530, 1670, 480, 620, 480, 620, 430, 1720, 480, 1670, 480, 570, 530, 1670, 530, 570, 480, 620, 480, 1620, 530, 1670, 480, 1720, 430, 1720, 480, 1670, 480, 1670, 530, 1670, 480, 1720, 430, 620, 480, 570, 480, 620, 480, 620, 480, 620, 430, 620, 530, 620, 430, 620, 480, 570, 480, 620, 480, 620, 480, 620, 430, 670, 430, 670, 430, 1670, 480, 1720, 430, 1720, 480, 1720, 430, 1670, 480, 1720, 480, 1670, 480, 1670, 530}; // Protocol=SAMSUNG Address=0xB24D Command=0x3B2 Raw-Data=0x3FCB24D 48 bits LSB first
const  uint16_t ACoff[99] =    {4330, 4420, 480, 1670, 480, 620, 480, 1670, 480, 1720, 480, 570, 480, 620, 480, 1670, 480, 620, 530, 570, 480, 1670, 480, 620, 480, 620, 430, 1720, 480, 1670, 480, 620, 480, 1670, 530, 570, 480, 1670, 480, 1720, 430, 1720, 480, 1670, 480, 620, 480, 1670, 480, 1720, 480, 1670, 480, 620, 430, 620, 480, 620, 480, 620, 480, 1670, 480, 620, 480, 570, 530, 1670, 480, 1670, 480, 1670, 480, 620, 480, 620, 430, 620, 480, 620, 480, 620, 480, 620, 430, 620, 480, 620, 480, 1670, 480, 1720, 430, 1720, 480, 1670, 480, 1670, 530}; // Protocol=SAMSUNG Address=0xB24D Command=0x21B2 Raw-Data=0x21DEB24D 48 bits LSB first
const  uint16_t ACheat[99] =   {4330, 4420, 480, 1670, 480, 620, 480, 1670, 480, 1670, 530, 570, 480, 620, 480, 1670, 480, 620, 530, 520, 530, 1670, 480, 620, 480, 620, 430, 1670, 530, 1670, 480, 620, 480, 1670, 530, 520, 530, 620, 480, 1620, 530, 1670, 480, 1670, 480, 1720, 480, 1670, 480, 1670, 530, 1670, 480, 1670, 480, 620, 480, 570, 480, 620, 480, 570, 530, 570, 480, 620, 530, 1620, 480, 620, 480, 620, 480, 1670, 480, 1670, 480, 1670, 530, 620, 430, 620, 530, 570, 480, 1670, 480, 1720, 480, 570, 480, 620, 480, 570, 530, 1670, 480, 1670, 530}; // Protocol=SAMSUNG Address=0xB24D Command=0x3B2 Raw-Data=0x3FCB24D 48 bits LSB first
const  uint16_t ACcool[99] =   {4330, 4370, 580, 1620, 580, 470, 580, 1620, 580, 1570, 580, 520, 580, 470, 580, 1620, 580, 470, 530, 570, 580, 1570, 580, 520, 580, 520, 580, 1570, 580, 1570, 580, 520, 580, 1620, 480, 570, 580, 520, 580, 1570, 580, 1570, 580, 1620, 580, 1570, 580, 1570, 580, 1620, 530, 1620, 580, 1570, 580, 520, 580, 520, 530, 520, 580, 520, 580, 520, 580, 470, 580, 520, 580, 520, 580, 520, 530, 520, 580, 520, 580, 520, 580, 470, 580, 520, 580, 1570, 580, 1620, 580, 1570, 580, 1620, 530, 1620, 530, 1620, 580, 1570, 580, 1570, 580}; // Protocol=SAMSUNG Address=0xB24D Command=0x3B2 Raw-Data=0x3FCB24D 48 bits LSB first


// TV
const  uint16_t onOff[67] =     {4480,4470, 530,1720, 530,1670, 530,1720, 530,570, 530,570, 530,570, 530,570, 530,570, 530,1720, 530,1670, 580,1670, 530,570, 530,570, 530,570, 530,570, 530,570, 530,570, 580,1670, 530,570, 530,570, 530,570, 530,570, 530,570, 530,570, 580,1670, 530,570, 530,1720, 530,1670, 580,1670, 530,1720, 530,1670, 530,1720, 530};  // Protocol=SAMSUNG Address=0x707 Command=0x2 Raw-Data=0xFD020707 32 bits LSB first
const  uint16_t TV_ch1[67] = {4480,4520, 480,1720, 530,1720, 480,1720, 530,620, 480,620, 480,620, 480,620, 530,620, 480,1720, 530,1720, 480,1720, 530,570, 530,620, 480,620, 480,620, 530,570, 530,620, 480,620, 480,1720, 530,620, 480,620, 480,620, 530,570, 530,620, 480,1720, 530,1720, 480,620, 480,1720, 530,1720, 480,1770, 480,1720, 480,1770, 480};  // Protocol=SAMSUNG Address=0x707 Command=0x4 Raw-Data=0xFB040707 32 bits LSB first
const  uint16_t TV_source[67] = {8930,4520, 480,620, 480,620, 480,620, 480,620, 480,620, 480,620, 480,620, 480,570, 530,1720, 480,1720, 480,1670, 530,620, 480,1720, 530,1670, 530,1720, 480,1720, 480,620, 480,570, 580,520, 530,1720, 480,1720, 480,620, 480,620, 480,620, 480,1720, 480,1720, 480,1720, 530,570, 530,570, 530,1670, 530,1720, 480,1720, 480};  // Protocol=NEC Address=0xF700 Command=0x18 Raw-Data=0xE718F700 32 bits LSB first
const  uint16_t TV_enter[67] = {8930,4520, 480,570, 530,620, 480,570, 530,620, 480,620, 480,620, 480,620, 480,570, 530,1670, 530,1720, 480,1720, 480,620, 480,1720, 530,1670, 530,1670, 530,1720, 480,1670, 530,1720, 530,1670, 530,570, 530,1670, 530,570, 530,570, 530,570, 530,570, 530,570, 480,620, 480,1720, 530,570, 530,1670, 530,1720, 480,1720, 530};  // Protocol=NEC Address=0xF700 Command=0x17 Raw-Data=0xE817F700 32 bits LSB first
const  uint16_t volPlus[67] =  {8930,4470, 530,570, 580,570, 580,520, 580,1620, 580,1620, 580,1670, 530,1670, 580,520, 580,1670, 530,1670, 580,1620, 580,570, 530,570, 580,520, 580,570, 530,1670, 580,570, 530,520, 630,1620, 580,1670, 530,570, 530,570, 580,1670, 530,570, 580,1620, 580,1670, 530,570, 530,570, 580,1670, 530,1670, 580,520, 580,1670, 580};  // Protocol=NEC Address=0x78 Command=0x4C Raw-Data=0xB34C8778 32 bits LSB first
const  uint16_t volMinus[67] =   {8880,4470, 530,620, 530,570, 530,620, 530,1670, 530,1670, 530,1720, 530,1670, 530,570, 530,1720, 530,1670, 530,1670, 530,570, 580,570, 530,570, 530,620, 530,1670, 530,1670, 530,1720, 530,1670, 530,1670, 530,1670, 530,620, 530,570, 530,620, 530,570, 530,620, 530,570, 530,570, 530,620, 530,1670, 530,1670, 530,1720, 530};  // Protocol=NEC Address=0x78 Command=0x1F Raw-Data=0xE01F8778 32 bits LSB first

uint16_t temp[200] = {0};
const uint16_t _CH2[67] =    {8880,4470, 530,620, 530,1670, 530,620, 530,1720, 530,1670, 530,620, 530,1670, 530,1720, 530,620, 530,570, 530,620, 530,570, 530,620, 530,620, 530,1670, 530,1720, 530,620, 530,1670, 530,620, 530,570, 530,1720, 530,620, 530,570, 530,620, 530,1670, 530,620, 530,1720, 530,1670, 530,620, 530,1670, 580,1670, 530,1720, 530};  // Protocol=NEC Address=0xC0DA Command=0x12 Raw-Data=0xED12C0DA 32 bits LSB first
const uint16_t _CH3[67] =    {8930,4470, 530,570, 580,1670, 580,570, 530,1670, 580,1670, 530,570, 580,1670, 580,1670, 530,570, 580,570, 580,570, 530,570, 580,570, 530,570, 580,1670, 530,1720, 530,1670, 580,1670, 530,570, 580,570, 580,1670, 530,570, 580,570, 530,570, 580,570, 580,570, 530,1670, 580,1670, 530,570, 580,1670, 580,1670, 530,1720, 530};  // Protocol=NEC Address=0xC0DA Command=0x13 Raw-Data=0xEC13C0DA 32 bits LSB first
/****************************/


/********** RTC ***********/
// source: https://ziniman.com/2021/07/iot-project-power-detector/
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

/****************************/

int two_digit_channel(const uint16_t *digit1_vector, const uint16_t *digit2_vector, String signal_name) {
    // print to serial monitor as an indication
    Serial.print("Tx: ");
    Serial.print(signal_name);
    Serial.print("\n");

    // transmitting
    Serial.print("Sending");
    IrSender.sendRaw(digit1_vector, sizeof(digit1_vector) / sizeof(digit1_vector[0]), 38);
    delay(600);
    IrSender.sendRaw(digit2_vector, sizeof(digit2_vector) / sizeof(digit2_vector[0]), 38);


}


void flashSig_ready() {
   digitalWrite(SIG_PIN, HIGH);
   delay(2000);
   digitalWrite(SIG_PIN, LOW);
  // Serial.print("flash\n");
  for (size_t i = 0; i < 5; i++) {
    digitalWrite(SIG_PIN, HIGH);
    delay(800);
    digitalWrite(SIG_PIN, LOW);
    delay(200);
  }
  for (size_t i = 0; i < 2; i++) {
    digitalWrite(SIG_PIN, HIGH);
    delay(50);
    digitalWrite(SIG_PIN, LOW);
    delay(50);
  }
  digitalWrite(SIG_PIN, LOW);

}

void flashSig() {
  // Serial.print("flash\n");
  for (size_t i = 0; i < 5; i++) {
    digitalWrite(SIG_PIN, HIGH);
    delay(50);
    digitalWrite(SIG_PIN, LOW);
    delay(50);
  }
  digitalWrite(SIG_PIN, LOW);

}


void setup()
{
    Serial.begin(9600);
    /********** setup pinmode ***********/

    pinMode(TV_ONOFF_PIN, INPUT_PULLUP);
    pinMode(TV_CH1_PIN, INPUT_PULLUP);
    pinMode(TV_CH12_PIN, INPUT_PULLUP);
    pinMode(TV_CH13_PIN, INPUT_PULLUP);
    pinMode(FIX_ME_PIN, INPUT_PULLUP);
    pinMode(TV_CH_PLUS_PIN, INPUT_PULLUP);
    pinMode(TV_VOL_PLUS_PIN, INPUT_PULLUP);

    pinMode(TV_CH_MINUS_PIN, INPUT_PULLUP);
    pinMode(TV_VOL_MINUS_PIN, INPUT_PULLUP);


    pinMode(AC_HEAT_ANALOG_PIN, INPUT_PULLUP);
    pinMode(AC_COOL_ANALOG_PIN, INPUT_PULLUP);

    pinMode(SIG_PIN, OUTPUT);
    digitalWrite(SIG_PIN, LOW);
/****************************/
    // initialize IR Tx object
    IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK);

    // initialize communication with G-sheet
    Google_Sheets_Init(column_name_in_sheets, Sheets_GAS_ID, No_of_Parameters );         /*Sets the column name for Google Sheets, the GAS ID, and the No of Parameter we want to send*/        

  // indications for "READY"
  flashSig_ready();
  Serial.print("SABABOX IS READY\n");
  Data_to_Sheets(No_of_Parameters,  device_MAC, "Connected", ssid);

}


void loop() {
    delay(50);

    if (minutes_of_the_hour()%5 ==0){ 
      Serial.println("alive_ping " +device_MAC+"/t"+" is alive "+localTime) ;
      Data_to_Sheets(2,  device_MAC,  "ALIVE");         /*1. This function accepts multiple float parameter, here No_of_Parameters decides how many parameters you want to send to Google Sheets; 2. The values sent should be in order as per the column in Google Sheets*/
    }  

  if (LOW == digitalRead(TV_ONOFF_PIN)) {
    flashSig();
    memcpy_P(temp, onOff, sizeof(onOff));
    IrSender.sendRaw(temp, sizeof(onOff) / sizeof(onOff[0]), 38);
    Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "TV_ON_OFF");         /*1. This function accepts multiple float parameter, here No_of_Parameters decides how many parameters you want to send to Google Sheets; 2. The values sent should be in order as per the column in Google Sheets*/
    Serial.print("TV_ONOFF_PIN\n");
  }

    else if (LOW == digitalRead(TV_CH1_PIN)) {
        flashSig();
        memcpy_P(temp, CH1, sizeof(CH1));
        //two_digit_channel(temp,temp,"CH11");
        digitalWrite(SIG_PIN, HIGH);
        IrSender.sendRaw(temp, sizeof(CH1) / sizeof(CH1[0]), 38);
        delay(DELAY_BETWEEN_PRESSES);
        IrSender.sendRaw(temp, sizeof(CH1) / sizeof(CH1[0]), 38);
        digitalWrite(SIG_PIN, LOW);

        Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "TV_PRESET1");
        Serial.print("TV_CH1_PIN\n");
    }
    else if (LOW == digitalRead(FIX_ME_PIN)) {
        flashSig();
        digitalWrite(SIG_PIN, HIGH);
        
        // press tv ch1
        memcpy_P(temp, TV_ch1, sizeof(TV_ch1));
        IrSender.sendRaw(temp, sizeof(TV_ch1) / sizeof(TV_ch1[0]), 38);
        delay(DELAY_BETWEEN_PRESSES);
        
        // press tv source X4
        memcpy_P(temp, TV_source, sizeof(TV_source));
        IrSender.sendRaw(temp, sizeof(TV_source) / sizeof(TV_source[0]), 38);
        delay(DELAY_BETWEEN_PRESSES);
        IrSender.sendRaw(temp, sizeof(TV_source) / sizeof(TV_source[0]), 38);
        delay(DELAY_BETWEEN_PRESSES);
        IrSender.sendRaw(temp, sizeof(TV_source) / sizeof(TV_source[0]), 38);
        delay(DELAY_BETWEEN_PRESSES);
        IrSender.sendRaw(temp, sizeof(TV_source) / sizeof(TV_source[0]), 38);
        delay(DELAY_BETWEEN_PRESSES);

        // press tv enter 
        memcpy_P(temp, TV_enter, sizeof(TV_enter));
        IrSender.sendRaw(temp, sizeof(TV_enter) / sizeof(TV_enter[0]), 38);
        delay(DELAY_BETWEEN_PRESSES);

        digitalWrite(SIG_PIN, LOW);
        Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "FIX_ME");
        Serial.print("FIX_ME_PIN\n");
    }

  else if (LOW == digitalRead(TV_CH12_PIN)) {
    flashSig();
    memcpy_P(temp, CH1, sizeof(CH1));
    digitalWrite(SIG_PIN, HIGH);
    IrSender.sendRaw(temp, sizeof(CH1) / sizeof(CH1[0]), 38);
    delay(DELAY_BETWEEN_PRESSES);
    IrSender.sendRaw(_CH2, sizeof(_CH2) / sizeof(_CH2[0]), 38);
    digitalWrite(SIG_PIN, LOW);

    Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "TV_PRESET_2");

    Serial.print("TV_CH12_PIN\n");
  }

  else if (LOW == digitalRead(TV_CH13_PIN)) {
    flashSig();
    memcpy_P(temp, CH1, sizeof(CH1));
    digitalWrite(SIG_PIN, HIGH);
    IrSender.sendRaw(temp, sizeof(CH1) / sizeof(CH1[0]), 38);
    delay(DELAY_BETWEEN_PRESSES);
    IrSender.sendRaw(_CH3, sizeof(_CH3) / sizeof(_CH3[0]), 38);
    digitalWrite(SIG_PIN, LOW);

    Serial.print("TV_CH13_PIN\n");
    Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "TV_PRESET_3");
  }

    else if (LOW == digitalRead(TV_CH_PLUS_PIN)) {
    flashSig();
    memcpy_P(temp, chPlus, sizeof(chPlus));
    IrSender.sendRaw(temp, sizeof(chPlus) / sizeof(chPlus[0]), 38);
    Serial.print("TV_CH_PLUS_PIN\n");
    Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "TV_CH_PLUS");


  }

  else if (LOW == digitalRead(TV_VOL_PLUS_PIN)) {
    flashSig();
    memcpy_P(temp, volPlus, sizeof(volPlus));
    IrSender.sendRaw(temp, sizeof(volPlus) / sizeof(volPlus[0]), 38);
    Serial.print("TV_VOL_PLUS_PIN\n");
    Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "TV_VOL_PLUS");
  }

  else if (LOW == digitalRead(TV_CH_MINUS_PIN)) {
    flashSig();
    memcpy_P(temp, chMinus, sizeof(chMinus));
    IrSender.sendRaw(temp, sizeof(chMinus) / sizeof(chMinus[0]), 38);
    Serial.print("TV_CH_MINUS_PIN\n");
    Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "TV_CH_MINUS");
  }

  else if (LOW == digitalRead(TV_VOL_MINUS_PIN)) {
    flashSig();
    memcpy_P(temp, volMinus, sizeof(volMinus));
    IrSender.sendRaw(temp, sizeof(volMinus) / sizeof(volMinus[0]), 38);
    Serial.print("TV_VOL_MINUS_PIN\n");
    Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "TV_VOL_MINUS");

  }

    else if ((analogRead(AC_HEAT_ANALOG_PIN)<512)) {
      flashSig();
      memcpy_P(temp, ACheat, sizeof(ACheat));
  
      IrSender.sendRaw(temp, sizeof(ACheat) / sizeof(ACheat[0]), 38);
      Serial.print("AC_HEAT_ANALOG_PIN\n");
      Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "AC_HEAT");

  }

    else if ((analogRead(AC_COOL_ANALOG_PIN)<512)) {
      flashSig();
  
      memcpy_P(temp, ACcool, sizeof(ACcool));

      IrSender.sendRaw(temp, sizeof(ACcool) / sizeof(ACcool[0]), 38);
      Serial.print("AC_COOL_ANALOG_PIN\n");
      Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "AC_COOL");


  }

    else if ((analogRead(NETFLIX)<512)) {
        flashSig();

        memcpy_P(temp, netFlix, sizeof(netFlix));

        IrSender.sendRaw(temp, sizeof(netFlix) / sizeof(netFlix[0]), 38);
        Serial.print("netflix_PIN\n");
        Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "NETFLIX");

  }
    
    else if ((analogRead(YOUTUBE)<512)) {
        flashSig();

        memcpy_P(temp, youTube, sizeof(youTube));

        IrSender.sendRaw(temp, sizeof(youTube) / sizeof(youTube[0]), 38);
        Serial.print("youTube_PIN\n");
        Data_to_Sheets(No_of_Parameters,  device_MAC, "Button_pressed", "YOUTUBE");

  }




}