#include <IRremote.h>
int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);  // 初始化紅外線訊號輸入
decode_results results;  // 儲存訊號的結構

void setup() {
  for (int n = 2; n <= 4; n++)//馬達的VCC
  {
    pinMode(n, OUTPUT);
    digitalWrite(n, HIGH);
  }

  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
int stopping() {
  analogWrite(9, 0);
  analogWrite(6, 0);
  analogWrite(5, 0);
}
void loop() {
  if (irrecv.decode(&results))  // 接收紅外線訊號並解碼
  {
    Serial.println(results.value);
    if (results.value == 16736925) //上-前進
    {
      analogWrite(5, 1000);
    }
    if (results.value == 16720605) //左-左轉
    {
      analogWrite(9, 1000);
    }
    if (results.value == 16761405) //右-右轉
    {
      analogWrite(6, 1000);
    }
    if (results.value == 16712445) //OK-停止
    {
      stopping();
    }
    irrecv.resume();
  }
  delay(100);
}




