int real_water_level;
int water_level_detect = 13;
int water_NET = 2;
int relay_waterpump = 3;

const byte  trigPinR = 6;
const byte  echoPinR = 7;
unsigned long distance_right_front;

const byte  trigPinL = 4;
const byte  echoPinL = 5;
unsigned long distance_right_back;
int right_LED = 8;

void setup() {
  Serial.begin(9600);
  pinMode(water_level_detect, INPUT);
  pinMode(water_NET, OUTPUT);
  pinMode(relay_waterpump, OUTPUT);

  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR , INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL , INPUT);

  pinMode(right_LED, OUTPUT);
}

void loop() {
  distance_right_front  = pingR();
  distance_right_back  = pingL();

  real_water_level = digitalRead(water_level_detect);
  Serial.println(distance_right_back);

  if (real_water_level == LOW) //觸發水位感測
  {
    digitalWrite(water_NET, HIGH);
    digitalWrite(relay_waterpump, HIGH);
  }
  if (real_water_level == HIGH) //取消觸發水位感測
  {
    digitalWrite(water_NET, LOW);
    digitalWrite(relay_waterpump, LOW);
  }

  if (distance_right_front > 250 && distance_right_front < 440 ) //距離感測，距離偏近(右前)  腳位6、7
  {
    Serial.write('C');
    Serial.write('E');
  }
  if (distance_right_front > 2 && distance_right_front < 250) //距離感測，距離很近(右前)  腳位6、7
  {
    Serial.write('D');
    Serial.write('E');
    digitalWrite(right_LED, HIGH);
  }
  if (distance_right_back > 250 && distance_right_back < 440 ) //距離感測，距離偏近(右後)  腳位3、4
  {
    Serial.write('C');
    Serial.write('E');
  }
  if (distance_right_back > 2 && distance_right_back < 250) //距離感測，距離很近(右後)  腳位3、4
  {
    Serial.write('D');
    Serial.write('E');
    digitalWrite(right_LED, HIGH);
  }
  if (distance_right_front > 250 && distance_right_front < 440 && distance_right_back > 250 && distance_right_back < 440)
  {
    digitalWrite(right_LED, LOW);
  }
  if (distance_right_front > 440 && distance_right_back > 440)
  {
    Serial.write('Y');
    digitalWrite(right_LED, LOW);
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
