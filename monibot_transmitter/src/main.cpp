#include <config.h>

void setup()
{
  Serial.begin(115200);

  // MQ4
  mq4.setRegressionMethod(1); //_PPM =  a*ratio^b
  mq4.setA(1012.7);
  mq4.setB(-2.786); // Configurate the ecuation values to get CH4 concentration
  mq4.begin();
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++)
  {
    mq4.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += mq4.calibrate(RatioMQ4CleanAir);
    Serial.print(".");
  }
  mq4.setR0(calcR0 / 10);
  Serial.println("  done!.");

  if (isinf(calcR0))
  {
    Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your "
                   "wiring and supply");
    while (1)
      ;
  }
  if (calcR0 == 0)
  {
    Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please "
                   "check your wiring and supply");
    while (1)
      ;
  }
  /*****************************  MQ CAlibration ********************************************/
  mq4.serialDebug(true);

  // MQ7
  mq7.setRegressionMethod(1); //_PPM =  a*ratio^b
  mq7.setA(99.042);
  mq7.setB(-1.518); // Configurate the ecuation values to get CO concentration
  mq7.begin();
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++)
  {
    mq7.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += mq7.calibrate(RatioMQ7CleanAir);
    Serial.print(".");
  }
  mq7.setR0(calcR0 / 10);
  Serial.println("  done!.");

  if (isinf(calcR0))
  {
    Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your "
                   "wiring and supply");
    while (1)
      ;
  }
  if (calcR0 == 0)
  {
    Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please "
                   "check your wiring and supply");
    while (1)
      ;
  }
  /*****************************  MQ CAlibration ********************************************/
  mq7.serialDebug(true);
}

void loop()
{

  // MQ4
  mq4.update();      // Update data, the arduino will be read the voltage on the analog pin
  mq4.readSensor();  // Sensor will read PPM concentration using the model and a and b values setted before or in the
                     // setup
  mq4.serialDebug(); // Will print the table on the serial port
  delay(500);        // Sampling frequency

  // MQ7
  mq7.update();      // Update data, the arduino will be read the voltage on the analog pin
  mq7.readSensor();  // Sensor will read PPM concentration using the model and a and b values setted before or in the
                     // setup
  mq7.serialDebug(); // Will print the table on the serial port
  delay(500);
}
