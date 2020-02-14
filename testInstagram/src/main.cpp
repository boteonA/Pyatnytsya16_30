#include <Arduino.h>
#include "WiFi.h"
#include "InstagramStats.h"
#include "WiFiClientSecure.h"
#include "JsonStreamingParser.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

char ssid[] = "Boteon MikroTik"; // your network SSID (name)
char password[] = "Boteon12345"; // your network key
WiFiClientSecure client;
InstagramStats instaStats(client);

#define SERVOMIN  150 
#define SERVOMAX  600
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

unsigned long delayBetweenChecks = 1000; //mean time between api requests
unsigned long whenDueToCheck = 0;
//Inputs
String userName = "pavla_s.r"; // Replace your Username
String inputString = ""; // a String to hold incoming data
long followers;

int getInstagramStatsForUser() 
{
  InstagramUserStats response = instaStats.getUserStats(userName);
  Serial.println(response.followedByCount);
  return response.followedByCount;
}

void setup() 
{
  Serial.begin(9600);

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password); 

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);       
    Serial.print("."); 
  }

  Serial.println("WiFi connected"); 

  Wire.begin(21, 22);
  pwm.begin();
  pwm.setPWMFreq(60);

}

void loop() 
{
  
  int followers = getInstagramStatsForUser();
  //int followers = 99;
  int angle = 0;

  if (followers == 0 && followers <= 100) angle = 0;
  if (followers > 100 && followers <= 150) angle = map(followers, 100, 150, 0, 6);
  if (followers > 150 && followers <= 250) angle = map(followers, 150, 250, 6, 12);
  if (followers > 250 && followers <= 500) angle = map(followers, 250, 500, 63, 91);
  if (followers > 500 && followers <= 1000) angle = map(followers, 500, 1000, 91, 119);
  if (followers > 1000 && followers <= 10000) angle = map(followers, 1000, 10000, 135, 150);
  if (followers > 10000 && followers <= 100000) angle = map(followers, 10000, 100000, 147, 180);
  
  pwm.setPWM(0, 0, map(180-angle, 0, 180, SERVOMIN, SERVOMAX));
  Serial.print("angle = ");
  Serial.println(angle);
  delay(15); 
}