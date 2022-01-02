/*
 Project Name   : REMOTE CONTROLLABLE IOT IRRIGATION SYSTEM
 Revision Date  : 01.01.2022 
 Author         : ERSIN MUTLU
 // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 Purpose        : The irrigation process, which is an important part of agriculture, can be completely controlled
                  and monitored remotely at any time and anywhere by reducing the labor.
 // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 System Input   :
                 1) Soil Moisture Sensor         : To measuring soil moisture
                 2) Mini Submersible Water Pump  : To pumping water

 System Output  :
                 1) Virtual Monitor via Wi-Fi    : To monitoring moisture level
                 2) Virtual Button  via Wi-Fi    : To activate the water pump             
                 3) Virtual e-mail               : To inform
 // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  System Work   : Nodemcu ESP8266 was used to provide remote communication with the Blynk application. Thus, control over Wi-Fi is aimed. Thanks to the soil
                  moisture sensor, we can instantly monitor the values taken from the soil. With the mini submersible pump, we can perform the irrigation process
                  when necessary situations. In addition, the latest status can be learnd by e-mail and notification on the application.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

// DEFINITIONS 
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>           // Include enough library for using NodeMCu
#include <BlynkSimpleEsp8266.h>
#include <Blynk.h>
#include <Arduino.h>

BlynkTimer timer;   // Timer for Blynk

char token[] = "UKeqv7WvIb_CMIGSjQjzdLojJhlFlqpS";   // The token given to us to communicate with the Blynk application.
char agAdi[] = "ErsinsiPhone";                         // The name of our Wi-Fi
char agSifre[] = "Ersin12398";                       // The password of our Wi-Fi


#define sensor A0       // Analogue reading pin of soil moisture sensor on the NodeMCU board
int sensor_pin = A0;    // Soil Moisture Pin
int output_value ;      // Value of measured soil moisture.
float Lastmessage = 0;  // Timing for e-mail
float LastEmail = 0;    // Timing for e-mail

const int pump_pin=4;   // The pin on the card to which the water pump is connected.
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// SETUP CODE AREA
void setup() {
  
  Serial.begin(9600);                                  // Start the monitor for communication between Blynk and NodeMCU ESP8266.
  Serial.println("Reading from the sensor ...");       // Transferring the value read from the mouisture sensor to the Serial Monitor ( just for checking ).
  Blynk.begin(token, agAdi, agSifre);                  // In order to connect the Blynk application and the NodeMCU card, write our token, Wi-Fi name and password specified above.
  pinMode(pump_pin, OUTPUT);                           // Pump pin defined as OUTPUT.
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// MAIN CODE AREA
void loop() {
    output_value= analogRead(sensor_pin);              // Read the analog Soil Moisture Sensor value
    output_value = map(output_value,550,0,0,100);      // To change the range of the analog value from 0-550 to 0-100 receive with the map command
    Blynk.virtualWrite(V1,output_value);               // To command Blynk to output
    Serial.print("Moisture : ");                       // To write the phrase "Moisture : " to Serial Monitor (just for checking)
    Serial.print(output_value);                        // To write the output_value to Serial Monitor (just for checking)
    Serial.println("%");                               // indicator

  
    
    if ( output_value <(-50) && (millis() > Lastmessage+6000 )) {                                      // when and what value range the message will be sent
      Lastmessage = millis();                                                                          // delay
      Blynk.email("ersinmutlubg@gmail.com", "Notification", "Moisture Level is low. Need to Water.");  // sent message and adress
      Blynk.notify("Notification Moisture Level is low. Need to Water");                               // sent message and adress
    } 
   if ( output_value >(-40) && (millis() > LastEmail+6000 )) {                                         // when and what value range the message will be sent
      LastEmail = millis();                                                                            // delay
      Blynk.email("ersinmutlubg@gmail.com", "Notification", "Moisture Level is high. Stop motor.");    // sent message and e-mail adress
      Blynk.notify("Notification Moisture Level is high. Stop irrigation");                            // sent  message and e-mail adress
   }
   
  Blynk.run();                                         // to run Blynk application.
  timer.run();                                         // to run setup Blynk application.

}
