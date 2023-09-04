#include <config.h>

// tampilkan data ke serial monitor
void tampilkan_Data(String temp, String humd, String ppmch4, String ppmco)
{
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
}

  // kiriman data ke web server
void kirimDataWebMonibot(String temp, String humd, String ppmch4, String ppmco)
{
  // Prepare the data query parameters
  String queryParams = "mode=save&temp=" + String(temp) + "&humd=" + String(humd) + "&ppmch4=" + String(ppmch4) + "&ppmco=" + String(ppmco);

  // Create the complete URL with query parameters
  String requestUrl = apiEndpoint + "?" + queryParams;
  // Make the HTTP GET request
  HTTPClient http;
  http.begin(requestUrl);

  // Send the GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0)
  {
      Serial.printf("HTTP Response code: %d\n", httpResponseCode);
      String response = http.getString();
      Serial.println(response);
  }
  else
  {
      Serial.printf("HTTP GET request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();
}

// kiriman data ke web server localhost
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
  // Input ppmhch4
  FuzzyInput *ppmhch4 = new FuzzyInput(1);
  FuzzySet *ppmhch4_small = new FuzzySet(0, 10, 20, 30);
  ppmhch4->addFuzzySet(ppmhch4_small);
  FuzzySet *ppmhch4_safe = new FuzzySet(30, 40, 50, 60);
  ppmhch4->addFuzzySet(ppmhch4_safe);
  FuzzySet *ppmhch4_big = new FuzzySet(60, 70, 80, 100);
  ppmhch4->addFuzzySet(ppmhch4_big);
  fuzzy->addFuzzyInput(ppmhch4);

  // Instantiating a FuzzyOutput objects
  FuzzyOutput *tune_buzz1 = new FuzzyOutput(1);
  FuzzySet *slow1 = new FuzzySet(0, 1000, 1000, 2000);
  tune_buzz1->addFuzzySet(slow1);
  FuzzySet *average1 = new FuzzySet(2000, 2000, 3000, 4000);
  tune_buzz1->addFuzzySet(average1);
  FuzzySet *fast1 = new FuzzySet(3000, 4000, 4500, 5000);
  tune_buzz1->addFuzzySet(fast1);
  fuzzy->addFuzzyOutput(tune_buzz1);

  // Building FuzzyRule "IF Input = small THEN TuneBuzz = slow"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputSmall1 = new FuzzyRuleAntecedent();
  ifInputSmall1->joinSingle(ppmhch4_small);
  FuzzyRuleConsequent *thenTuneBuzzSlow1 = new FuzzyRuleConsequent();
  thenTuneBuzzSlow1->addOutput(slow1);
  FuzzyRule *fuzzyRule01 = new FuzzyRule(1, ifInputSmall1, thenTuneBuzzSlow1);
  fuzzy->addFuzzyRule(fuzzyRule01);

  // Building FuzzyRule "IF Input = safe THEN TuneBuzz = average"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputSafe1 = new FuzzyRuleAntecedent();
  ifInputSafe1->joinSingle(ppmhch4_safe);
  FuzzyRuleConsequent *thenTuneBuzzAverage1 = new FuzzyRuleConsequent();
  thenTuneBuzzAverage1->addOutput(average1);
  FuzzyRule *fuzzyRule02 = new FuzzyRule(2, ifInputSafe1, thenTuneBuzzAverage1);
  fuzzy->addFuzzyRule(fuzzyRule02);

  // Building FuzzyRule "IF Input = big THEN TuneBuzz = high"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputBig1 = new FuzzyRuleAntecedent();
  ifInputBig1->joinSingle(ppmhch4_big);
  FuzzyRuleConsequent *thenTuneBuzzFast1 = new FuzzyRuleConsequent();
  thenTuneBuzzFast1->addOutput(fast1);
  FuzzyRule *fuzzyRule03 = new FuzzyRule(3, ifInputBig1, thenTuneBuzzFast1);
  fuzzy->addFuzzyRule(fuzzyRule03);

  //-------------------------------------------------------------------------------------------------
  // Input ppmco
  FuzzyInput *ppmco = new FuzzyInput(2);
  FuzzySet *ppmco_small = new FuzzySet(0, 10, 20, 30);
  ppmco->addFuzzySet(ppmco_small);
  FuzzySet *ppmco_safe = new FuzzySet(30, 40, 50, 60);
  ppmco->addFuzzySet(ppmco_safe);
  FuzzySet *ppmco_big = new FuzzySet(60, 70, 80, 100);
  ppmco->addFuzzySet(ppmco_big);
  fuzzy->addFuzzyInput(ppmco);

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
  ifInputSmall2->joinSingle(ppmco_small);
  FuzzyRuleConsequent *thenTuneBuzzSlow2 = new FuzzyRuleConsequent();
  thenTuneBuzzSlow2->addOutput(slow2);
  FuzzyRule *fuzzyRule11 = new FuzzyRule(1, ifInputSmall2, thenTuneBuzzSlow2);
  fuzzy->addFuzzyRule(fuzzyRule11);

  // Building FuzzyRule "IF Input = safe THEN TuneBuzz = average"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputSafe2 = new FuzzyRuleAntecedent();
  ifInputSafe2->joinSingle(ppmco_safe);
  FuzzyRuleConsequent *thenTuneBuzzAverage2 = new FuzzyRuleConsequent();
  thenTuneBuzzAverage2->addOutput(average2);
  FuzzyRule *fuzzyRule12 = new FuzzyRule(2, ifInputSafe2, thenTuneBuzzAverage2);
  fuzzy->addFuzzyRule(fuzzyRule12);

  // Building FuzzyRule "IF Input = big THEN TuneBuzz = high"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifInputBig2 = new FuzzyRuleAntecedent();
  ifInputBig2->joinSingle(ppmco_big);
  FuzzyRuleConsequent *thenTuneBuzzFast2 = new FuzzyRuleConsequent();
  thenTuneBuzzFast2->addOutput(fast2);
  FuzzyRule *fuzzyRule13 = new FuzzyRule(3, ifInputBig2, thenTuneBuzzFast2);
  fuzzy->addFuzzyRule(fuzzyRule13);

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
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Menguhubungkan LoRa...");
    display.display();
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

    // tampilkan data ke lcd
    tampilkan_Data(temp, humd, ppmch4, ppmco);

    // pengiriman nilai sensor ke web server localhost
    // kirim_Data(temp, humd, ppmch4, ppmco);

    // pengiriman nilai sensor ke web server monibot
    kirimDataWebMonibot(temp, humd, ppmch4, ppmco);

    // rubah tipe variabel string ke float
    temp_float = temp.toFloat();
    humd_float = humd.toFloat();
    ppmch4_float = ppmch4.toFloat();
    ppmco_float = ppmco.toFloat();

    // Set limit co and ch4
    float input_ch4, input_co;
    if (ppmch4_float > 100)
    {
      input_ch4 = 100;
    }
    else
    {
      input_ch4 = ppmch4_float;
    }

    if (ppmco_float > 100)
    {
      input_co = 100;
    }
    else
    {
      input_co = ppmco_float;
    }

    // Set input
    fuzzy->setInput(1, input_ch4);
    fuzzy->setInput(2, input_co);

    // Running the Fuzzification
    fuzzy->fuzzify();

    // Running the Defuzzification
    output1 = fuzzy->defuzzify(1);
    output2 = fuzzy->defuzzify(2);

    count++;
  }

  // memberikan peringatan
  if (ppmch4_float > 30 or ppmco_float > 30)
  {
    if (output1 > output2)
    {
      bunyi_buzzer(output1);

      // tampilkan frekuensi buzzer di oled
      display.print("FrekBuzz: ");
      display.print(output1);
      display.println(" Hz");
    }
    else if (output2 > output1)
    {
      bunyi_buzzer(output2);

      // tampilkan frekuensi buzzer di oled
      display.print("FrekBuzz: ");
      display.print(output2);
      display.println(" Hz");
    }
  }
  else
  {
    display.println("FrekBuzz: 0 Hz");
  }

  display.print("Data ke-");
  display.println(count);
  display.display();
}