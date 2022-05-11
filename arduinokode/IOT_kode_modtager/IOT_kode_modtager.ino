#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <LiquidCrystal.h>
const int RS = D2, EN = D3, d4 = D6 , d5 = D7, d6 = D8, d7 = D9;
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);

const char* ssid = "xxxxxxx"; // wifi name
const char* pass = "xxxxxxx"; //password
WiFiClient client;
unsigned long channelID = xxxxxxx; //your TS channal
const char * ReadAPIKey = "xxxxxxx"; //your TS API Read
const char * WriteAPIKey = "xxxxxxx"; //your TS API Write
const char* server = "api.thingspeak.com";
const int postDelay = 20 * 1000; //post data every 20 seconds

int waterlevel = A0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  WiFi.begin(ssid, pass);
  int waterlevel = A0;
}

void loop() {
  ThingSpeak.begin(client);
  client.connect(server, 80); //connect(URL, Port)
  int Humidity = ThingSpeak.readFloatField(channelID, 1,ReadAPIKey);
  int Temp = ThingSpeak.readFloatField(channelID, 2,ReadAPIKey);
  client.stop();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Humidity: ");lcd.print(String(Humidity));
  lcd.setCursor(0,1);
  lcd.print("Temperature: ");lcd.print(String(Temp));
  delay(postDelay); //wait and then post again
}
