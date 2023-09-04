#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Fuzzy.h>

Fuzzy *fuzzy = new Fuzzy();

unsigned long previousMillis = 0;
const long interval = 10000;

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
}

// olah data
void olah_data(String ppmch4, String ppmco)
{

    // fuzzy logic
    fuzzy->setInput(0, ppmch4.toFloat());
    fuzzy->setInput(1, ppmco.toFloat());
    fuzzy->fuzzify();

    float fuzzyOutput = fuzzy->defuzzify(0);
    Serial.println("Fuzzy Output: " + String(fuzzyOutput));

    // mengatur frekuensi bunyi buzzer dengan fungsi map
    int buzzerFreq = map(fuzzyOutput, 0, 100, 0, 1000);

    Serial.println("Buzzer Frequency: " + String(buzzerFreq));
}

// fuzzy setup
void fuzzy_setup()
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

void setup()
{
    // Set the Serial output
    Serial.begin(115200);

    // setup fuzzy
    fuzzy_setup();
}

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis; // Perbarui nilai previousMillis

        // Getting a random value
        int temp = random(0, 45);
        int ch4 = random(0, 45);
        int co = random(0, 45);

        // Printing something
        Serial.print("Temp: ");
        Serial.println(temp);
        Serial.print("CH4: ");
        Serial.println(ch4);
        Serial.print("CO: ");
        Serial.println(co);

        // Set the random value as an input
        fuzzy->setInput(1, temp);
        fuzzy->setInput(2, ch4);
        fuzzy->setInput(3, co);

        // Running the Fuzzification
        fuzzy->fuzzify();

        // Running the Defuzzification
        float output1 = fuzzy->defuzzify(1);
        float output2 = fuzzy->defuzzify(2);
        float output3 = fuzzy->defuzzify(3);

        if (temp > 30 or ch4 > 30 or co > 30)
        {
            if (output1 > output2 and output1 > output3)
            {
                Serial.print("Frekuensi Buzzer1: ");
                Serial.println(output1);
            }
            else if (output2 > output1 and output2 > output3)
            {
                Serial.print("Frekuensi Buzzer2: ");
                Serial.println(output2);
            }
            else if (output3 > output1 and output3 > output2)
            {
                Serial.print("Frekuensi Buzzer3: ");
                Serial.println(output3);
            }
            else
            {
                Serial.print("Aman");
            }
        }
    }
}