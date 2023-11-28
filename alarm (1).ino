// #include <WiFiNINA.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>

#define SSID "12"
#define PASS "9414070970im"

int lcdColumns = 16;
int lcdRows = 2;
int flag=1;
const long utcOffsetWinter = 3600; // Offset from UTC in seconds (3600 seconds = 1h) -- UTC+1 (Central European Winter Time)
const long utcOffsetSummer = 7200; // Offset from UTC in seconds (7200 seconds = 2h) -- UTC+2 (Central European Summer Time)
unsigned long lastupdate = 0UL;
unsigned long lastupdate2 = 0UL;
 
// Define NTP Client to get time
WiFiUDP udpSocket;
NTPClient ntpClient(udpSocket, "pool.ntp.org", utcOffsetWinter);
 
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
String myTime = ""; 
String myAlarm = "";
String status = "OFF";
int hrs2=-1, mins2=0;
int hrs=0, mins=0;

// hour vars
// uint8_t D5 = 14; // hour
uint8_t D4 = 2; //hour
uint8_t D8 = 15; // alarm status
uint8_t D7 = 13;
uint8_t D6 = 12; 
int x=0;

void setup()
{
  Serial.begin(9600);
  pinMode(D4, INPUT);
  pinMode(D8, INPUT);
  pinMode(D7, INPUT);
  pinMode(D6,OUTPUT);
  WiFi.begin(SSID, PASS);

   // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Connecting...");
  Serial.print("Connecting to ");
  Serial.print(SSID);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(250);
  }
 
  Serial.println(" Done!");
   
  ntpClient.begin();

  //hour
  // pinMode(D5, INPUT);
}
 
void loop()
{
  showTime();
  // if(flag==1){
    showAlarm();
  // }
  int sv = digitalRead(D4);
  if(sv == 1){
    setHour2();
  }
  int sv2 = digitalRead(D7);
  if(sv2 == 1){
    setMins2();
  }
  int sv3 = digitalRead(D8);
  if(sv3 == 1){
    setAlarm();
  }
  if(status=="ON"){
    checkAlarm();
  }
  delay(300);
    
}

// to match time of alarm
void checkAlarm(){
  // Serial.print("Inside check alarm: ");
  // Serial.println(mins2);
  if(hrs==hrs2 and mins==mins2){
    // Serial.println("Inside condition: ");
    // myAlarm="Ringing...";
    flag=0;
  }
}

// to set alarm and change status
void setAlarm(){
  // Serial.print("Inside alarm: ");
  if(status=="OFF"){
    status="ON";
    // Serial.print("Inside OFF: ");
  }
  else{
    status="OFF";
    hrs2=0;
    mins2=0;
    showAlarm();
    flag=1;
    // Serial.print("Inside ON: ");
  }
}

// to set min
void setMins2(){
  // x=x+1;
  mins2=mins2+1;
  if(mins2==60){
    mins2=0;
  }
  // Serial.print("Mins set: ");
  // Serial.println(mins2);
  pinMode(LED_BUILTIN, OUTPUT);
}

// to set hour
void setHour2(){
  // x=x+1;
  hrs2=hrs2+1;
  if(hrs2==24){
    hrs2=0;
  }
  // Serial.print("Hour set: ");
  // Serial.println(hrs2);
  pinMode(LED_BUILTIN, OUTPUT);
}

// to display clock time
void showTime(){
    ntpClient.update();
     int tmp = ntpClient.getHours()+4 ;
     if(tmp>=24)
     {
     tmp = tmp-24;
     }
    hrs = tmp+(ntpClient.getMinutes()+30)/60;
    mins = (ntpClient.getMinutes()+30)%60;
    myTime = "Time: ";
    myTime.concat(hrs);
    myTime.concat(":");
    myTime.concat(mins);
    myTime.concat(":");
    myTime.concat(ntpClient.getSeconds());
    // lastupdate = currentMillis;
    // display
    lcd.clear();
    // set cursor to first column, second row
    lcd.setCursor(0,0);
    lcd.print(myTime);
}

// to display alarm status
void showAlarm(){
    if(flag==0){
      myAlarm="Ringing... ON";
      lcd.setCursor(0,1);
      lcd.print(myAlarm);
      digitalWrite(D6 , HIGH);
      delay(1000);
      digitalWrite(D6, LOW);
      delay(1000);
    
    }
    else{
      myAlarm = "Alarm: ";
      myAlarm.concat(hrs2);
      myAlarm.concat(":");
      myAlarm.concat(mins2);
      myAlarm.concat(" ");
      myAlarm.concat(status);
      lcd.setCursor(0,1);
      lcd.print(myAlarm);
    }
}