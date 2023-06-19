#ifndef MONIBOT_TRANS_CONFIG_H
#define MONIBOT_TRANS_CONFIG_H

#include <Arduino.h>
#include <SPI.h>
#include <ArduinoJson.h>

#include <LoRa.h>
#include <Wire.h>

#include "Adafruit_Sensor.h"
#include "DHT.h"
#include "DHT_U.h"
#include "MQ-Sensor-SOLDERED.h"

#include "WiFi.h"
#include "esp_bt.h"

// LORA
#define SS 5
#define RST 14
#define DIO0 2

// MQ4
#define RatioMQ4CleanAir 9.83 // RS / R0 = 9.83 ppm
#define MQ4_PIN 35
MQ4 mq4(MQ4_PIN);

// MQ7
#define RatioMQ7CleanAir 27.5 // RS / R0 = 27.5 ppm
#define MQ7_PIN 34
MQ7 mq7(MQ7_PIN);

//

// DH21
#define DHT21_PIN 13
#define DHTTYPE DHT21
DHT dht(DHT21_PIN, DHTTYPE);

#endif