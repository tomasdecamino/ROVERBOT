#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

unsigned int port = 2390;

const char *ssid = "ROVER";
const char *password = "itisopen";

char angulos[128];
int indice;
int packetSize;
char buffer[255];

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
 
  Udp.begin(port);
}

void loop() {
  packetSize = Udp.parsePacket();
  if (packetSize) {
    indice = Udp.read(buffer, 255);
    if (indice > 0) {
      buffer[indice] = 0;
    }
    Serial.print(' ');
    delay(1);
    Serial.print(buffer);
    delay(1);
    Serial.print(' ');
    
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(angulos);
    Udp.endPacket();
  }
  if(Serial.available()){
    indice = Serial.readBytesUntil('s', angulos, 255);
    angulos[indice]='\0';
  }
}
