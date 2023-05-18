#include <config.h>

void setup()
{
  // NodeMCU Utility
  Serial.begin(115200);

  // Lora
  while (!Serial)
    ;
  Serial.println("LoRa Receiver");
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
  Serial.println("LoRa Receiver Ready!");

  // Networking
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // koneksi ke web client
  WiFiClient client;

  if (!client.connect(host, port))
  {
    Serial.println("Connection failed");
    return;
  }

  // penerimaan lora
  String receivedData = "";

  int packetSize = LoRa.parsePacket();
  if (packetSize != 0)
  {
    Serial.print("Received packet '");

    while (LoRa.available())
    {
      receivedData += (char)LoRa.read();
    }

    Serial.println(receivedData);

    char stringData[receivedData.length() + 1];
    strcpy(stringData, receivedData.c_str());

    char *ptr = strtok(stringData, ",");
    int i = 0;
    String data[6];

    while (ptr != NULL)
    {
      data[i] = String(ptr);
      i++;
      ptr = strtok(NULL, ",");
    }

    String temp = data[0];
    String humd = data[1];
    String ppmch4 = data[2];
    String ppmco = data[3];

    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    //===============================================================

    // pengiriman nilai sensor ke web server
    String apiUrl = "http://intai.com/crud/kirim_data.php?";
    apiUrl += "mode=save";
    apiUrl += "&temp=" + temp;
    apiUrl += "&humd=" + humd;
    apiUrl += "&ppmch2=" + ppmch4;
    apiUrl += "&ppmco2=" + ppmco;

    // Set header Request
    client.print(String("GET ") + apiUrl + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    // Pastikan tidak berlarut-larut
    unsigned long timeout = millis();
    while (client.available() == 0)
    {
      if (millis() - timeout > 3000)
      {
        Serial.println(">>> Client Timeout !");
        Serial.println(">>> Operation failed !");
        client.stop();
        return;
      }
    }

    // Baca hasil balasan dari PHP
    while (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.println(line);
    }
  }
}