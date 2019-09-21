#include "Servo.h"                       //include伺服馬達的函式庫
Servo m;                                 //利用Servo類別來建立m這個物件(OOP)
int judge = 0, val = 98;                 //宣告判斷變數judge、伺服馬達轉動角度變數val
int relayPin = 12, right = 4, left = 8;  //宣告繼電器的驅動腳位relayPin、兩個LED燈的腳位right跟left
void setup() {
  Serial.begin(38400);                   //初始化Arduino通信，並設定Baud rate為38400
  pinMode(right, OUTPUT);                //設定LED燈腳位right為輸出腳
  pinMode(left, OUTPUT);                 //設定LED燈腳位left為輸出腳
  pinMode(relayPin, OUTPUT);             //設定繼電器為輸出腳
  m.attach(6);                           //設定伺服馬達的角度控制腳位為6
}
void loop() {
  if (Serial.available())                //如果Arduino有接受到訊息，將接收到的訊息assign給judge
    judge = Serial.read();
  if (judge == 'l')                      //若Arduino收到的訊息為l，則將伺服馬達轉動角度變數val扣2
  {
    val -= 2;
    digitalWrite(4, HIGH);
    digitalWrite(8, LOW);                //扣2的同時，使一LED燈亮、另一LED燈暗以確認val確實有改變
  }
  else if (judge == 'r')                 //若Arduino收到的訊息為r，則將伺服馬達轉動角度變數val加2
  {
    val += 2;
    digitalWrite(8, HIGH);
    digitalWrite(4, LOW);                //加2的同時，使一LED燈亮、另一LED燈暗以確認val確實有改變
  }
  else if (judge == 'u')                 //若Arduino收到的訊息為u，則使伺服馬達轉動角度變數val為98(回正)
  {
    val = 98;
    digitalWrite(4, LOW);
    digitalWrite(8, LOW);                //回正的同時，使兩LED燈暗以確認val為98
  }
  else if (judge == 's')                 //若Arduino收到的訊息為s，則繼電器控制腳位為HIGH
    digitalWrite(relayPin, HIGH);
  else if (judge == 'q')                 //若Arduino收到的訊息為q，則繼電器控制腳位為LOW
    digitalWrite(relayPin, LOW);
  if (val > 180)                         //為防止馬達轉動角度變數val超過伺服馬達最大運轉角，故val大於
    val = 180;                           //180的話，便使val等於180
  else if (val < 0)                      //為防止馬達轉動角度變數val低過伺服馬達最小運轉角，故val小於
    val = 0;                             //0的話，便使val等於0
  m.write(val);                          //使伺服馬達轉動val度
  delay(10);                             //停止個10毫秒使傳送的訊息間有些緩衝
}
