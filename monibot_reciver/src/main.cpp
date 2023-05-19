#include <config.h>

void setup()
{
  // NodeMCU Utility
  Serial.begin(115200);

  // BUZZER
  pinMode(BUZZER, OUTPUT);

  // LORA
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

  // NETWORKING
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

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 alokasi gagal"));
    for (;;)
      ;
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Sensor Data");
  display.display();

  // bunyikan buzzer
  tone(BUZZER, 1000);
  delay(1000);
  noTone(BUZZER);
}

void loop()
{
  // koneksi ke web client
  WiFiClient client;

  if (!client.connect(host, port))
  {
    Serial.println("Connection failed");

    // Tamplikan pesan error ke OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Connection failed");
    display.display();

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
    String data[4];

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

    Serial.print("Received '");
    Serial.print(temp);
    Serial.print(" ");
    Serial.print(humd);
    Serial.print(" ");
    Serial.print(ppmch4);
    Serial.print(" ");
    Serial.print(ppmco);

    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    // BUZZER
    if (ppmco.toInt() > 100)
    {
      tone(BUZZER, 1000);
    }
    else
    {
      noTone(BUZZER);
    }

    // Tampilkan dan data sensor ke OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    String judul = "MONIBOT USR";
    display.setCursor((SCREEN_WIDTH - (judul.length() * 6)) / 2, 0);
    display.println(judul);

    display.setTextSize(1);
    display.println("\nSuhu: " + String(temp) + "C");
    display.println("Kelembaban: " + String(humd) + "%");
    display.println("MQ4: " + String(ppmch4));
    display.println("MQ7: " + String(ppmco));

    display.display();

    //===============================================================

    // pengiriman nilai sensor ke web server
    String apiUrl = "http://monibot.com/crud/kirim_data.php?";
    apiUrl += "mode=save";
    apiUrl += "&temp=" + temp;
    apiUrl += "&humd=" + humd;
    apiUrl += "&ppmch4=" + ppmch4;
    apiUrl += "&ppmco=" + ppmco;

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