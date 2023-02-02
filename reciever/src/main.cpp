/*
  Adapted from .pio\libdeps\heltec_wifi_lora_32_V2\\examples\LoRa\OLED_LoRa_Reciever.ino
*/

// #include "reciever.h"
#include <Arduino.h>
#include "OLED.h"
#include "LoRa_RX.h"
#define DT 0.02

const unsigned int MAX_MESSAGE_LENGTH = 10;
bool SA_STATUS = false;
bool SA_COMMAND = false;

long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

String rssi = "RSSI --";
String packSize = "--";
String packet;

void setup()
{
  Serial.begin(115200);

  Heltec.begin(
      true /*Display Enabled*/,
      true /*Heltec.Heltec.Heltec.LoRa Disable*/,
      true /*Serial Enable*/,
      true /*PABOOST Enable*/,
      BAND /*long BAND*/);

  OLED_init(); // Initialize the OLED screen

  LoRa.setSignalBandwidth(500E3); // must match transmitter
}

int count = 0;
unsigned long startTime;

void sendPacket(String message)
{
  LoRa.beginPacket();
  LoRa.setTxPower(20,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(message);
  LoRa.endPacket();
}

void loop()
{
  startTime = millis();

  char msg[50];

  int packetSize = LoRa.parsePacket();

  if (packetSize)
  {
    OLED_clear();

    int returned = onReceive(packetSize); // recieve and print recieved packet

    if (returned == 1) {
      SA_STATUS = true;
    }
    count++;
    sprintf(msg, "Packet Received: %d", count);
    OLED_print(0, 26, MAX_SCREEN_WIDTH, msg);
    sprintf(msg, "State Received: %d", returned);
    OLED_print(0, 18, MAX_SCREEN_WIDTH, msg);
  }

  if (SA_COMMAND == true && SA_STATUS == false && millis() - lastSendTime > interval) {
    sendPacket("SA");
    lastSendTime = millis();            // timestamp the message
    interval = random(1000) + 500;    // 0.5-1.5 seconds
    Serial.println("[*] SA - State Armed command sent - trying to arm the core...");
  }

  // Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0)
  {
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    // Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    // Message coming in (check not terminating character) and guard for over message size
    if (inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1))
    {
      // Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    // Full message received...
    else
    {
      // Add null character to string
      message[message_pos] = '\0';

      // Print the message
      Serial.print("Serial Message: ");
      Serial.println(message);
      if (message[0] == 'S' && message[1] == 'A')
      {
        Serial.println("SA_COMMAND is True");
        SA_COMMAND = true;
      }

      // Reset for the next message
      message_pos = 0;
    }
  }

  // Each loop should be at least 20ms.
  while (millis() - startTime < (DT * 1000))
  {
    // wait
  }
}
