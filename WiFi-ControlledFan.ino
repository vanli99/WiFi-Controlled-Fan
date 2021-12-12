
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "NodeMCU";  //Name of the SSID
const char* password = "";  //No password needed

ESP8266WebServer server(80);

//IP Address details 
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

uint8_t LEDLOW = D6;
uint8_t LEDMED = D7;
uint8_t LEDHIGH = D8;
uint8_t FAN = D4;

bool LOWstatus = LOW;
bool MEDstatus = LOW;
bool HIGHstatus = LOW;

void setup() {
  Serial.begin(115200);
 
  pinMode(LEDLOW, OUTPUT);
  pinMode(LEDMED, OUTPUT);
  pinMode(LEDHIGH, OUTPUT);
  pinMode(FAN, OUTPUT);
  
  delay(200);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  
  server.on("/", handle_OnConnect);
  server.on("/fanlow", handle_low);
  server.on("/fanmed", handle_med);
  server.on("/fanhigh", handle_high);
  server.on("/fanoff", handle_off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(LOWstatus){
    analogWrite(FAN, 100);
    digitalWrite(LEDLOW, HIGH);
    digitalWrite(LEDHIGH, LOW);
    digitalWrite(LEDMED, LOW);
  }else if (MEDstatus){
    analogWrite(FAN, 175);
    digitalWrite(LEDMED, HIGH);
    digitalWrite(LEDHIGH, LOW);
    digitalWrite(LEDLOW, LOW);
  }else if (HIGHstatus){
    analogWrite(FAN, 255);
    digitalWrite(LEDHIGH, HIGH);
     digitalWrite(LEDLOW, LOW);
    digitalWrite(LEDMED, LOW);
  }else {
    analogWrite(FAN, 0);
    digitalWrite(LEDLOW, LOW);
    digitalWrite(LEDMED, LOW);
    digitalWrite(LEDHIGH, LOW);
  }
}

void handle_OnConnect() {
  LOWstatus = HIGH;
  MEDstatus = LOW;
  HIGHstatus = LOW;
  Serial.println("GPIO6 Status: ON | GPIO7 Status: OFF | GPIO8 Status: OFF | GPIO4 Status: 100");
  server.send(200, "text/html", SendHTML(100)); 
}

void handle_low() {
  LOWstatus = HIGH;
  MEDstatus = LOW;
  HIGHstatus = LOW;
  Serial.println("GPIO6 Status: ON | GPIO7 Status: OFF | GPIO8 Status: OFF | GPIO4 Status: 100");
  server.send(200, "text/html", SendHTML(100)); 
}

void handle_med() {
  LOWstatus = LOW;
  MEDstatus = HIGH;
  HIGHstatus = LOW;
  Serial.println("GPIO6 Status: OFF | GPIO7 Status: ON | GPIO8 Status: OFF | GPIO4 Status: 175");
  server.send(200, "text/html", SendHTML(175)); 
}

void handle_high() {
  LOWstatus = LOW;
  MEDstatus = LOW;
  HIGHstatus = HIGH;
  Serial.println("GPIO6 Status: OFF | GPIO7 Status: OFF | GPIO8 Status: ON | GPIO4 Status: 255");
  server.send(200, "text/html", SendHTML(255)); 
}

void handle_off() {
  LOWstatus = LOW;
  MEDstatus = LOW;
  HIGHstatus = LOW;
  Serial.println("GPIO6 Status: OFF | GPIO7 Status: OFF | GPIO8 Status: OFF | GPIO4 Status: 0");
  server.send(200, "text/html", SendHTML(0)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(int speed){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>WiFi-Controlled Fan</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";

  if(speed == 100){
    ptr +="<p>STATUS</p><a class=\"button button-on\" href=\"/fanlow\">LOW</a>\n";
  }else if (speed == 175) {
    ptr +="<p>STATUS</p><a class=\"button button-on\" href=\"/fanmed\">MED</a>\n";
  }else if (speed == 255) {
    ptr +="<p>STATUS</p><a class=\"button button-on\" href=\"/fanhigh\">HIGH</a>\n";
  }else if(speed == 0) {
    ptr +="<p>STATUS</p><a class=\"button button-off\" href=\"/fanoff\">OFF</a>\n";
  }
  
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
