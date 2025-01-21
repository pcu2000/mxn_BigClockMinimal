#include <ESP32Time.h>
#include <WiFiClientSecure.h>

const int anzD0 = 13;
const int anzD1 = 12; //14
const int anzD2 = 26;
const int anzD3 = 27;
const int anzBL = 14;
const int anzLE0 = 25;
const int anzLE1 = 33; //17
const int anzLE2 = 4;
const int anzLE3 = 15;

const int startButton = 34;
const int endSensor = 39;

const int pointUp = 16;
const int pointDown = 17;
const int chalangeBarrier = 21;
const int chalangeRover = 32;

const int offset = 0;
ESP32Time rtc(offset); // create an instance with a specifed offset in seconds
int second = 0;
int minute = 0;
int hour = 0;
int secondOld = 0;
int second_1er = 0;
int second_10er = 0;
int minute_1er = 0;
int minute_10er = 0;
int hour_1er = 0;
int hour_10er = 0;

int second3 = 0;
int second3_1er = 0;
int second3_10er = 0;
int second3_100er = 0;
int milis = 0;
int milis_100er = 0;

int state = 20;
const int STARTUP = 0;
const int STARTWAIT = 5;
const int CHANGECHALLANGE = 10;
const int COWNTDOWN = 20;
const int RACE = 30;
const int ENDRACE = 80;


const char* ssid     = "wlan-name";     // your network SSID (name of wifi network)
const char* password = "your-password"; // your network password

const char* server = "https://blablabla.azurewebsites.net/";

int wlanConnectCounter = 0;
int status = WL_IDLE_STATUS;                     // the Wifi radio's status

WiFiClientSecure client;

void actualDisp();
void actualDisp100();
void valueToDisp(int digit, int value);
void printWifiStatus();

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("start");

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  //WiFi.begin(ssid, password);
  WiFi.begin(ssid);
  
  Serial.println(WiFi.macAddress());
  
  /*while ( status != WL_CONNECTED && wlanConnectCounter <= 5) {
    Serial.print("Attempting to connect to WEP network, SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid);
    Serial.print("Anzahl Versuche: ");
    Serial.println(wlanConnectCounter);
    wlanConnectCounter++;
    // wait 10 seconds for connection:
    delay(10000);
  }*/

  if(status == WL_CONNECTED){
    Serial.println("Wifi verbunden");
  }else{
    Serial.println("Wifi nicht verbunden");
    Serial.println(WiFi.status());
  }
  printWifiStatus();
  client.setInsecure(); // don't use a root cert
  //Serial.println(WiFi.macAddress());
  rtc.setTime(0, 0, 0, 1, 1, 2023);
  pinMode(anzD0, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzD1, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzD2, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzD3, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzBL, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzLE0, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzLE1, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzLE2, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzLE3, OUTPUT);    // sets the digital pin 13 as output

  pinMode(startButton, INPUT);
  pinMode(endSensor, INPUT);

  pinMode(pointUp, OUTPUT);
  pinMode(pointDown, OUTPUT);
  pinMode(chalangeBarrier, OUTPUT);
  pinMode(chalangeRover, OUTPUT);

  digitalWrite(anzD0, LOW);
  digitalWrite(anzD1, LOW);
  digitalWrite(anzD2, LOW);
  digitalWrite(anzD3, LOW);
  digitalWrite(anzLE0, LOW);
  digitalWrite(anzLE1, LOW);
  digitalWrite(anzLE2, LOW);
  digitalWrite(anzLE3, LOW);

  digitalWrite(anzBL, HIGH);

  
  digitalWrite(pointUp, HIGH);
  digitalWrite(pointDown, HIGH);
  digitalWrite(chalangeBarrier, HIGH);
  digitalWrite(chalangeRover, HIGH);

}

void loop() {
  //digitalWrite(anzBL, HIGH); // sets the digital pin 13 on
  //delay(500);            // waits for a second
  //digitalWrite(anzBL, LOW);  // sets the digital pin 13 off
  //delay(500);            // waits for a second
  switch (state) {
    case STARTUP:
      if(!digitalRead(startButton)){
        state = STARTWAIT;
          Serial.println(state);
          }
      break;
    case STARTWAIT:
      if(digitalRead(startButton) && !digitalRead(endSensor)){
        state = CHANGECHALLANGE;
        Serial.println(state);
        }
      break;
    case CHANGECHALLANGE:
      //getAnswersCorrect();
      //digitalWrite(chalangeProp, HIGH);
      digitalWrite(chalangeBarrier, LOW);
      digitalWrite(chalangeRover, LOW);
      state = COWNTDOWN;
      Serial.println(state);
      break;
    case COWNTDOWN:    
      valueToDisp(0, 4);
      delay(10);
      valueToDisp(1, 4);
      delay(10);
      valueToDisp(2, 2);
      delay(10);
      valueToDisp(3, 4);
      delay(1000);    
      valueToDisp(0, 3);
      delay(10);
      valueToDisp(1, 3);
      delay(10);
      valueToDisp(3, 3);
      delay(1000);    
      valueToDisp(0, 2);
      delay(10);
      valueToDisp(1, 2);
      delay(10);
      valueToDisp(3, 2);
      delay(1000);    
      valueToDisp(0, 1);
      delay(10);
      valueToDisp(1, 1);
      delay(10);
      valueToDisp(3, 1);
      delay(1000);
      rtc.setTime(0, 0, 0, 17, 1, 2025);
      //digitalWrite(startCoil, LOW);    
      valueToDisp(0, 0);
      delay(10);
      valueToDisp(1, 0);
      delay(10);
      valueToDisp(3, 0);
      delay(1000);
      state = RACE;
      Serial.println(state);
      break;
    case RACE:
      if(digitalRead(endSensor)){
        //digitalWrite(startCoil, HIGH);
          //state = ENDRACE;
        Serial.println(state);
      }
      break;
    case ENDRACE:
      digitalWrite(anzBL, HIGH);
      delay(500);
      digitalWrite(anzBL, LOW);
      delay(500);
      if(digitalRead(startButton)){    
      valueToDisp(0, 8);
      delay(10);
      valueToDisp(1, 8);
      delay(10);
      valueToDisp(3, 8);
      
      //digitalWrite(chalangeProp, LOW);
      digitalWrite(chalangeBarrier, HIGH);
      digitalWrite(chalangeRover, HIGH);
        state = STARTUP;
      Serial.println(state);
        }
      break;
    default:
      // statements
      break;
  }

  actualDisp();
  //actualMilis();
  //actualDisp100();

  /*second = rtc.getSecond();
  if(secondOld != second && state == RACE){
    second_1er = second % 10;
    second_10er = second / 10;
    minute = rtc.getMinute();
    minute_1er = minute % 10;
    minute_10er = minute / 10;
    valueToDisp(0, second_1er);
    delay(10);
    valueToDisp(1, second_10er);
    delay(10);
    valueToDisp(2, minute_1er);
    delay(10);
    valueToDisp(3, minute_10er);
    delay(10);
    secondOld = second;
  }*/

  /*if(digitalRead(startButton)){
    digitalWrite(startCoil, HIGH);
  } else {
    digitalWrite(startCoil, LOW);
    digitalWrite(chalangeProp, LOW);
    digitalWrite(chalangeBarrier, LOW);
    digitalWrite(chalangeRover, LOW);
  }*/

  delay(10);

}

void actualDisp(){
    second = rtc.getSecond();
  if(secondOld != second && state == RACE){
    second_1er = second % 10;
    second_10er = second / 10;
    minute = rtc.getMinute();
    minute_1er = minute % 10;
    minute_10er = minute / 10;
    hour = rtc.getHour(true);
    minute_1er = minute % 10;
    minute_10er = minute / 10;
    if(second % 2 == 1){
      digitalWrite(pointUp, LOW); 
      digitalWrite(pointDown, LOW); 
    } else {
      digitalWrite(pointUp, HIGH); 
      digitalWrite(pointDown, HIGH); 
    }
    valueToDisp(3, second_1er);
    //delay(10);
    valueToDisp(2, second_10er);
    //delay(10);
    valueToDisp(1, minute_1er);
    //delay(10);
    valueToDisp(0, minute_10er);
    //delay(10);
    //valueToDisp(3, hour_1er);
    //delay(10);
    //valueToDisp(4, hour_10er);
    //delay(10);
    /*if(second % 2 == 1){
      valueToDisp(2, 8); 
    } else {
      valueToDisp(2, 2);
    }*/
    //delay(10);
    secondOld = second;
    /*if(minute_1er >= 9 && second_10er >= 5 && second_1er >= 9){
      state = ENDRACE;
    }*/
  }
}


void actualMilis(){
    
  if(state == RACE){
    milis = rtc.getMillis();
    milis_100er = milis / 100;
    valueToDisp(0, milis_100er);
    delay(10);
  }
}

void actualDisp100(){
    second = rtc.getSecond();
  if(secondOld != second && state == RACE){
    minute = rtc.getMinute();
    second3 = (minute * 60) + second;
    second3_1er = second3 % 10;
    second3_10er = (second3 % 100) / 10;
    second3_100er = second3 / 100;
    valueToDisp(1, second3_1er);
    delay(10);
    valueToDisp(2, second3_10er);
    delay(10);
    valueToDisp(3, second3_100er);
    delay(10);
    if(second3 >= 999){
      state = ENDRACE;
    }
    secondOld = second;
  }
}

void valueToDisp(int digit, int value){
  digitalWrite(anzD0, bitRead(value, 0));
  digitalWrite(anzD1, bitRead(value, 1));
  digitalWrite(anzD2, bitRead(value, 2));
  digitalWrite(anzD3, bitRead(value, 3));
  switch (digit) {
    case 3:
      digitalWrite(anzLE0, LOW);
      delay(10);
      digitalWrite(anzLE0, HIGH);
      delay(10);
      break;
    case 2:
      digitalWrite(anzLE1, LOW);
      delay(10);
      digitalWrite(anzLE1, HIGH);
      delay(10);
      break;
    case 1:
      digitalWrite(anzLE2, LOW);
      delay(10);
      digitalWrite(anzLE2, HIGH);
      delay(10);
      break;
    case 0:
      digitalWrite(anzLE3, LOW);
      delay(10);
      digitalWrite(anzLE3, HIGH);
      delay(10);
      break;
    default:
      // statements
      break;
  }
  //digitalWrite(anzLE0, HIGH);
  //digitalWrite(anzLE1, HIGH);
  //digitalWrite(anzLE2, HIGH);
  //digitalWrite(anzLE3, HIGH);

}

void printWifiStatus(){
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Adress: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}
