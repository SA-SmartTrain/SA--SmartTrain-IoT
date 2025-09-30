#include <WiFi.h>
WiFiClient client; //nome do cliente wifi
const String SSID = "nome da rede"; //nome da rede wifi
const String PASS = "senha da rede"; //senha da rede

void setup() {
  Serial.begin(115200);
  client.begin(nome, senha); //tenta conectar na rede
  Serial.println("Conectado no WiFi");
  while(client.status()! = WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
