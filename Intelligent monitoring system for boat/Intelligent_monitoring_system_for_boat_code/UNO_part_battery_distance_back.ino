#include <Wire.h>

const byte  trigPinR = 5;
const byte  echoPinR = 6;
unsigned long distance_back_front;
const byte  trigPinL = 3;
const byte  echoPinL = 4;
unsigned long distance_back_back;

float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;

int voltage_detect = A1;
int voltage_NET = 2;
int frontback_LED = 7;
int value;

void setup() {
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR , INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL , INPUT);

  pinMode(voltage_detect, INPUT);
  pinMode(voltage_NET, OUTPUT);
  pinMode(frontback_LED, OUTPUT);

  Serial.begin(9600);
}
void loop() {
  distance_back_front  = pingR();
  distance_back_back = pingL();

  value = analogRead(voltage_detect);
  vout = (value * 5.0) / 1024.0; // see text
  vin = vout / (R2 / (R1 + R2));
  Serial.println(vin);

  if (distance_back_front > 250 && distance_back_front < 440 )    //距離感測，距離偏近(後右)  腳位2、3
  {
    Serial.write('C');
    Serial.write('G');
  }

  if (distance_back_front > 2 && distance_back_front < 250)  //距離感測，距離很近(後右)  腳位2、3
  {
    Serial.write('D');
    Serial.write('G');
    digitalWrite(frontback_LED, HIGH);
  }

  if (distance_back_back > 250 && distance_back_back < 440 ) //距離感測，距離偏近(後左)  腳位4、5
  {
    Serial.write('C');
    Serial.write('G');
  }

  if (distance_back_back > 2 && distance_back_back < 250)    //距離感測，距離很近(後左)  腳位4、5
  {
    Serial.write('D');
    Serial.write('G');
    digitalWrite(frontback_LED, HIGH);
  }
  if (distance_back_front > 250 && distance_back_front < 440 && distance_back_back > 250 && distance_back_back < 440)
  {
    digitalWrite(frontback_LED, LOW);
  }

  if ( vin < 12 && vin > 11.8 )    //電壓感測，電壓偏低
  {
    Serial.write('H');
    digitalWrite(voltage_NET, LOW);
  }

  if (vin < 11.8 && vin > 7)         //電壓感測，電壓很低
  {
    Serial.write('I');
    digitalWrite(voltage_NET, HIGH);
  }

  if (distance_back_back > 440 && distance_back_back > 440) //距離沒問題
  {
    Serial.write('Y');
    digitalWrite(frontback_LED, LOW);
  }
  if (vin > 12 && vin < 14)                              //電壓沒問題
  {
    Serial.write('Z');
    digitalWrite(voltage_NET, LOW);
  }
}
int pingR() {
  digitalWrite(trigPinR, HIGH); //觸發腳位設定為高電位
  digitalWrite(trigPinR, LOW);
  return  ( pulseIn(echoPinR, HIGH) / 58); // 換算成 cm 並傳回
}
int pingL() {
  digitalWrite(trigPinL, HIGH); //觸發腳位設定為高電位
  digitalWrite(trigPinL, LOW);
  return  ( pulseIn(echoPinL, HIGH) / 58); // 換算成 cm 並傳回
}

