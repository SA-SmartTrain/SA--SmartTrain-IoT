#include <WiFi.h>
#include <PubSubClient.h>
#include <PubSubClientSecure.h>
#include "env.h"
#include "DHT.h"


WiFiClientSecure wifi_client;
PubSubClient mqtt(wifi_client);

const int trigg = 22; //sensor ultrassônico
const int echo = 23;

const byte pino_led = 19; //led rgb
const byte red_pin = 14;
const byte green_pin = 26;
const byte blue_pin = 25;

int ldr = 34; //luminosidade
int valorldr = 0;

#define DHTPIN 4 //DHT
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const String brokerUser = "" ;
const String brokerPass = "" ;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  wifi_client.setInsecure();
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
  mqtt.subscribe(TOPIC_LUMINOSIDADE);
  mqtt.setCallback(callback);
  Serial.println("\nConectado ao Broker!");

}


void loop() {
  // put your main code here, to run repeatedly:
 long dist_1 = lerDistancia(trig, echo){
  if (dist_1 < 10){
     mqtt.publish(TOPIC_ULTRASSONICO, "Presente");
  }





  delay(100);
  mqtt.loop();
}


// void callback(char* topic, byte* payload, unsigned long length){
//   String MensagemRecebida = "";
//   if(pino_led == HIGH){
//     digitalWrite (pino_led,LOW);
//     delay(200);
//     digitalWrite(pino_led,HIGH);
//     MensagemRecebida += (char) payload[pino_led]; //Pega cada letra de payload e junta na mensagem

//   }
//   Serial.println(MensagemRecebida);
// }

void connectToBroker(){
  Serial.println("Conectado ao brooker!");
}

//Sensor ultrassônico
long lerDistancia(byte trigg_pin, byte echo_pin) {
  digitalWrite(trigg_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigg_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigg_pin, LOW);
  
  long duracao = pulseIn(echo_pin, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  
  return distancia;
} //sensor ultrassônico



void callback(char* topic, byte* payload, unsigned long length) {
  String MensagemRecebida = "";
  for(int i = 0; i < length; i++){
    MensagemRecebida += (char) payload[i];
  }
  Serial.println(MensagemRecebida);

  if( topic == TOPIC_PRESENCE1){
    if(MensagemRecebida == "Acender"){
      digitalWrite(pino_led, HIGH);
    } else (MensagemRecebida == "Apagar"){
      digitalWrite(pino_led, LOW)
    }
  }
}
void connectToBrooker(){
  Serial.println("Conectado ao Brooker...");
}

