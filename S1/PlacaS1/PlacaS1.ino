#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"
#include <DHT.h>

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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  wifi_client.setInsecure();
  pinMode(pino_led, OUTPUT);           
  digitalWrite(pino_led, LOW);        
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
  // mqtt.subscribe(topic.c_str());
  mqtt.subscribe(TOPIC_LUMINOSIDADE);
  mqtt.setCallback(callback);
  Serial.println("\nConectado ao Broker!");

  pinMode(ldr,INPUT);
}

void loop() {
  long distancia = lerDistancia(trigg, echo){
    if (distancia < 10){
     mqtt.publish(TOPIC_ULTRASSONICO, "Presente");
  }
  //ldr
  valorldr(ldr);
  mqtt.loop();
}

long valorldr(int ldr_pin){
  int luminance = analogRead(ldr_pin);
  if (luminance < 400){
     mqtt.publish(TOPIC_luminosidade, "Presente");//mudar
  }
  return luminance;
}

long lerDistancia(byte trigg, byte echo) {
  digitalWrite(trigg, LOW);
  delayMicroseconds(2);
  digitalWrite(trigg, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigg, LOW);
  
  long duracao = pulseIn(echo, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  
  return distancia;
}


void callback(char* topic, byte* payload, unsigned long length) {
  String MensagemRecebida = "";
  for(int i = 0; i < length; i++){
    MensagemRecebida += (char) payload[i];
  }
  Serial.println(MensagemRecebida);

  if( topic == TOPIC_PRESENCE){
    if(MensagemRecebida == "Acender"){
      digitalWrite(pino_led, HIGH);
    } else(MensagemRecebida == "Apagar"){
      digitalWrite(pino_led, LOW);
  }

  if (topic == TOPIC_LUMINOSIDADE){ //mudar
    if(MensagemRecebida == "Acender"){
      digitalWrite(pino_led, HIGH);
    } else(MensagemRecebida == "Apagar"){
      digitalWrite(pino_led, LOW);
  }
}
void connectToBrooker(){
  Serial.println("Conectado ao Brooker...");
}