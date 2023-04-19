// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""

#define BLYNK_FIRMWARE_VERSION "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

#include "BlynkEdgent.h"
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>
#include <IRremote.h>

IRrecv IR(18);
int motionPin = 14;
int motion_data = 0;
const long utcOffsetInSeconds = 21600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);
int HH, MM, final_time, sec;
int time_blynk, data;
Servo servo_1;
BlynkTimer timer;

BLYNK_WRITE(V0) {
  int data = param.asInt();
  if (data == 1) {
    feedThem();
  }
}
BLYNK_WRITE(V1) {
  time_blynk = param.asInt();
  Serial.println(time_blynk);
}
void setup() {
  pinMode(motionPin, INPUT);
  IR.enableIRIn();
  Serial.begin(115200);
  delay(100);
  BlynkEdgent.begin();
  timer.setInterval(100L, sendMotion);
  servo_1.attach(23);
  timeClient.begin();
}

void loop() {
irRemote();
  timeClient.update();
  HH = timeClient.getHours();
  MM = timeClient.getMinutes();
  sec = timeClient.getSeconds();
  final_time = 3600 * HH + 60 * MM + sec;
  if (time_blynk == final_time) {
    Serial.println(time_blynk);
    Serial.println(final_time);
    feedThem();
  }
  BlynkEdgent.run();
  timer.run();
}
void feedThem() {
  servo_1.write(180);
  delay(1000);
  servo_1.write(0);
}
void sendMotion() {
  int motion_data = digitalRead(motionPin);
  Serial.println(motion_data);
  delay(200);
  Blynk.virtualWrite(V2, motion_data);
  delay(300);

}
void irRemote() {
  if (IR.decode()) {
    if (IR.decodedIRData.decodedRawData == 0xE31CFF00) feedThem();
    delay(500);
    IR.resume();
  }
}
