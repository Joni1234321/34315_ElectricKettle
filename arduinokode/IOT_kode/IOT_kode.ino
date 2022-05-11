#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include "DHTesp.h"

DHTesp dht;

const char* ssid = "xxxxxxx"; // wifi name
const char* pass = "xxxxxxx"; //password
WiFiClient client;
unsigned long channelID = xxxxxxx; //TS channal
const char * WriteAPIKey = "xxxxxxx"; //TS API Write
const char * ReadAPIKey = "xxxxxxx"; //TS API Read
const char* server = "api.thingspeak.com";
const int postDelay = 20 * 1000; //post data every 20 seconds

int atomizer = 4; //D2:4   D1:5
int waterlevel = A0;
int heatlamp = 15; //D8

int humTweet = 0;
int tempTweet = 0;
int waterTweet = 0;
int tweet = 0;

bool sendtweet = false;
String tweetmessage ="";

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  pinMode(atomizer, OUTPUT);
  pinMode(heatlamp,OUTPUT);
  dht.setup(14, DHTesp::DHT11); // D5
  WiFi.begin(ssid, pass);
}

void loop() {
  Serial.println();
  ThingSpeak.begin(client);
  client.connect(server, 80);
  long GoalHumidity = ThingSpeak.readLongField(channelID, 4,ReadAPIKey);
  long GoalTemp = ThingSpeak.readLongField(channelID, 3,ReadAPIKey);
  client.stop();
  float humidity = dht.getHumidity();
  delay(1100);
  float temperatur = dht.getTemperature();
  Serial.println("Water level: " + String(analogRead(waterlevel)));
  Serial.println("Wanted humidity: " + String(GoalHumidity));
  Serial.println("Wanted Temperature: " + String(GoalTemp));
  if (analogRead(waterlevel) > 300){  //check if there is enough water left
    //----------------- If there is enough water left -------------//
    waterTweet = 0;
    if (humidity < GoalHumidity){ //check if the humidity is high enough
      digitalWrite(atomizer,HIGH); //turn on water atomizer
      humTweet++;
      if (humTweet%112==0){  //if the humidity is too low for too long, send tweet
          sendtweet = true;
          tweetmessage += " You need to refill the water container. ";
          humTweet=0;}
      else{  //if humidity is fine, reset tweet count
        humTweet=0;
        digitalWrite(atomizer,LOW); //turn off water atomizer
      }
    }
    else{digitalWrite(atomizer,LOW);} //if humidity is high enough turn off water atomizer
  }
  else{
    //----------------- If water level is too low -----------------------//
    if (waterTweet%112==0){  //if water level too low for too long, send tweet
      sendtweet = true;
      tweetmessage += "Oh no! You need to refill the water container.";
      waterTweet=0;}
    waterTweet++ ;
    digitalWrite(atomizer,LOW);} //if there is not enough water, turn atomizer off
  if (temperatur<GoalTemp){  // if it is to cold, turn the heatlamp (LED) on
    digitalWrite(heatlamp,HIGH);
    tempTweet++;
    if (tempTweet%112==0){
      sendtweet = true;
      tweetmessage += " You need to turn the heater on. ";
      tempTweet=0;
    }
  }
  else{
    digitalWrite(heatlamp,LOW);
    tempTweet=0;}
  Serial.print(" Humidity (%): " + String(round(humidity)));
  Serial.print(" Temperature (C): " + String(round(temperatur)));
  ThingSpeak.begin(client);
  client.connect(server, 80); //connect(URL, Port)
  if(sendtweet==true){
    int tweet = ThingSpeak.setTwitterTweet("ReptileSystem15", String(tweetmessage));
    Serial.print(tweet);
    sendtweet = false;
    tweetmessage = ""; 
  }
  ThingSpeak.setField(1, temperatur); //set data on the X graph
  ThingSpeak.setField(2, humidity); //set data on the X graph
  ThingSpeak.setField(6,analogRead(waterlevel));
  ThingSpeak.writeFields(channelID, WriteAPIKey);//post everything to TS
  client.stop();
  delay(postDelay); //wait and then post again
  Serial.println();
}
