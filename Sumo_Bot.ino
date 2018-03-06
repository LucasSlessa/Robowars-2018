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

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  static uint16_t leftLine,rightLine,centerLine,backLine,leftSensor,rightSensor,frontLeftSensor,frontRightSensor,frontCenterSensor,backSensor;
  leftLine = readLine(Left_Line_Pin);
  rightLine = readLine(Right_Line_Pin);
  centerLine = readLine(Center_Line_Pin);
  backLine = readLine(Back_Line_Pin);
  frontLeftSensor = analogRead(Front_Left_Pin);
  frontRightSensor = analogRead(Front_Right_Pin);
  frontCenterSensor = analogRead(Front_Center_Pin);
  backSensor = analogRead(Back_Pin);
  leftSensor = analogRead(Left_Pin);
  rightSensor = analogRead(Right_Pin);
}

int readLine(uint8_t pin)
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
