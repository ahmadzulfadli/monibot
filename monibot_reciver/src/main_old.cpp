#include <config.h>

// tampilkan data ke serial monitor
void tampilkan_Data(String temp, String humd, String ppmch4, String ppmco)
{
  Serial.print("Suhu: ");
  Serial.print(temp);
  Serial.print("C");
  Serial.print(" | Kelembaban: ");
  Serial.print(humd);
  Serial.print("%");
  Serial.print(" | MQ4: ");
  Serial.print(ppmch4);
  Serial.print(" | MQ7: ");
  Serial.print(ppmco);
  Serial.println();

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
}

// kiriman data ke web server
void kirim_Data(String temp, String humd, String ppmch4, String ppmco)
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

  // pengiriman nilai sensor ke web server
  // windows
  String apiUrl = "http://localhost/web_monibot/crud/kirim_data.php?";
  // linux
  // String apiUrl = "http://monibot.com/crud/kirim_data.php?";
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

// setup fungsi fuzzy
void setup_fuzzy()
{
  //-------------------------------------------------------------------------------------------------
  // Input temp
  FuzzyInput *temp = new FuzzyInput(1);
  FuzzySet *small = new FuzzySet(0, 5, 10, 15);
  temp->addFuzzySet(small);
  FuzzySet *safe = new FuzzySet(15, 20, 25, 30);
  temp->addFuzzySet(safe);
  FuzzySet *big = new FuzzySet(30, 35, 40, 45);
  temp->addFuzzySet(big);
  fuzzy->addFuzzyInput(temp);

  // Instantiating a FuzzyOutput objects
  FuzzyOutput *tune_buzz = new FuzzyOutput(1);
  FuzzySet *slow = new FuzzySet(0, 1000, 1000, 2000);
  tune_buzz->addFuzzySet(slow);
  FuzzySet *average = new FuzzySet(1000, 2000, 3000, 4000);
  tune_buzz->addFuzzySet(average);
  FuzzySet *fast = new FuzzySet(3000, 4000, 4500, 5000);
  tune_buzz->addFuzzySet(fast);
  fuzzy->addFuzzyOutput(tune_buzz);

  // Building FuzzyRule "IF Input = small THEN TuneBuzz = slow"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputSmall = new FuzzyRuleAntecedent();
  ifInputSmall->joinSingle(small);
  FuzzyRuleConsequent *thenTuneBuzzSlow = new FuzzyRuleConsequent();
  thenTuneBuzzSlow->addOutput(slow);
  FuzzyRule *fuzzyRule01 = new FuzzyRule(1, ifInputSmall, thenTuneBuzzSlow);
  fuzzy->addFuzzyRule(fuzzyRule01);

  // Building FuzzyRule "IF Input = safe THEN TuneBuzz = average"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputSafe = new FuzzyRuleAntecedent();
  ifInputSafe->joinSingle(safe);
  FuzzyRuleConsequent *thenTuneBuzzAverage = new FuzzyRuleConsequent();
  thenTuneBuzzAverage->addOutput(average);
  FuzzyRule *fuzzyRule02 = new FuzzyRule(2, ifInputSafe, thenTuneBuzzAverage);
  fuzzy->addFuzzyRule(fuzzyRule02);

  // Building FuzzyRule "IF Input = big THEN TuneBuzz = high"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputBig = new FuzzyRuleAntecedent();
  ifInputBig->joinSingle(big);
  FuzzyRuleConsequent *thenTuneBuzzFast = new FuzzyRuleConsequent();
  thenTuneBuzzFast->addOutput(fast);
  FuzzyRule *fuzzyRule03 = new FuzzyRule(3, ifInputBig, thenTuneBuzzFast);
  fuzzy->addFuzzyRule(fuzzyRule03);

  //-------------------------------------------------------------------------------------------------
  // Input ppmhch4
  FuzzyInput *ppmhch4 = new FuzzyInput(2);
  FuzzySet *ppmhch4_small = new FuzzySet(0, 10, 20, 30);
  ppmhch4->addFuzzySet(ppmhch4_small);
  FuzzySet *ppmhch4_safe = new FuzzySet(30, 40, 50, 60);
  ppmhch4->addFuzzySet(ppmhch4_safe);
  FuzzySet *ppmhch4_big = new FuzzySet(60, 70, 80, 100);
  ppmhch4->addFuzzySet(ppmhch4_big);
  fuzzy->addFuzzyInput(ppmhch4);

  // Instantiating a FuzzyOutput objects
  FuzzyOutput *tune_buzz2 = new FuzzyOutput(2);
  FuzzySet *slow2 = new FuzzySet(0, 1000, 1000, 2000);
  tune_buzz2->addFuzzySet(slow2);
  FuzzySet *average2 = new FuzzySet(1000, 2000, 3000, 4000);
  tune_buzz2->addFuzzySet(average2);
  FuzzySet *fast2 = new FuzzySet(3000, 4000, 4500, 5000);
  tune_buzz2->addFuzzySet(fast2);
  fuzzy->addFuzzyOutput(tune_buzz2);

  // Building FuzzyRule "IF Input = small THEN TuneBuzz = slow"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputSmall2 = new FuzzyRuleAntecedent();
  ifInputSmall2->joinSingle(ppmhch4_small);
  FuzzyRuleConsequent *thenTuneBuzzSlow2 = new FuzzyRuleConsequent();
  thenTuneBuzzSlow2->addOutput(slow2);
  FuzzyRule *fuzzyRule012 = new FuzzyRule(1, ifInputSmall2, thenTuneBuzzSlow2);
  fuzzy->addFuzzyRule(fuzzyRule012);

  // Building FuzzyRule "IF Input = safe THEN TuneBuzz = average"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputSafe2 = new FuzzyRuleAntecedent();
  ifInputSafe2->joinSingle(ppmhch4_safe);
  FuzzyRuleConsequent *thenTuneBuzzAverage2 = new FuzzyRuleConsequent();
  thenTuneBuzzAverage2->addOutput(average2);
  FuzzyRule *fuzzyRule022 = new FuzzyRule(2, ifInputSafe2, thenTuneBuzzAverage2);
  fuzzy->addFuzzyRule(fuzzyRule022);

  // Building FuzzyRule "IF Input = big THEN TuneBuzz = high"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputBig2 = new FuzzyRuleAntecedent();
  ifInputBig2->joinSingle(ppmhch4_big);
  FuzzyRuleConsequent *thenTuneBuzzFast2 = new FuzzyRuleConsequent();
  thenTuneBuzzFast2->addOutput(fast2);
  FuzzyRule *fuzzyRule032 = new FuzzyRule(3, ifInputBig2, thenTuneBuzzFast2);
  fuzzy->addFuzzyRule(fuzzyRule032);

  //-------------------------------------------------------------------------------------------------
  // Input ppmco
  FuzzyInput *ppmco = new FuzzyInput(3);
  FuzzySet *ppmco_small = new FuzzySet(0, 10, 20, 30);
  ppmco->addFuzzySet(ppmco_small);
  FuzzySet *ppmco_safe = new FuzzySet(30, 40, 50, 60);
  ppmco->addFuzzySet(ppmco_safe);
  FuzzySet *ppmco_big = new FuzzySet(60, 70, 80, 100);
  ppmco->addFuzzySet(ppmco_big);
  fuzzy->addFuzzyInput(ppmco);

  // Instantiating a FuzzyOutput objects
  FuzzyOutput *tune_buzz3 = new FuzzyOutput(3);
  FuzzySet *slow3 = new FuzzySet(0, 1000, 1000, 2000);
  tune_buzz3->addFuzzySet(slow3);
  FuzzySet *average3 = new FuzzySet(1000, 2000, 3000, 4000);
  tune_buzz3->addFuzzySet(average3);
  FuzzySet *fast3 = new FuzzySet(3000, 4000, 4500, 5000);
  tune_buzz3->addFuzzySet(fast3);
  fuzzy->addFuzzyOutput(tune_buzz3);

  // Building FuzzyRule "IF Input = small THEN TuneBuzz = slow"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputSmall3 = new FuzzyRuleAntecedent();
  ifInputSmall3->joinSingle(ppmco_small);
  FuzzyRuleConsequent *thenTuneBuzzSlow3 = new FuzzyRuleConsequent();
  thenTuneBuzzSlow3->addOutput(slow3);
  FuzzyRule *fuzzyRule013 = new FuzzyRule(1, ifInputSmall3, thenTuneBuzzSlow3);
  fuzzy->addFuzzyRule(fuzzyRule013);

  // Building FuzzyRule "IF Input = safe THEN TuneBuzz = average"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputSafe3 = new FuzzyRuleAntecedent();
  ifInputSafe3->joinSingle(ppmco_safe);
  FuzzyRuleConsequent *thenTuneBuzzAverage3 = new FuzzyRuleConsequent();
  thenTuneBuzzAverage3->addOutput(average3);
  FuzzyRule *fuzzyRule023 = new FuzzyRule(2, ifInputSafe3, thenTuneBuzzAverage3);
  fuzzy->addFuzzyRule(fuzzyRule023);

  // Building FuzzyRule "IF Input = big THEN TuneBuzz = high"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputBig3 = new FuzzyRuleAntecedent();
  ifInputBig3->joinSingle(ppmco_big);
  FuzzyRuleConsequent *thenTuneBuzzFast3 = new FuzzyRuleConsequent();
  thenTuneBuzzFast3->addOutput(fast3);
  FuzzyRule *fuzzyRule033 = new FuzzyRule(3, ifInputBig3, thenTuneBuzzFast3);
  fuzzy->addFuzzyRule(fuzzyRule033);

  //-------------------------------------------------------------------------------------------------
}

// bunyi buzzer
void bunyi_buzzer(float frekuensi)
{
  tone(BUZZER, frekuensi);
  delay(100);
  noTone(BUZZER);
}

void setup()
{
  // NodeMCU Utility
  Serial.begin(115200);

  // OLED

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 alokasi gagal"));
    for (;;)
      ;
  }
  display.clearDisplay();

  // BUZZER
  pinMode(BUZZER, OUTPUT);

  // SETUP FUZZY
  setup_fuzzy();

  // NETWORKING
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Menguhubungkan ...");
    display.display();
  }
  Serial.println(""); // NETWORKING
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Tampilkan pesan ke OLED
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Sensor Data");
  display.display();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Tampilkan pesan ke OLED
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Sensor Data");
  display.display();

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

    // bunyikan buzzer
  tone(BUZZER, 1000);
  delay(1000);
  noTone(BUZZER);
}

void loop()
{

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

    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    // tampilkan data ke lcd
    tampilkan_Data(temp, humd, ppmch4, ppmco);

    // pengiriman nilai sensor ke web server
    kirim_Data(temp, humd, ppmch4, ppmco);

    // rubah tipe variabel string ke float
    temp_float = temp.toFloat();
    humd_float = humd.toFloat();
    ppmch4_float = ppmch4.toFloat();
    ppmco_float = ppmco.toFloat();

    // Set input
    fuzzy->setInput(1, temp_float);
    fuzzy->setInput(2, ppmch4_float);
    fuzzy->setInput(3, ppmco_float);

    // Running the Fuzzification
    fuzzy->fuzzify();

    // Running the Defuzzification
    output1 = fuzzy->defuzzify(1);
    output2 = fuzzy->defuzzify(2);
    output3 = fuzzy->defuzzify(3);
  }
  // memberikan peringatan

  if (temp_float > 30 or ppmch4_float > 30 or ppmco_float > 30)
  {
    if (output1 > output2 and output1 > output3)
    {
      Serial.print("Frekuensi Buzzer1: ");
      Serial.println(output1);
      bunyi_buzzer(output1);
    }
    else if (output2 > output1 and output2 > output3)
    {
      Serial.print("Frekuensi Buzzer2: ");
      Serial.println(output2);
      bunyi_buzzer(output2);
    }
    else if (output3 > output1 and output3 > output2)
    {
      Serial.print("Frekuensi Buzzer3: ");
      Serial.println(output3);
      bunyi_buzzer(output3);
    }
    else
    {
      Serial.print("Aman");
    }
  }
}