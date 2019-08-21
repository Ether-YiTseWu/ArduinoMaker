int temperature_detection;
int real_temperature;
const byte temperature_detect = A0;
int left_LED = 5;

const byte  trigPinR = 6;
const byte  echoPinR = 7;
unsigned long distance_left_front;

const byte  trigPinL = 3;
const byte  echoPinL = 4;
unsigned long distance_left_back;

void setup() {
  Serial.begin(9600);
  pinMode(temperature_detect, INPUT);

  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR , INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL , INPUT);

  pinMode(left_LED, OUTPUT);
}

void loop() {

  temperature_detection = analogRead(temperature_detect);

  real_temperature = (double) temperature_detection * (5 / 10.24);//換算後的溫度

  distance_left_front  = pingR();
  distance_left_back  = pingL();
  Serial.println(real_temperature);

  if (45 < real_temperature && real_temperature < 75 ) //溫度感測，溫度偏高
  {
    Serial.write('A');
  }
  if (75 < real_temperature && real_temperature < 100) //溫度感測，溫度偏高
  {
    Serial.write('B');
  }

  if (distance_left_front > 250 && distance_left_front < 440 ) //距離感測，距離偏近(右前)  腳位6、7
  {
    Serial.write('C');
    Serial.write('F');
  }
  if (distance_left_front > 2 && distance_left_front < 250) //距離感測，距離很近(右前)  腳位6、7
  {
    Serial.write('D');
    Serial.write('F');
    digitalWrite(left_LED, HIGH);
  }
  if (distance_left_back > 250 && distance_left_back < 440 ) //距離感測，距離偏近(右後)  腳位3、4
  {
    Serial.write('C');
    Serial.write('F');
  }
  if (distance_left_back > 2 && distance_left_back < 250) //距離感測，距離很近(右後)  腳位3、4
  {
    Serial.write('D');
    Serial.write('F');
    digitalWrite(left_LED, HIGH);
  }
  if (distance_left_back > 250 && distance_left_back < 440 && distance_left_front > 250 && distance_left_front < 440)
  {
    digitalWrite(left_LED, LOW);
  }
  
  if (45 > real_temperature)
  {
    Serial.write('X');
  }
  if (distance_left_front > 440 && distance_left_back > 440)
  {
    Serial.write('Y');
    digitalWrite(left_LED, LOW);
  }

}

int pingR() {
  digitalWrite(trigPinR, HIGH); //觸發腳位設定為高電位
  delayMicroseconds(10);   //持續5微秒
  digitalWrite(trigPinR, LOW);
  return  ( pulseIn(echoPinR, HIGH) / 58); // 換算成 cm 並傳回
}
int pingL() {
  digitalWrite(trigPinL, HIGH); //觸發腳位設定為高電位
  delayMicroseconds(10);   //持續5微秒
  digitalWrite(trigPinL, LOW);
  return  ( pulseIn(echoPinL, HIGH) / 58); // 換算成 cm 並傳回
}
