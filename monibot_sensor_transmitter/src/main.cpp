#include "config.h"

// lora
const long BAND = 433E6;     // frekuensi operasi LoRa
const int TX_POWER = 17;     // daya output LoRa
const long BAUD_RATE = 9600; // kecepatan data LoRa
const int BW = 125E3;        // bandwidth LoRa

void setup()
{
  // ESP32
  Serial.begin(115200);

  WiFi.disconnect(true);       // Mematikan WiFi
  esp_bt_controller_disable(); // Mematikan Bluetooth

  // LORA
  while (!Serial)
    ;
  Serial.println("LoRa Sender");
  LoRa.setPins(SS, RST, DIO0);
  while (!LoRa.begin(BAND))
  {
    Serial.println(".");
    delay(500);
  }
  LoRa.setTxPower(TX_POWER);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(BW);
  LoRa.setCodingRate4(5);
  LoRa.enableCrc();
  Serial.println("LoRa Transmitter Ready!");

  Serial.println("Memulai pengambilan data sensor...");

  /* // MQ4
  mq4.setRegressionMethod(1); //_PPM =  a*ratio^b
  mq4.setA(574.25);
  mq4.setB(-2.222);
  mq4.begin();
  Serial.print("Calibrating please wait.");
  float calcR04 = 0;
  for (int i = 1; i <= 10; i++)
  {
    mq4.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR04 += mq4.calibrate(RatioMQ4CleanAir);
    Serial.print(".");
  }
  mq4.setR0(calcR04 / 10);
  Serial.println("  done!.");

  if (isinf(calcR04))
  {
    Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your "
                   "wiring and supply");
    while (1)
      ;
  }
  if (calcR04 == 0)
  {
    Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please "
                   "check your wiring and supply");
    while (1)
      ;
  }
  mq4.serialDebug(true); */

  // MQ7
  mq7.setRegressionMethod(1); //_PPM =  a*ratio^b
  mq7.setA(99.042);
  mq7.setB(-1.518);
  mq7.begin();

  Serial.print("Calibrating please wait.");
  float calcR07 = 0;
  for (int i = 1; i <= 10; i++)
  {
    mq7.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR07 += mq7.calibrate(RatioMQ7CleanAir);
    Serial.print(".");
  }
  mq7.setR0(calcR07 / 10);
  Serial.println("  done!.");

  if (isinf(calcR07))
  {
    Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your "
                   "wiring and supply");
    while (1)
      ;
  }
  if (calcR07 == 0)
  {
    Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please "
                   "check your wiring and supply");
    while (1)
      ;
  }
  // mq7.serialDebug(true);

  // DHT
  dht.begin();
}

void loop()
{
  // MILIS
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  const long interval = 30000;

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    // DHT22
    float temp = dht.readTemperature();
    float humd = dht.readHumidity();
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" *C");
    Serial.print("Humidity: ");
    Serial.print(humd);
    Serial.println(" %");

    // MQ4
    float mq4_ppm = random(0, 100);
    /* mq4.update();
    float mq4_ppm = mq4.readSensor();
    Serial.print("MQ4: ");
    Serial.print(mq4_ppm);
    Serial.println(" ppm"); */

    // MQ7
    mq7.update();
    float mq7_ppm = mq7.readSensor();
    Serial.print("MQ7: ");
    Serial.print(mq7_ppm);
    Serial.println(" ppm");

    // LORA DATA
    String LoRaData = String(temp);
    LoRaData += "," + String(humd);
    LoRaData += "," + String(mq4_ppm);
    LoRaData += "," + String(mq7_ppm);

    // SEND DATA
    LoRa.beginPacket();
    LoRa.print(LoRaData);
    LoRa.endPacket();

    Serial.println("====== Paket Send =======");
    Serial.print("Mengirim Data: " + LoRaData);
    Serial.println("\n====== End Paket =======");
  }
}