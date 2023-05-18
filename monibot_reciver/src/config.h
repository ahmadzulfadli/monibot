#ifndef MONIBOT_TRANSMITTER_CONFIG_H_
#define MONIBOT_TRANSMITTER_CONFIG_H_

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

#include <DHT.h>
#include "MQ-Sensor-SOLDERED.h"

// DHT 22
#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// MQ4
#define SENSOR_MQ4_PIN 45
#define RatioMQ4CleanAir (4.4) // RS / R0 = 60 ppm
MQ2 mq4(SENSOR_MQ4_PIN);

// MQ7
#define SENSOR_MQ7_PIN 34
#define RatioMQ7CleanAir 27.5 // RS / R0 = 27.5 ppm
MQ2 mq7(SENSOR_MQ7_PIN);

#endif // MONIBOT_TRANSMITTER_CONFIG_H_