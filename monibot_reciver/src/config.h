#ifndef MONIBOT_TRANSMITTER_CONFIG_H_
#define MONIBOT_TRANSMITTER_CONFIG_H_

#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>

#include <HTTPClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Fuzzy.h>

// NETWORKING
const char *ssid = "n19corp";
const char *password = "11111111";
const char *host = "192.168.192.190";
const int port = 80;

//WEBSERVER MONIBOT
String apiEndpoint = "https://ahmadzulfadli.my.id/inputData";

// LORA
#define SS 5
#define RST 14
#define DIO0 2
int counter = 0;

const long BAND = 433E6;     // frekuensi operasi LoRa
const int TX_POWER = 17;     // daya output LoRa
const long BAUD_RATE = 9600; // kecepatan data LoRa
const int BW = 125E3;        // bandwidth LoRa

// OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// BUZZER
#define BUZZER 13

// FUZZY
Fuzzy *fuzzy = new Fuzzy();

float temp_float, humd_float, ppmch4_float, ppmco_float, output1, output2;
int count = 0;
#endif // MONIBOT_TRANSMITTER_CONFIG_H_