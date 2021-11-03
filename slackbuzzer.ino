/**
  This code was written by MAKSIM SURGUY and DAVID CHITA and licensed under the GNU GENERAL PUBLIC LICENSE.
*/

//Libraries and Variables
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wifi.h>
#include "WiFiMulti.h"
#include <HTTPClient.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include <Wire.h>
#include <ArduinoJson.h>

#define ARDUINOJSON_DECODE_UNICODE 1

//Slack config
#define SLACK_BOT_TOKEN "#########"
#define CHANNEL "##########"
bool connected = false;   
bool newMessageState = false;

//Weefee
#define WIFI_SSID       "#########"
#define WIFI_PASSWORD   "#########"
WiFiMulti WifiMulti2;

//Websockets
WebSocketsClient webSocket;

//Define Button
const int buttonPin = 35;
int buttonState = 0;

//Vibration and Timer
int vibePin = 15;
int vibeState = LOW;            
unsigned long previousMillis = 0;       
const long interval = 1000; 

//Graphics
TFT_eSPI tft = TFT_eSPI(); 
#define GFXFF 1
#define aws_width 125
#define aws_height 66
PROGMEM const uint8_t aws[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x0F, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xF8, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xC0, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 
  0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xF8, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x0F, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xF8, 0xFF, 0xE5, 0xEB, 0xD7, 0xCF, 0x0F, 0xFC, 0xFF, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xC1, 0xC3, 0x83, 
  0x8F, 0x03, 0xF0, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 
  0xFF, 0xC0, 0xC3, 0x83, 0x87, 0xC3, 0xF0, 0xFF, 0x0F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0xC7, 0x83, 0xC7, 0xE1, 0xE1, 0xFF, 
  0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xC8, 0xC7, 0x83, 
  0xC7, 0xF1, 0xE1, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 
  0xFF, 0x80, 0xC7, 0x83, 0xC7, 0xF1, 0xE3, 0xFF, 0x7F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xC0, 0xFF, 0x7F, 0x8C, 0x87, 0x83, 0xC7, 0xF0, 0xFF, 0xFF, 
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x7F, 0x8C, 0x87, 0x11, 
  0xE3, 0xE1, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 
  0x7F, 0x8C, 0x8F, 0x11, 0xE3, 0x81, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xF0, 0xFF, 0x3F, 0x0C, 0x8F, 0x11, 0xE3, 0x03, 0xFE, 0xFF, 
  0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x3F, 0x1E, 0x8F, 0x11, 
  0xE3, 0x07, 0xF8, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 
  0x3F, 0x1E, 0x8F, 0x31, 0xE3, 0x1F, 0xF0, 0xFF, 0xFF, 0x01, 0x00, 0x00, 
  0x00, 0x00, 0xF0, 0xFF, 0x3F, 0x1E, 0x0E, 0x19, 0xF2, 0xFF, 0xE0, 0xFF, 
  0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x1F, 0x00, 0x1E, 0x38, 
  0xF1, 0xFF, 0xE1, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 
  0x1F, 0x00, 0x1E, 0x38, 0xF0, 0xF9, 0xE3, 0xFF, 0xFF, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0xF0, 0xFF, 0x1F, 0x10, 0x1E, 0x38, 0xF0, 0xF0, 0xC3, 0xFF, 
  0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x0F, 0x3F, 0x1E, 0x38, 
  0xF0, 0xF0, 0xE3, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 
  0x0F, 0x3F, 0x1C, 0x3C, 0xF8, 0xE1, 0xE1, 0xFF, 0xFF, 0x01, 0x00, 0x00, 
  0x00, 0x00, 0xF0, 0xFF, 0x8F, 0x7F, 0x3C, 0x7C, 0xF8, 0x81, 0xF0, 0xFF, 
  0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x87, 0x7F, 0x3C, 0x7C, 
  0xF8, 0x03, 0xF0, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 
  0x8F, 0x7F, 0x79, 0x7C, 0xFA, 0x0F, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  };

//Process slack message and relay to buzzer
void processSlackMessage(String receivedpayload) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, receivedpayload);
    DeserializationError error = deserializeJson(doc, receivedpayload);

 if (error) {
    Serial.print(F("deserializeJson() failed with code "));

    Serial.println(error.c_str());
 return;
  }

    JsonObject obj = doc.as<JsonObject>();
    String type_msg = obj["type"];
    String texto = obj["text"];
    String channel = obj["channel"];

    if(type_msg.equals("message")) {
      Serial.println(type_msg + " "+ channel + " " + texto);
      tft.fillScreen(TFT_BLACK); 
      tft.setTextColor(TFT_GOLD);
      tft.setCursor(0, 0, 2);
      tft.println("======Alert======");
      tft.println(texto);
      newMessageState=true;
   
    
  }  

}

//Sends an Ack when you press a button
void sendAck(String channel) {
  DynamicJsonDocument  ack(1024);
  ack["type"] = "message";
  String msg_ack = "Ack";
  ack["text"] = msg_ack;
  ack["channel"] = channel;
  String json;
  serializeJson(ack, json);
  webSocket.sendTXT(json);
  Serial.println("Replied with Ack");
  delay(100);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.println("Ack sent");
  digitalWrite(vibePin, LOW);
  newMessageState=false;
 
}

// Called on each web socket event. 
// Handles disconnection, and incoming messages from Slack.
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length){
  
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.printf("[WebSocket] Disconnected :-( \n");
      connected = false;
      break;

    case WStype_CONNECTED:
      Serial.printf("[WebSocket] Connected to: %s\n", payload);
      
      break;

     case WStype_TEXT:
      Serial.printf("[WebSocket] Message: %s\n", payload);
      
      String receivedpayload;
      for (int i = 0; i < length; i++)
      {
         receivedpayload += (char)payload[i];
      }
        
         processSlackMessage(receivedpayload);
      
      break;
  }
}

//Establishes a bot connection to Slack:
//1. Performs a REST call to get the WebSocket URL
//2. Conencts the WebSocket
//   Returns true if the connection was established successfully.
bool connectToSlack() {
  
  HTTPClient http;
  Serial.printf("Connecting to\n");
  Serial.printf("slack.com/api/rtm.connect");
  Serial.printf("\n");
  http.begin("https://slack.com/api/rtm.connect?token=" SLACK_BOT_TOKEN);
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {    
    Serial.printf("HTTP GET failed with code %d\n", httpCode);
    return false;
  }
  
  WiFiClient *client = http.getStreamPtr();
  client->find("wss:\\/\\/");
  String host = client->readStringUntil('\\');
  String path = client->readStringUntil('"');
  path.replace("\\/", "/");

  // Step 2: Open WebSocket connection and register event handler
  Serial.println("WebSocket Host=" + host + " Path=" + path);
  webSocket.beginSSL(host, 443, path, "", "");
  webSocket.onEvent(webSocketEvent);
  Serial.println("\nConnected to slack");
  tft.fillScreen(TFT_BLACK); 
  tft.setTextColor(TFT_GOLD, TFT_BLACK);
  tft.println("Connected to slack");
  
   // start heartbeat
   // ping server every 15000 ms
   // expect pong from server within 3000 ms
   // consider connection disconnected if pong is not received 2 times
   webSocket.setReconnectInterval(15000);
   webSocket.enableHeartbeat(15000, 3000, 2);
   return true;
}

void setup() {
  //Initialize serial
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  //Initialize IO
  pinMode(vibePin, OUTPUT); 
  pinMode(buttonPin, INPUT_PULLUP);
  
  //Initialize Graphics and AWS Slack Buzzer Welcome Screen
  tft.begin(); 
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK); 
  tft.drawXBitmap(15,14,aws, aws_width , aws_height,TFT_GOLD, TFT_BLACK);
  tft.setTextColor(TFT_GOLD, TFT_BLACK);
  tft.drawString("AWS Slack Buzzer!", 20, 100, GFXFF);

  //Connect to Weefee and get the time
  WifiMulti2.addAP(WIFI_SSID, WIFI_PASSWORD);
  while (WifiMulti2.run() != WL_CONNECTED) {
    delay(100);
  }
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

}

void loop() {

  webSocket.loop();
   if(!connected){
  connected = connectToSlack();
  }

   // Vibration and Button Logic Loop

   // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  unsigned long currentMillis = millis();
  if(buttonState == LOW && newMessageState == true) {

    sendAck(CHANNEL);
  }
  if(buttonState == HIGH && newMessageState == true) {   
    

  if (currentMillis - previousMillis >= interval) {
    // save the last time you vibed
    previousMillis = currentMillis;

    // if the vibe is off turn it on and vice-versa:
    if (vibeState == LOW) {
      vibeState = HIGH;
    } else {
      vibeState = LOW;
    }

    // set the vibeState
    digitalWrite(vibePin, vibeState);
  }

  }
 


}