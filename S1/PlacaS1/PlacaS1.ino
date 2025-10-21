#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"


WiFiClient wifi_client;
PubSubClient mqtt(wifi_client);

const String topic = "Camafeu";
const byte pino_led = 2;


const String brokerUser = "" ;
const String brokerPass = "" ;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Conectando no WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!");
  mqtt.setServer(BROKER_URL,BROKER_PORT);
  String clientID = "S1";
  clientID += String(random(0xffff),HEX);
  while (mqtt.connect(clientID.c_str()) == 0){
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(TOPIC_PRESENCE1);
  mqtt.setCallback(callback);
  Serial.println("\nConectado ao Broker!");

  pinMode(pino_led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  String mensagem = " ";
  if(Serial.available() > 0) {
    mensagem = Serial.readStringUntil('\n');
    mensagem = "Beatriz: " + mensagem;
    mqtt.publish("S2",mensagem.c_str()); 
  }
  mqtt.loop();

}
void callback(char* topic, byte* payload, unsigned long length){
  String MensagemRecebida = "";
  if(pino_led == HIGH){
    digitalWrite (pino_led,LOW);
    delay(200);
    digitalWrite(pino_led,HIGH);
    MensagemRecebida += (char) payload[pino_led]; //Pega cada letra de payload e junta na mensagem

  }
  Serial.println(MensagemRecebida);
}

void connectToBroker(){
  Serial.println("Conectado ao brooker!");
}
