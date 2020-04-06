#define BLYNK_PRINT Serial
#define USING_AXTLS
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClientSecureAxTLS.h>
using namespace axTLS;
// Config connect WiFi
char auth[] = "###########################";
#define WIFI_SSID "###################"
#define WIFI_PASSWORD "############"
// Line config
#define LINE_TOKEN "#########################"
#define SW D2
#define PiezoPin D7
String message = "%E0%B8%95%E0%B8%A3%E0%B8%A7%E0%B8%88%E0%B8%AA%E0%B8%AD%E0%B8%9A%E0%B8%94%E0%B9%88%E0%B8%A7%E0%B8%99!!!%20%E0%B8%A1%E0%B8%B5%E0%B8%84%E0%B8%99%E0%B9%80%E0%B8%82%E0%B9%89%E0%B8%B2%E0%B8%9A%E0%B9%89%E0%B8%B2%E0%B8%99";
void Line_Notify(String message) ;
void setup() {
pinMode(SW, INPUT);
pinMode(PiezoPin, OUTPUT);
Serial.begin(9600);
WiFi.mode(WIFI_STA);
// connect to wifi.
  Blynk.begin(auth,WIFI_SSID,WIFI_PASSWORD); 
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("connecting");
while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}
Serial.println();
Serial.print("connected: ");
Serial.println(WiFi.localIP());
}
void loop() {
    Blynk.run(); 
if (digitalRead(SW) == LOW) {
while (digitalRead(SW) == LOW) delay(10);
Serial.println("Enter !");
Line_Notify(message);
loop:
digitalWrite(PiezoPin, HIGH);
delay(1000);
digitalWrite(PiezoPin, LOW);
delay(3000);
goto loop;   
// Serial.println();
}
delay(10);
}
void Line_Notify(String message) {
WiFiClientSecure client;
if (!client.connect("notify-api.line.me", 443)) {
Serial.println("connection failed");
return;
}
String req = "";
req += "POST /api/notify HTTP/1.1\r\n";
req += "Host: notify-api.line.me\r\n";
req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
req += "Cache-Control: no-cache\r\n";
req += "User-Agent: ESP8266\r\n";
req += "Content-Type: application/x-www-form-urlencoded\r\n";
req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
req += "\r\n";
req += "message=" + message;
// Serial.println(req);
client.print(req);
delay(20);
// Serial.println("-------------");
while (client.connected()) {
String line = client.readStringUntil('\n');
if (line == "\r") {
break;
}
//Serial.println(line);
}
// Serial.println("-------------");
}
