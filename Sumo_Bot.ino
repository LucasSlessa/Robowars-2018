#include <DualVNH5019MotorShield.h>

DualVNH5019MotorShield Motors;

//Line Sensor Pins
uint8_t Left_Line_Pin = 0;
uint8_t Right_Line_Pin = 0;
uint8_t Center_Line_Pin = 0;
uint8_t Back_Line_Pin = 0;

//Distance Sensor Pins
uint8_t Front_Center_Pin = A0;
uint8_t Front_Right_Pin = A1;
uint8_t Front_Left_Pin = A2;
uint8_t Back_Pin = A3;
uint8_t Left_Pin = A4;
uint8_t Right_Pin = A5;

//Sensor Threshold
uint32_t Left_Line_Threshold = 0;
uint32_t Right_Line_Threshold = 0;
uint32_t Center_Line_Threshold = 0;
uint32_t Back_Line_Threshold = 0;
uint32_t Front_Center_Threshold = 0;
uint32_t Front_Right_Threshold = 0;
uint32_t Front_Left_Threshold = 0;
uint32_t Back_Threshold = 0;
uint32_t Left_Threshold = 0;
uint32_t Right_Threshold = 0;

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

void setup()
{
  Serial.begin(9600);
  uint32_t initTimer;
  initTimer = millis();
  uint16_t count = 0;
  while (millis() - initTimer < 3000)
  {
    Left_Line_Threshold =  (readLine(Left_Line_Pin) + (count * Left_Line_Threshold)) / (count + 1);
    Right_Line_Threshold =  (readLine(Right_Line_Pin) + (count * Right_Line_Threshold)) / (count + 1);
    Center_Line_Threshold =  (readLine(Center_Line_Pin) + (count * Center_Line_Threshold)) / (count + 1);
    Back_Line_Threshold =  (readLine(Back_Line_Pin) + (count * Back_Line_Threshold)) / (count + 1);
    Front_Center_Threshold = (analogRead(Front_Center_Pin) + (count * Front_Center_Threshold)) / (count + 1);
    Front_Left_Threshold = (analogRead(Front_Left_Pin) + (count * Front_Left_Threshold)) / (count + 1);
    Front_Right_Threshold = (analogRead(Front_Right_Pin) + (count * Front_Right_Threshold)) / (count + 1);
    Back_Threshold = (analogRead(Back_Pin) + (count * Back_Threshold)) / (count + 1);
    Left_Threshold = (analogRead(Left_Pin) + (count * Left_Threshold)) / (count + 1);
    Right_Threshold = (analogRead(Right_Pin) + (count * Right_Threshold)) / (count + 1);
    count++;
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

  sensorProcessed.leftLine = (sensorProcessed.leftLine * 3 + sensorReadings.leftLine) / 4;
  sensorProcessed.rightLine = (sensorProcessed.rightLine * 3 + sensorReadings.rightLine) / 4;
  sensorProcessed.centerLine = (sensorProcessed.centerLine * 3 + sensorReadings.centerLine) / 4;
  sensorProcessed.backLine = (sensorProcessed.backLine * 3 + sensorReadings.backLine) / 4;
  sensorProcessed.left = (sensorProcessed.left * 3 + sensorReadings.left) / 4;
  sensorProcessed.right = (sensorProcessed.right * 3 + sensorReadings.right) / 4;
  sensorProcessed.frontLeft = (sensorProcessed.frontLeft * 3 + sensorReadings.frontLeft) / 4;
  sensorProcessed.frontRight = (sensorProcessed.frontRight * 3 + sensorReadings.frontRight) / 4;
  sensorProcessed.frontCenter = (sensorProcessed.frontCenter * 3 + sensorReadings.frontCenter) / 4;
  sensorProcessed.back = (sensorProcessed.back * 3 + sensorReadings.back) / 4;

  if (sensorProcessed.left > Left_Threshold) {
    sensorFlags.left = 1;
  } else {
    sensorFlags.left = 0;
  }
  if (sensorProcessed.right > Right_Threshold) {
    sensorFlags.right = 1;
  } else {
    sensorFlags.right = 0;
  }
  if (sensorProcessed.back > Back_Threshold) {
    sensorFlags.back = 1;
  } else {
    sensorFlags.back = 0;
  }
  if (sensorProcessed.frontLeft > Front_Left_Threshold) {
    sensorFlags.frontLeft = 1;
  } else {
    sensorFlags.frontLeft = 0;
  }
  if (sensorProcessed.frontRight > Front_Right_Threshold) {
    sensorFlags.frontRight = 1;
  } else {
    sensorFlags.frontRight = 0;
  }
  if (sensorProcessed.frontCenter > Front_Center_Threshold) {
    sensorFlags.frontCenter = 1;
  } else {
    sensorFlags.frontCenter = 0;
  }
  if (sensorProcessed.rightLine > Right_Line_Threshold) {
    sensorFlags.rightLine = 1;
  } else {
    sensorFlags.rightLine = 0;
  }
  if (sensorProcessed.leftLine > Left_Line_Threshold) {
    sensorFlags.leftLine = 1;
  } else {
    sensorFlags.leftLine = 0;
  }
  if (sensorProcessed.backLine > Back_Line_Threshold) {
    sensorFlags.backLine = 1;
  } else {
    sensorFlags.backLine = 0;
  }
  if (sensorProcessed.centerLine > Center_Line_Threshold) {
    sensorFlags.centerLine = 1;
  } else {
    sensorFlags.centerLine = 0;
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
/*
  void processSensors(Sensors readings, Sensors &results)
  {
  static Sensors Count = {
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
  }*/


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

bool checkLine();
