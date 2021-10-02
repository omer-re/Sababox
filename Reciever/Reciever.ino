// Reciever_21.8 version

// IRremote.h version 3.1

#include <IRremote.h>
#include <Arduino.h>
#include <FastLED.h>

#define RECV_PIN 5

#define SIG_LED 4 //NOTE: this is just a regular indication led- has nothing to the with IR nor with the actual operation
#define PENDING_LED 6
#define POWER_LED 0 //NOTE: this is just a regular indication led- has nothing to the with IR nor with the actual operation

IRrecv irrecv(RECV_PIN);
decode_results results;

void flash_flash() {
    // led flash as indication
    for (int i = 0; i < 3; i++) {
        digitalWrite(SIG_LED, HIGH);
        delay(90);
        digitalWrite(SIG_LED, LOW);
        delay(90);
    }
}


String sig_name;
void setup() {
    Serial.begin(9600);
    irrecv.enableIRIn(); // Start the receiver

    pinMode(SIG_LED, OUTPUT);
    pinMode(PENDING_LED, OUTPUT);

    Serial.print("RAW_BUFFER_LENGTH\t");Serial.println(RAW_BUFFER_LENGTH);
    flash_flash();
    Serial.println("READY TO GO");
}

void loop() {
    Serial.println("Type signal name: "); 
    while (Serial.available() == 0){
            EVERY_N_MILLISECONDS( 800 ) {digitalWrite(PENDING_LED, !digitalRead(PENDING_LED));}
      }
    sig_name=Serial.readString();
    Serial.println(sig_name);  
    Serial.println("Now record remote operation: "); 
    irrecv.resume(); // clear buffer to avoid miss-ups

    while (irrecv.available() == 0){ }  

    if (irrecv.decode()) {
        Serial.print("Receiving signal:");
        irrecv.compensateAndPrintIRResultAsCArray(&Serial, sig_name);// prints data about the signal
        delay(1000);
        irrecv.resume(); // Receive the next value
        flash_flash();
 
        Serial.print("Done, next\n");
        

    }

}
