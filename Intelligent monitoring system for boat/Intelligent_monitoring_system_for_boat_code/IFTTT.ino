#include <ESP8266WiFi.h>

const char* ssid = "73**";
const char* password = "24******";
const char* host = "maker.ifttt.com";
const char* apiKey = "nIDv0_0********hKH**F";

int pin_water = 2;     //D4腳位
int pin_battery = 15;  //D8腳位

const char* PIN_state = "not pressed";

void PINStatus() {
  PIN_state = "SOS pressed";
}

void setup() {
  Serial.begin(9600);

  delay(100);
  pinMode(pin_water, OUTPUT);
  pinMode(pin_battery, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(pin_water), PINStatus, FALLING);
  attachInterrupt(digitalPinToInterrupt(pin_battery), PINStatus, FALLING);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println(digitalRead(pin_water));

  if (digitalRead(pin_water) == HIGH) 
  {
    Serial.print("connecting to ");
    Serial.println(host);

    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) 
    {
      Serial.println("connection failed");
      return;
    }

    String url = "/trigger/water_level_device/with/key/";
    url += apiKey;

    Serial.print("Requesting URL: ");
    Serial.println(url);
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Content-Type: application/x-www-form-urlencoded\r\n" +
                 "Content-Length: 13\r\n\r\n" +
                 "value1=" + PIN_state + "\r\n");
  }
 
  if (digitalRead(pin_battery) == HIGH) 
  {
    Serial.print("connecting to ");
    Serial.println(host);

    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) 
    {
      Serial.println("connection failed");
      return;
    }

    String url = "/trigger/battery_voltage_device/with/key/";
    url += apiKey;

    Serial.print("Requesting URL: ");
    Serial.println(url);
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Content-Type: application/x-www-form-urlencoded\r\n" +
                 "Content-Length: 13\r\n\r\n" +
                 "value1=" + PIN_state + "\r\n");
  }
  delay(100);
}
