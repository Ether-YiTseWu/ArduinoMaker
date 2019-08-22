#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define LCD_ROWS 2  // LCD顯示器的列數
#define LCD_COLS 16  // LCD顯示器的行數
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int i;
int buttonState;
int beforeState;
int pressTime;
char val_0;
char val_1;
char val_2;

int red_temp = 23;         //溫度RGB腳位設定
int green_temp = 25;
int blue_temp = 27;

int red_distance = 29;     //測距RGB腳位設定
int green_distance = 31;
int blue_distance = 33;

int red_battery = 35;     //電池RGB腳位設定
int green_battery = 37;
int blue_battery = 39;

int right_distance_LED = 28;     //測距燈號設定
int left_distance_LED = 30;
int back_distance_LED = 32;

int buzzer_small = 52;    //蜂鳴器_小聲
int buzzer_loud = 53;     //蜂鳴器_大聲
int button_Status = 8;    //段位模式的按鈕感測

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // RX1/TX1
  Serial2.begin(9600); // RX2/TX2
  Serial3.begin(9600); // RX3/TX3

  lcd.begin(16, 2); // 初始化lcd物件
  lcd.backlight();

  for (i = 22; i <= 53; i++ )
  {
    pinMode(i, OUTPUT);
  }
  pinMode(buzzer_loud, OUTPUT);
  pinMode(button_Status, INPUT);
}

void loop() {
  onoff();
  Serial.println(pressTime);

  if (Serial.available() || Serial1.available() || Serial2.available() ) {
    //代表MEGA上的每個TX RX串口，連接不同的ARDUINO UON版
    val_0 = Serial.read();                //左測距的UNO，連接MEGA的
    val_1 = Serial1.read();               //右測距的UNO
    val_2 = Serial2.read();               //後測距的UNO

    Serial.println(val_0);
    if (val_1 == 'A') //觸發溫度感測，溫度較高，亮綠燈，螢幕輸出"temperature!"
    {
      RGBoff_temp();
      analogWrite(red_temp, 0);
      analogWrite(green_temp, 1000);
      analogWrite(blue_temp, 0);

      lcd.setCursor(0, 0);
      lcd.print("temperature!");
    }

    else if (val_1 == 'B') //觸發溫度感測，溫度很高，亮紅燈，蜂鳴器響，螢幕輸出"temperature!"
    {
      RGBoff_temp();
      analogWrite(red_temp, 1000);
      analogWrite(green_temp, 0);
      analogWrite(blue_temp, 0);

      lcd.setCursor(0, 0);
      lcd.print("temperature!");

      tone(buzzer_small, 1000, 50);
      delay(250);
    }

    else if (val_0 == 'C' || val_1 == 'C' || val_2 == 'C' ) //觸發測距，距離較近，亮綠燈，螢幕輸出"distance!"
    {
      RGBoff_distance();
      analogWrite(red_distance, 0);
      analogWrite(green_distance, 1000);
      analogWrite(blue_distance, 0);

      lcd.setCursor(0, 1);
      lcd.print("distance!");
    }

    else if (val_0 == 'D' || val_1 == 'D' || val_2 == 'D') //觸發測距，距離很近，亮紅燈，蜂鳴器響，螢幕輸出"distance!"
    {
      RGBoff_distance();
      analogWrite(red_distance, 1000);
      analogWrite(green_distance, 0);
      analogWrite(blue_distance, 0);

      lcd.setCursor(0, 1);
      lcd.print("distance!");

      tone(buzzer_small, 1000, 50);
      delay(250);
    }

    else if (val_1 == 'E') //觸發測距(右)，燈號顯示(右邊)
    {
      digitalWrite(right_distance_LED, HIGH);
      digitalWrite(right_distance_LED, LOW);
    }

    else if (val_0 == 'F') //觸發測距(左)，燈號顯示
    {
      digitalWrite(left_distance_LED, HIGH);
      digitalWrite(left_distance_LED, LOW);
    }

    else if (val_2 == 'G') //觸發測距(後)，燈號顯示
    {
      digitalWrite(back_distance_LED, HIGH);
      digitalWrite(back_distance_LED, LOW);
    }

    else if (val_2 == 'H') //觸發電池偵測，電壓或電流較低，亮綠燈
    {
      RGBoff_battery();
      analogWrite(red_battery, 0);
      analogWrite(green_battery, 1000);
      analogWrite(blue_battery, 0);
    }

    else if (val_2 == 'I') //觸發電池偵測，電壓或電流很低，亮紅燈
    {
      RGBoff_battery();
      analogWrite(red_battery, 1000);
      analogWrite(green_battery, 0);
      analogWrite(blue_battery, 0);

      tone(buzzer_small, 1000, 50);
      delay(250);
    }

    else if (val_1 == 'X')                              //溫度沒問題，亮藍燈，螢幕警示消失
    {
      RGBoff_temp();
      analogWrite(red_temp, 0);
      analogWrite(green_temp, 0);
      analogWrite(blue_temp, 1000);

      lcd.setCursor(0, 0);
      lcd.print("                ");
    }
    else if (val_0 == 'Y' && val_1 == 'Y' && val_2 == 'Y') //距離沒問題，亮藍燈，螢幕警示消失
    {
      RGBoff_distance();
      analogWrite(red_distance, 0);
      analogWrite(green_distance, 0);
      analogWrite(blue_distance, 1000);

      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
    else if (val_2 == 'Z')                             //電壓沒問題，亮藍燈
    {
      RGBoff_battery();
      analogWrite(red_battery, 0);
      analogWrite(green_battery, 0);
      analogWrite(blue_battery, 1000);
    }
  }
}

int onoff() {
  buttonState = digitalRead(button_Status);

  if (buttonState == 1 and beforeState == 0 )
  {
    pressTime = pressTime + 1;
    Serial.println(pressTime);
  }
  if (buttonState != beforeState)
  {
    delay(10);
  }

  if (pressTime % 2 == 0 )
  {
    noTone(buzzer_loud);
  }
  else
  {
    digitalWrite(buzzer_loud, HIGH);
  }
  beforeState = buttonState;
  delay(1);
}

int RGBoff_temp() {                             //溫度RGB重置燈光
  analogWrite(red_temp, 0);
  analogWrite(green_temp, 0);
  analogWrite(blue_temp, 0);
}
int RGBoff_distance() {                         //距離RGB重置燈光
  analogWrite(red_distance, 0);
  analogWrite(green_distance, 0);
  analogWrite(blue_distance, 0);
}
int RGBoff_battery() {                          //電池RGB重置燈光
  analogWrite(red_battery, 0);
  analogWrite(green_battery, 0);
  analogWrite(blue_battery, 0);
}
