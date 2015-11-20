#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const char *ssid = "ROVER";
const char *password = "itisopen";

const char *vi = "vi";
const char *vd = "vd";

volatile int velI;
volatile int velD;

String angulos;
String temp;
int indice;

ESP8266WebServer server(80);
void handleRoot() {
  indice = server.args();
  for(;indice>=0;--indice){
    temp = server.argName(indice);
    if(temp == vi){
      velI =  server.arg(indice).toInt();
    }else{
      if(temp == vd){
        velD = server.arg(indice).toInt();
      } 
    }
  }  
	server.send(200, "text/html", angulos);
  Serial.print(' ');
  delay(10);
  Serial.print(velI);
  Serial.print(',');
  Serial.print(velD);
}

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
	delay(1000);
	Serial.begin(115200);
	WiFi.softAP(ssid, password);
  
	IPAddress myIP = WiFi.softAPIP();
	server.on("/", handleRoot);
	server.begin();
  Serial.flush();
  digitalWrite(13,HIGH);
}

void loop() {
	server.handleClient();
  if(Serial.available()){
    angulos = Serial.readString();
  }
}
