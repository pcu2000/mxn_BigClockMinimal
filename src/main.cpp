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

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

/* For PWMOut*/
const int PWM_CHANNEL_BACKLIGHT = 0;
const int PWM_FREQ = 500;
const int PWM_RESOLUTION = 8;

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

int state = 20;
const int STARTUP = 0;
const int COWNTDOWN = 20;
const int PRINT_TIME = 30;


const char* ssid     = "wlan-name";     // your network SSID (name of wifi network)
const char* password = "your-password"; // your network password

const char* server = "https://blablabla.azurewebsites.net/";

int wlanConnectCounter = 0;
int status = WL_IDLE_STATUS;                     // the Wifi radio's status

WiFiClientSecure client;

void setBackLight(uint8_t brightness, bool onOff);
void printActualTime();
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
    /*---------set with NTP---------------*/
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)){
      rtc.setTimeStruct(timeinfo); 
    }
    Serial.println(rtc.getTime());          //  (String) 15:24:38
    Serial.println(rtc.getDate());          //  (String) Sun, Jan 17 2021
  }else{
    Serial.println("Wifi nicht verbunden");
    Serial.println(WiFi.status());
    rtc.setTime(0, 19, 7, 27, 1, 2025);
  }
  printWifiStatus();
  client.setInsecure(); // don't use a root cert
  //Serial.println(WiFi.macAddress());




  pinMode(anzD0, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzD1, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzD2, OUTPUT);    // sets the digital pin 13 as output
  pinMode(anzD3, OUTPUT);    // sets the digital pin 13 as output
  //pinMode(anzBL, OUTPUT);    // sets the digital pin 13 as output
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

  //digitalWrite(anzBL, HIGH);

  
  digitalWrite(pointUp, HIGH);
  digitalWrite(pointDown, HIGH);
  digitalWrite(chalangeBarrier, HIGH);
  digitalWrite(chalangeRover, HIGH);

  ledcSetup(PWM_CHANNEL_BACKLIGHT, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(anzBL, PWM_CHANNEL_BACKLIGHT);
  setBackLight(255, 1);

}

void loop() {
  switch (state) {
    case STARTUP:
        state = COWNTDOWN;
      break;
    case COWNTDOWN:    
      valueToDisp(0, 4);
      delay(10);
      valueToDisp(1, 4);
      delay(10);
      valueToDisp(2, 4);
      delay(10);
      valueToDisp(3, 4);
      delay(1000);    
      valueToDisp(0, 3);
      delay(10);
      valueToDisp(1, 3);
      delay(10);
      valueToDisp(2, 3);
      delay(10);
      valueToDisp(3, 3);
      delay(1000);    
      valueToDisp(0, 2);
      delay(10);
      valueToDisp(1, 2);
      delay(10);
      valueToDisp(2, 2);
      delay(10);
      valueToDisp(3, 2);
      delay(1000);    
      valueToDisp(0, 1);
      delay(10);
      valueToDisp(1, 1);
      delay(10);
      valueToDisp(2, 1);
      delay(10);
      valueToDisp(3, 1);
      delay(1000);
      valueToDisp(0, 0);
      delay(10);
      valueToDisp(1, 0);
      delay(10);
      valueToDisp(2, 0);
      delay(10);
      valueToDisp(3, 0);
      delay(1000);
      state = PRINT_TIME;
      Serial.println(state);
      break;
    case PRINT_TIME:
      printActualTime();
      break;
    default:
      // statements
      break;
  }

  delay(10);

}

/**
 * @brief Diese Fuktion ist für das einstellen der Hintergrundbeleuchtung verantwortlich
 * 
 */
void setBackLight(uint8_t brightness, bool onOff){
  if(onOff == true){
    ledcWrite(PWM_CHANNEL_BACKLIGHT, brightness);
  }else{
    ledcWrite(PWM_CHANNEL_BACKLIGHT, 0);
  }
}


void printActualTime(){
  second = rtc.getSecond();
  int dayOfWeek = rtc.getDayofWeek();
  if(secondOld != second){
    second_1er = second % 10;
    second_10er = second / 10;
    minute = rtc.getMinute();
    minute_1er = minute % 10;
    minute_10er = minute / 10;
    hour = rtc.getHour(true);
    hour_1er = hour % 10;
    hour_10er = hour / 10;
    if(second % 2 == 1){
      digitalWrite(pointUp, LOW); 
      digitalWrite(pointDown, LOW); 
      //setBackLight(255, 1);
    } else {
      digitalWrite(pointUp, HIGH); 
      digitalWrite(pointDown, HIGH); 
      //setBackLight(255, 1);
    }
    //valueToDisp(3, second_1er);
    //delay(10);
    //valueToDisp(2, second_10er);
    //delay(10);
    valueToDisp(3, minute_1er);
    //delay(10);
    valueToDisp(2, minute_10er);
    //delay(10);
    valueToDisp(1, hour_1er);
    //delay(10);
    valueToDisp(0, hour_10er);
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
   if((hour > 6 || hour < 18)&&(dayOfWeek > 0 && dayOfWeek < 6)){
    setBackLight(255, 1);
   }else{
    setBackLight(0, 1);
   }
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
