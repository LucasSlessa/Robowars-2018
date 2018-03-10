#include <DualVNH5019MotorShield.h>

DualVNH5019MotorShield Motors;

//Sensor Pins
uint8_t Left_Line_Pin = 11;
uint8_t Right_Line_Pin = 13;
uint8_t Center_Line_Pin = 5;
uint8_t Back_Line_Pin = 3;

uint8_t Front_Center_Pin = A3;
uint8_t Front_Right_Pin = A2;
uint8_t Front_Left_Pin = A4;
uint8_t Back_Pin = A5;
uint8_t Left_Pin = A0;
uint8_t Right_Pin = A1;

struct Sensors
{
  uint16_t leftLine;
  uint16_t rightLine;
  uint16_t centerLine;
  uint16_t backLine;
  uint16_t left;
  uint16_t right;
  uint16_t frontLeft;
  uint16_t frontRight;
  uint16_t frontCenter;
  uint16_t back;
};

static Sensors sensorThresholds = {
  .leftLine = 0,
  .rightLine = 0,
  .centerLine = 0,
  .backLine = 0,
  .left = 0,
  .right = 0,
  .frontLeft = 0,
  .frontRight = 0,
  .frontCenter = 0,
  .back = 0
};

void setup()
{
  Motors.init();

  Serial.begin(230400);
  uint32_t initTimer;
  initTimer = millis();
  uint16_t count = 0;
  while (millis() - initTimer < 2400)
  {
    if (count < 50 && millis() > 1000)
    {
      if (count < 15)
      {
        sensorThresholds.leftLine =  (readLine(Left_Line_Pin) + sensorThresholds.leftLine);
        sensorThresholds.rightLine =  (readLine(Right_Line_Pin) + sensorThresholds.rightLine);
        sensorThresholds.centerLine =  (readLine(Center_Line_Pin) + sensorThresholds.centerLine);
        sensorThresholds.backLine =  (readLine(Back_Line_Pin) + sensorThresholds.backLine);
      }
      sensorThresholds.frontCenter = (analogRead(Front_Center_Pin) + sensorThresholds.frontCenter);
      sensorThresholds.frontLeft = (analogRead(Front_Left_Pin) + sensorThresholds.frontLeft);
      sensorThresholds.frontRight = (analogRead(Front_Right_Pin) + sensorThresholds.frontRight);
      sensorThresholds.back = (analogRead(Back_Pin) + sensorThresholds.back);
      sensorThresholds.left = (analogRead(Left_Pin) + sensorThresholds.left);
      sensorThresholds.right = (analogRead(Right_Pin) +  sensorThresholds.right);
      count++;
    }
  }
  sensorThresholds.leftLine /= 30; if (sensorThresholds.leftLine < 100) {
    sensorThresholds.leftLine = 100;
  }
  sensorThresholds.rightLine /= 30; if (sensorThresholds.rightLine < 100) {
    sensorThresholds.rightLine = 100;
  }
  sensorThresholds.centerLine /= 30; if (sensorThresholds.centerLine < 100) {
    sensorThresholds.centerLine = 100;
  }
  sensorThresholds.backLine /= 30; if (sensorThresholds.backLine < 100) {
    sensorThresholds.backLine = 100;
  }
  count = count *2 / 3;
  sensorThresholds.left /= count; if (sensorThresholds.left < 100) {
    sensorThresholds.left = 100;
  }
  sensorThresholds.right /= count; if (sensorThresholds.right < 100) {
    sensorThresholds.right = 100;
  }
  sensorThresholds.frontLeft /= count; if (sensorThresholds.frontLeft < 100) {
    sensorThresholds.frontLeft = 100;
  }
  sensorThresholds.frontRight /= count; if (sensorThresholds.frontRight < 100) {
    sensorThresholds.frontRight = 100;
  }
  sensorThresholds.frontCenter /= count; if (sensorThresholds.frontCenter < 100) {
    sensorThresholds.frontCenter = 100;
  }
  sensorThresholds.back /= count; if (sensorThresholds.back < 100) {
    sensorThresholds.back = 100;
  }
}

void loop()
{
  static Sensors sensorReadings;
  checkSensors(sensorReadings);
  static Sensors sensorFlags;
  static Sensors sensorProcessed = {
    .leftLine = sensorReadings.leftLine,
    .rightLine = sensorReadings.rightLine,
    .centerLine = sensorReadings.centerLine,
    .backLine = sensorReadings.backLine,
    .left = sensorReadings.left,
    .right = sensorReadings.right,
    .frontLeft = sensorReadings.frontLeft,
    .frontRight = sensorReadings.frontRight,
    .frontCenter = sensorReadings.frontCenter,
    .back = sensorReadings.back
  };
  sensorFilter(sensorProcessed, sensorReadings);
  sensorCompare(sensorThresholds, sensorProcessed, sensorFlags);


  //    setLeft(200);
  //   setRight(200);
  // Serial.println(Motors.getM1Fault());
  // Serial.println(Motors.getM2Fault());

  if ((sensorFlags.centerLine == 1) || (sensorThresholds.leftLine == 1) || (sensorThresholds.rightLine == 1 ))
  {
    setLeft(-400);
    setRight(-400);
    delay(700);
    setLeft(400);
    setRight(-400);
    delay(300);
    setLeft(400);
    setRight(400);
    Serial.print("1\n");
  }
  else if (sensorFlags.backLine == 1 )
  {
    setLeft(400);
    setRight(400);
    Serial.print("2\n");
  }
  else if (sensorFlags.frontCenter == 1 )
  {
    setLeft(400);
    setRight(400);
    Serial.print("3\n");
  }
    else if (sensorFlags.frontLeft == 1 )
    {
    setLeft(250);
    setRight(300);
    Serial.print("4\n");
    }
    else if (sensorFlags.frontRight == 1 )
    {
    setLeft(300);
    setRight(250);
    Serial.print("5\n");
    }
    else if (sensorFlags.left == 1 )
    {
    setLeft(20);
    setRight(300);
    delay(100);
    Serial.print("6\n");
    }
    else if (sensorFlags.right == 1 )
    {
    setLeft(300);
    setRight(20);
    delay(100);
    Serial.print("7\n");
    }/*
    else if (sensorFlags.back == 1 )
    {
    setLeft(400);
    setRight(-400);
    delay(300);
    Serial.print("8\n");
    }*/
  else
  {
    Serial.print("9\n");
    static uint32_t timestamp = 0;
    if (millis() - timestamp > 700)
    {
      Serial.print("10\n");
      timestamp = millis();
      uint16_t randNumber1 = random(100) + 200;
      setLeft(randNumber1);
      uint16_t randNumber2 = random(100) + 200;
      setRight(randNumber2);
      // Serial.print("\n");
      // Serial.println(randNumber1);
      // delay(1000);
    }
    //if (randNumber1 )
  }


  /*
    Serial.print("\nThreshold back: ");
    Serial.print(sensorThresholds.back);
    Serial.print("    Reading back: ");
    Serial.print(sensorReadings.back);
    Serial.print("    Processed back: ");
    Serial.print(sensorProcessed.back);
    Serial.print("   Flag back: ");
    Serial.print(sensorFlags.back);


    Serial.print("\nThreshold left: ");
    Serial.print(sensorThresholds.left);
    Serial.print("   Reading left: ");
    Serial.print(sensorReadings.left);
    Serial.print("   Processed left: ");
    Serial.print(sensorProcessed.left);
    Serial.print("   Flag left: ");
    Serial.print(sensorFlags.left);

    Serial.print("\nThreshold right: ");
    Serial.print(sensorThresholds.right);
    Serial.print("   Reading right: ");
    Serial.print(sensorReadings.right);
    Serial.print("   Processed right: ");
    Serial.print(sensorProcessed.right);
    Serial.print("   Flag right: ");
    Serial.print(sensorFlags.right);

    Serial.print("\nThreshold frontCenter: ");
    Serial.print(sensorThresholds.frontCenter);
    Serial.print("   Reading frontCenter: ");
    Serial.print(sensorReadings.frontCenter);
    Serial.print("   Processed frontCenter: ");
    Serial.print(sensorProcessed.frontCenter);
    Serial.print("   Flag frontCenter: ");
    Serial.print(sensorFlags.frontCenter);

    Serial.print("\nThreshold frontLeft: ");
    Serial.print(sensorThresholds.frontLeft);
    Serial.print("    Reading frontLeft: ");
    Serial.print(sensorReadings.frontLeft);
    Serial.print("   Processed frontLeft: ");
    Serial.print(sensorProcessed.frontLeft);
    Serial.print("   Flag frontLeft: ");
    Serial.print(sensorFlags.frontLeft);

    Serial.print("\nThreshold frontRight: ");
    Serial.print(sensorThresholds.frontRight);
    Serial.print("   Reading frontRight: ");
    Serial.print(sensorReadings.frontRight);
    Serial.print("   Processed frontRight: ");
    Serial.print(sensorProcessed.frontRight);
    Serial.print("   Flag frontRight: ");
    Serial.print(sensorFlags.frontRight);
  */
  /*
      Serial.print("\nThreshold rightLine: ");
      Serial.print(sensorThresholds.rightLine);
      Serial.print("   Reading rightLine: ");
      Serial.print(sensorReadings.rightLine);
      Serial.print("   Processed rightLine: ");
      Serial.print(sensorProcessed.rightLine);
      Serial.print("   Flag rightLine: ");
      Serial.print(sensorFlags.rightLine);

    Serial.print("\nThreshold leftLine: ");
    Serial.print(sensorThresholds.leftLine);
    Serial.print("   Reading leftLine: ");
    Serial.print(sensorReadings.leftLine);
    Serial.print("   Processed leftLine: ");
    Serial.print(sensorProcessed.leftLine);
    Serial.print("   Flag leftLine: ");
    Serial.print(sensorFlags.leftLine);

    Serial.print("\nThreshold centerLine: ");
    Serial.print(sensorThresholds.centerLine);
    Serial.print("   Reading centerLine: ");
    Serial.print(sensorReadings.centerLine);
    Serial.print("   Processed centerLine: ");
    Serial.print(sensorProcessed.centerLine);
    Serial.print("   Flag centerLine: ");
    Serial.print(sensorFlags.centerLine);

    Serial.print("\nThreshold backLine: ");
    Serial.print(sensorThresholds.backLine);
    Serial.print("   Reading backLine: ");
    Serial.print(sensorReadings.backLine);
    Serial.print("   Processed backLine: ");
    Serial.print(sensorProcessed.backLine);
    Serial.print("   Flag backLine: ");
    Serial.print(sensorFlags.backLine);
  */
  /*
            Serial.print("\n\n\n\n");
            delay(700);*/
}





void setLeft(uint16_t speed)
{
  Motors.setM1Speed(-speed);
}
void setRight(uint16_t speed)
{
  Motors.setM2Speed(speed);
}


void sensorFilter(Sensors &Processed, Sensors &Readings)
{
  static const uint8_t numerator = 7;
  static const uint8_t denominator = 8;
  Processed.leftLine = Readings.leftLine;
  Processed.rightLine = Readings.rightLine;
  Processed.centerLine = Readings.centerLine;
  Processed.backLine =  Readings.backLine;
  Processed.left = (Processed.left * numerator + Readings.left) / denominator;
  Processed.right = (Processed.right * numerator + Readings.right) / denominator;
  Processed.frontLeft = (Processed.frontLeft * numerator + Readings.frontLeft) / denominator;
  Processed.frontRight = (Processed.frontRight * numerator + Readings.frontRight) / denominator;
  Processed.frontCenter = (Processed.frontCenter * numerator + Readings.frontCenter) / denominator;
  Processed.back = (Processed.back * numerator + Readings.back) / denominator;
}

void sensorCompare(Sensors Thresholds, Sensors Processed, Sensors &Flags)
{
  if (Processed.left > Thresholds.left) {
    Flags.left = 1;
  } else {
    Flags.left = 0;
  }
  if (Processed.right > Thresholds.right) {
    Flags.right = 1;
  } else {
    Flags.right = 0;
  }
  if (Processed.back > Thresholds.back) {
    Flags.back = 1;
  } else {
    Flags.back = 0;
  }
  if (Processed.frontLeft > Thresholds.frontLeft) {
    Flags.frontLeft = 1;
  } else {
    Flags.frontLeft = 0;
  }
  if (Processed.frontRight > Thresholds.frontRight) {
    Flags.frontRight = 1;
  } else {
    Flags.frontRight = 0;
  }
  if (Processed.frontCenter > Thresholds.frontCenter) {
    Flags.frontCenter = 1;
  } else {
    Flags.frontCenter = 0;
  }
  if (Processed.rightLine < Thresholds.rightLine) {
    Flags.rightLine = 1;
  } else {
    Flags.rightLine = 0;
  }
  if (Processed.leftLine < Thresholds.leftLine) {
    Flags.leftLine = 1;
  } else {
    Flags.leftLine = 0;
  }
  if (Processed.backLine < Thresholds.backLine) {
    Flags.backLine = 1;
  } else {
    Flags.backLine = 0;
  }
  if (Processed.centerLine < Thresholds.centerLine) {
    Flags.centerLine = 1;
  } else {
    Flags.centerLine = 0;
  }
}
void checkSensors(Sensors &sensors)
{
  sensors.leftLine = readLine(Left_Line_Pin);
  sensors.rightLine = readLine(Right_Line_Pin);
  sensors.centerLine = readLine(Center_Line_Pin);
  sensors.backLine = readLine(Back_Line_Pin);
  sensors.frontLeft = analogRead(Front_Left_Pin);
  sensors.frontRight = analogRead(Front_Right_Pin);
  sensors.frontCenter = analogRead(Front_Center_Pin);
  sensors.back = analogRead(Back_Pin);
  sensors.left = analogRead(Left_Pin);
  sensors.right = analogRead(Right_Pin);
}

int16_t readLine(uint8_t pin)
{
  pinMode(pin, OUTPUT );
  digitalWrite(pin, HIGH );
  delayMicroseconds(10);
  pinMode(pin, INPUT);
  int32_t timestamp = micros();
  while ((digitalRead(pin) == HIGH) && (micros() - timestamp < 3000))
  {
    delay(0);
  }
  uint16_t diff = micros() - timestamp;
  return diff;
}

int16_t readDistance (uint8_t pin)
{
  uint16_t temp;
  temp = analogRead(pin);
  return temp;
}
