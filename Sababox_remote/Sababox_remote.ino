//
// Created by omerr on 08/08/2021.
//

// IRremote.h version 3.1
// a little newer

#include "IRremote.h"
#include <avr/pgmspace.h>
#include <Arduino.h>

#define IR_SEND_PIN 3
#define SIG_PIN 2
#define LED_PIN 2 //NOTE: this is just a regular indication led- has nothing to the with IR nor with the actual operation

#define TV_ONOFF_PIN 12 //ENABLED
#define TV_CH1_PIN 11 //ENABLED
#define TV_CH12_PIN 10 //ENABLED -MOCKUP-ELADS TV CHANEL 2 
#define TV_CH13_PIN 9 //ENABLED -MOCKUP-ELADS TV CHANEL 3 
#define FM88_PIN 8 //DISABLED
#define DAROM_PIN 7 //DISABLED
#define TV_CH_PLUS_PIN 6 //ENABLED
#define TV_VOL_PLUS_PIN 5 //ENABLED

#define TV_CH_MINUS_ANALOG_PIN  A5 //ENABLED
#define TV_VOL_MINUS_ANALOG_PIN  A4 //ENABLED

#define AC_ON_ANALOG_PIN A7 //ENABLED
#define AC_OFF_ANALOG_PIN A6 //ENABLED
#define AC_HEAT_ANALOG_PIN A3 //ENABLED
#define AC_COOL_ANALOG_PIN A2 //ENABLED

#define DELAY_BETWEEN_PRESSES 500

//IRsend irsend;
//decode_results results;

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

void flashSig_ready() {
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
const PROGMEM uint16_t onOff[67] =     {4480,4470, 530,1720, 530,1670, 530,1720, 530,570, 530,570, 530,570, 530,570, 530,570, 530,1720, 530,1670, 580,1670, 530,570, 530,570, 530,570, 530,570, 530,570, 530,570, 580,1670, 530,570, 530,570, 530,570, 530,570, 530,570, 530,570, 580,1670, 530,570, 530,1720, 530,1670, 580,1670, 530,1720, 530,1670, 530,1720, 530};  // Protocol=SAMSUNG Address=0x707 Command=0x2 Raw-Data=0xFD020707 32 bits LSB first
  
const PROGMEM uint16_t CH1[67] =    {8930,4470, 580,520, 580,1670, 580,570, 530,1670, 580,1670, 580,570, 530,1670, 580,1670, 580,520, 580,570, 580,570, 530,570, 580,570, 530,570, 580,1670, 580,1670, 530,1670, 580,570, 580,520, 580,570, 580,1670, 530,570, 580,570, 580,520, 580,570, 580,1670, 530,1670, 580,1670, 580,570, 530,1670, 580,1670, 580,1670, 530};  // Protocol=NEC Address=0xC0DA Command=0x11 Raw-Data=0xEE11C0DA 32 bits LSB first
  
const PROGMEM uint16_t CH2[67] =    {8930, 4470, 530, 570, 580, 570, 530, 570, 580, 570, 530, 570, 530, 570, 580, 570, 530, 570, 580, 1670, 580, 1670, 530, 1670, 580, 1720, 530, 1670, 580, 1670, 580, 1670, 580, 520, 580, 570, 530, 1720, 530, 570, 580, 570, 530, 570, 530, 570, 580, 570, 530, 570, 580, 1670, 580, 570, 530, 1670, 580, 1670, 580, 1670, 530, 1720, 530, 1720, 530, 1720, 530}; // Protocol=NEC Address=0x7F00 Command=0x2 Raw-Data=0xFD027F00 32 bits LSB first
  
const PROGMEM uint16_t CH3[67] =    {8930,4470, 580,520, 580,1670, 580,570, 530,1670, 580,1670, 580,570, 530,1670, 580,1670, 580,520, 580,570, 580,570, 530,570, 580,570, 530,570, 580,1670, 580,1670, 530,1670, 580,570, 580,520, 580,570, 580,1670, 530,570, 580,570, 580,520, 580,570, 580,1670, 530,1670, 580,1670, 580,570, 530,1670, 580,1670, 580,1670, 530};  // Protocol=NEC Address=0xC0DA Command=0x11 Raw-Data=0xEE11C0DA 32 bits LSB first

const PROGMEM uint16_t chPlus[67] =   {8980,4370, 630,520, 630,1620, 580,520, 630,1620, 580,1620, 630,520, 630,1620, 580,1670, 580,520, 630,520, 580,570, 580,520, 580,520, 630,520, 630,1620, 580,1670, 580,1620, 580,520, 630,520, 630,520, 580,570, 580,520, 580,570, 580,520, 630,520, 580,1670, 580,1620, 580,1620, 630,1670, 580,1620, 580,1670, 580,1620, 630};  // Protocol=NEC Address=0xC0DA Command=0x1 Raw-Data=0xFE01C0DA 32 bits LSB first

const PROGMEM uint16_t volPlus[67] =  {8930,4470, 530,570, 580,570, 580,520, 580,1620, 580,1620, 580,1670, 530,1670, 580,520, 580,1670, 530,1670, 580,1620, 580,570, 530,570, 580,520, 580,570, 530,1670, 580,570, 530,520, 630,1620, 580,1670, 530,570, 530,570, 580,1670, 530,570, 580,1620, 580,1670, 530,570, 530,570, 580,1670, 530,1670, 580,520, 580,1670, 580};  // Protocol=NEC Address=0x78 Command=0x4C Raw-Data=0xB34C8778 32 bits LSB first

const PROGMEM uint16_t chMinus[67] =  {8930,4470, 530,570, 580,570, 580,520, 580,1620, 580,1620, 580,1670, 530,1670, 580,520, 580,1670, 530,1670, 580,1620, 580,570, 530,570, 580,520, 580,570, 530,1670, 580,570, 530,520, 630,1620, 580,1670, 530,570, 530,570, 580,1670, 530,570, 580,1620, 580,1670, 530,570, 530,570, 580,1670, 530,1670, 580,520, 580,1670, 580};  // Protocol=NEC Address=0x78 Command=0x4C Raw-Data=0xB34C8778 32 bits LSB first

const PROGMEM uint16_t volMinus[67] =   {8880,4470, 530,620, 530,570, 530,620, 530,1670, 530,1670, 530,1720, 530,1670, 530,570, 530,1720, 530,1670, 530,1670, 530,570, 580,570, 530,570, 530,620, 530,1670, 530,1670, 530,1720, 530,1670, 530,1670, 530,1670, 530,620, 530,570, 530,620, 530,570, 530,620, 530,570, 530,570, 530,620, 530,1670, 530,1670, 530,1720, 530};  // Protocol=NEC Address=0x78 Command=0x1F Raw-Data=0xE01F8778 32 bits LSB first

const PROGMEM uint16_t ACon[99] =     {4380, 4370, 480, 1670, 480, 620, 480, 1670, 480, 1670, 530, 570, 480, 620, 480, 1670, 480, 620, 530, 520, 530, 1670, 480, 620, 480, 620, 430, 1720, 480, 1670, 480, 570, 530, 1670, 530, 570, 480, 620, 480, 1620, 530, 1670, 480, 1720, 430, 1720, 480, 1670, 480, 1670, 530, 1670, 480, 1720, 430, 620, 480, 570, 480, 620, 480, 620, 480, 620, 430, 620, 530, 620, 430, 620, 480, 570, 480, 620, 480, 620, 480, 620, 430, 670, 430, 670, 430, 1670, 480, 1720, 430, 1720, 480, 1720, 430, 1670, 480, 1720, 480, 1670, 480, 1670, 530}; // Protocol=SAMSUNG Address=0xB24D Command=0x3B2 Raw-Data=0x3FCB24D 48 bits LSB first

const PROGMEM uint16_t ACoff[99] =    {4330, 4420, 480, 1670, 480, 620, 480, 1670, 480, 1720, 480, 570, 480, 620, 480, 1670, 480, 620, 530, 570, 480, 1670, 480, 620, 480, 620, 430, 1720, 480, 1670, 480, 620, 480, 1670, 530, 570, 480, 1670, 480, 1720, 430, 1720, 480, 1670, 480, 620, 480, 1670, 480, 1720, 480, 1670, 480, 620, 430, 620, 480, 620, 480, 620, 480, 1670, 480, 620, 480, 570, 530, 1670, 480, 1670, 480, 1670, 480, 620, 480, 620, 430, 620, 480, 620, 480, 620, 480, 620, 430, 620, 480, 620, 480, 1670, 480, 1720, 430, 1720, 480, 1670, 480, 1670, 530}; // Protocol=SAMSUNG Address=0xB24D Command=0x21B2 Raw-Data=0x21DEB24D 48 bits LSB first

const PROGMEM uint16_t ACheat[99] =   {4330, 4420, 480, 1670, 480, 620, 480, 1670, 480, 1670, 530, 570, 480, 620, 480, 1670, 480, 620, 530, 520, 530, 1670, 480, 620, 480, 620, 430, 1670, 530, 1670, 480, 620, 480, 1670, 530, 520, 530, 620, 480, 1620, 530, 1670, 480, 1670, 480, 1720, 480, 1670, 480, 1670, 530, 1670, 480, 1670, 480, 620, 480, 570, 480, 620, 480, 570, 530, 570, 480, 620, 530, 1620, 480, 620, 480, 620, 480, 1670, 480, 1670, 480, 1670, 530, 620, 430, 620, 530, 570, 480, 1670, 480, 1720, 480, 570, 480, 620, 480, 570, 530, 1670, 480, 1670, 530}; // Protocol=SAMSUNG Address=0xB24D Command=0x3B2 Raw-Data=0x3FCB24D 48 bits LSB first

const PROGMEM uint16_t ACcool[99] =   {4330, 4370, 580, 1620, 580, 470, 580, 1620, 580, 1570, 580, 520, 580, 470, 580, 1620, 580, 470, 530, 570, 580, 1570, 580, 520, 580, 520, 580, 1570, 580, 1570, 580, 520, 580, 1620, 480, 570, 580, 520, 580, 1570, 580, 1570, 580, 1620, 580, 1570, 580, 1570, 580, 1620, 530, 1620, 580, 1570, 580, 520, 580, 520, 530, 520, 580, 520, 580, 520, 580, 470, 580, 520, 580, 520, 580, 520, 530, 520, 580, 520, 580, 520, 580, 470, 580, 520, 580, 1570, 580, 1620, 580, 1570, 580, 1620, 530, 1620, 530, 1620, 580, 1570, 580, 1570, 580}; // Protocol=SAMSUNG Address=0xB24D Command=0x3B2 Raw-Data=0x3FCB24D 48 bits LSB first

uint16_t temp[200] = {0};
const uint16_t _CH2[67] =    {8880,4470, 530,620, 530,1670, 530,620, 530,1720, 530,1670, 530,620, 530,1670, 530,1720, 530,620, 530,570, 530,620, 530,570, 530,620, 530,620, 530,1670, 530,1720, 530,620, 530,1670, 530,620, 530,570, 530,1720, 530,620, 530,570, 530,620, 530,1670, 530,620, 530,1720, 530,1670, 530,620, 530,1670, 580,1670, 530,1720, 530};  // Protocol=NEC Address=0xC0DA Command=0x12 Raw-Data=0xED12C0DA 32 bits LSB first
  
const uint16_t _CH3[67] =    {8930,4470, 530,570, 580,1670, 580,570, 530,1670, 580,1670, 530,570, 580,1670, 580,1670, 530,570, 580,570, 580,570, 530,570, 580,570, 530,570, 580,1670, 530,1720, 530,1670, 580,1670, 530,570, 580,570, 580,1670, 530,570, 580,570, 530,570, 580,570, 580,570, 530,1670, 580,1670, 530,570, 580,1670, 580,1670, 530,1720, 530};  // Protocol=NEC Address=0xC0DA Command=0x13 Raw-Data=0xEC13C0DA 32 bits LSB first

void setup()
{
  Serial.begin(9600);
  pinMode(TV_ONOFF_PIN, INPUT_PULLUP);
  pinMode(TV_CH1_PIN, INPUT_PULLUP);
  pinMode(TV_CH12_PIN, INPUT_PULLUP);
  pinMode(TV_CH13_PIN, INPUT_PULLUP);
  pinMode(FM88_PIN, INPUT_PULLUP);
  pinMode(DAROM_PIN, INPUT_PULLUP);
  pinMode(TV_CH_PLUS_PIN, INPUT_PULLUP);
  pinMode(TV_VOL_PLUS_PIN, INPUT_PULLUP);

  pinMode(TV_CH_MINUS_ANALOG_PIN, INPUT_PULLUP);
  pinMode(TV_VOL_MINUS_ANALOG_PIN, INPUT_PULLUP);

  pinMode(AC_ON_ANALOG_PIN, INPUT_PULLUP);
  pinMode(AC_OFF_ANALOG_PIN, INPUT_PULLUP);
  pinMode(AC_HEAT_ANALOG_PIN, INPUT_PULLUP);
  pinMode(AC_COOL_ANALOG_PIN, INPUT_PULLUP);

  pinMode(SIG_PIN, OUTPUT);
  digitalWrite(SIG_PIN, LOW);
  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK);

  // indication for "READY"
  flashSig_ready();
}

void loop() {
  delay(50);
  if (LOW == digitalRead(TV_ONOFF_PIN)) {
    flashSig();
    memcpy_P(temp, onOff, sizeof(onOff));
    IrSender.sendRaw(temp, sizeof(onOff) / sizeof(onOff[0]), 38);
   
    //        Serial.print("d12\n");
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


    //    Serial.print("~d11~\n");
  }

  else if (LOW == digitalRead(TV_CH12_PIN)) {
    flashSig();
    memcpy_P(temp, CH1, sizeof(CH1));
    //memcpy_P(temp2, CH2, sizeof(CH2));
    digitalWrite(SIG_PIN, HIGH);
    IrSender.sendRaw(temp, sizeof(CH1) / sizeof(CH1[0]), 38);
    delay(DELAY_BETWEEN_PRESSES);
    IrSender.sendRaw(_CH2, sizeof(_CH2) / sizeof(_CH2[0]), 38);
    digitalWrite(SIG_PIN, LOW);


//    IrSender.sendRaw(temp, sizeof(ch12) / sizeof(ch12[0]), 38);
    //    Serial.print("~d10~\n");
  }

  else if (LOW == digitalRead(TV_CH13_PIN)) {
    flashSig();
    memcpy_P(temp, CH1, sizeof(CH1));
    //memcpy_P(temp2, CH3, sizeof(CH3));
    digitalWrite(SIG_PIN, HIGH);
    IrSender.sendRaw(temp, sizeof(CH1) / sizeof(CH1[0]), 38);
    delay(DELAY_BETWEEN_PRESSES);
    IrSender.sendRaw(_CH3, sizeof(_CH3) / sizeof(_CH3[0]), 38);
    digitalWrite(SIG_PIN, LOW);

    //IrSender.sendRaw(temp, sizeof(CH13) / sizeof(CH13[0]), 38);
    //    Serial.print("~d9~\n");
  }

  else if (LOW == digitalRead(TV_CH_PLUS_PIN)) {
    flashSig();
    memcpy_P(temp, chPlus, sizeof(chPlus));
    IrSender.sendRaw(temp, sizeof(chPlus) / sizeof(chPlus[0]), 38);
    //    Serial.print("~d6~\n");
  }

  else if (LOW == digitalRead(TV_VOL_PLUS_PIN)) {
    flashSig();
    memcpy_P(temp, volPlus, sizeof(volPlus));
    IrSender.sendRaw(temp, sizeof(volPlus) / sizeof(volPlus[0]), 38);
    //      Serial.print("~d5~\n");
  }

  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~analog pins~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  else if ((analogRead(TV_CH_MINUS_ANALOG_PIN) < 512)) {
    flashSig();
    memcpy_P(temp, chMinus, sizeof(chMinus));
    IrSender.sendRaw(temp, sizeof(chMinus) / sizeof(chMinus[0]), 38);
    //    Serial.print("~a5~\n");
  }

  else if ((analogRead(TV_VOL_MINUS_ANALOG_PIN) < 512)) {
    flashSig();
    memcpy_P(temp, volMinus, sizeof(volMinus));
    IrSender.sendRaw(temp, sizeof(volMinus) / sizeof(volMinus[0]), 38);
    //    Serial.print("~a4~\n");
  }

  else if ((analogRead(AC_OFF_ANALOG_PIN) < 512)) {
    flashSig();
    memcpy_P(temp, ACoff, sizeof(ACoff));

    IrSender.sendRaw(temp, sizeof(ACoff) / sizeof(ACoff[0]), 38);
    //    Serial.print("~a6~\n");
  }

  else if ((analogRead(AC_ON_ANALOG_PIN) < 512)) {
    flashSig();
    memcpy_P(temp, ACon, sizeof(ACon));

    IrSender.sendRaw(temp, sizeof(ACon) / sizeof(ACon[0]), 38);
    //      Serial.print("~a7~\n");
  }
  
    else if ((analogRead(AC_HEAT_ANALOG_PIN)<512)) {
        flashSig();
    memcpy_P(temp, ACheat, sizeof(ACheat));

      IrSender.sendRaw(temp, sizeof(ACheat) / sizeof(ACheat[0]), 38);
//      Serial.print("~a3~\n");
  }
  
    else if ((analogRead(AC_COOL_ANALOG_PIN)<512)) {
        flashSig();
  
    memcpy_P(temp, ACcool, sizeof(ACcool));

      IrSender.sendRaw(temp, sizeof(ACcool) / sizeof(ACcool[0]), 38);
//    Serial.print("~a2~\n");
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

}
