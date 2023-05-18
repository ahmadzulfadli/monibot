#ifndef MONIBOT_TRANSMITTER_CONFIG_H_
#define MONIBOT_TRANSMITTER_CONFIG_H_

#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>

// Network ID
const char *ssid = "MyASUS";
const char *password = "hy12345678";
const char *host = "192.168.80.240";
const int port = 80;

// lora
#define SS 5
#define RST 14
#define DIO0 2
int counter = 0;

const long BAND = 433E6;     // frekuensi operasi LoRa
const int TX_POWER = 17;     // daya output LoRa
const long BAUD_RATE = 9600; // kecepatan data LoRa
const int BW = 125E3;        // bandwidth LoRa

#endif // MONIBOT_TRANSMITTER_CONFIG_H_