#ifndef MONIBOT_TRANSMITTER_CONFIG_H_
#define MONIBOT_TRANSMITTER_CONFIG_H_

#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// NETWORKING
const char *ssid = "MyASUS";
const char *password = "hy12345678";
const char *host = "192.168.80.240";
const int port = 80;

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

#endif // MONIBOT_TRANSMITTER_CONFIG_H_