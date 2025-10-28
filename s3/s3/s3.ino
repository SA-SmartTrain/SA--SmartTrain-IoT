#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h> //Inclusão de Biblioteca (hivemq)
#include "env.h"

WiFiClientSecure wifi_client; //Criando Cliente WIFI
PubSubClient mqtt(wifi_client); //Criando Cliente MQTT

const int LED = 2; //Definição de pino referente ao LED

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  Serial.begin(115200);
  wifi_client.setInsecure(); //Broker ignorar o Certificado de Segurança/Autenticação
  WiFi.begin( WIFI_SSID, WIFI_PASS);  //tenta conectar na rede
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!");

  mqtt.setServer(BROKER_URL, BROKER_PORT);
  String clientID = "S3_beatrizcercal";
  clientID += String(random(0xffff), HEX);
  while (mqtt.connect(clientID.c_str()) == 0) {
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(TOPIC_PRESENCE1());
  mqtt.setCallback(callback);
  Serial.println("\nConectado ao broker!");
}
void callback(char* topic, byte* payload, unsigned long lenght) {
  String MensagemRecebida = "";
  for (int i = 0; i < lenght; i++){
    MensagemRecebida += (char) payload[i]; //Cada letra de payload e junta na mensagem

  }
  Serial.println(MensagemRecebida);
  if (mensagem == "1"){
    digitalWrite(LED, high); //Condicional para ligar o LED
    Serial.println("LED Ligado"); //Exibição no Monitor Serial
  } else if (mensagem == "0"){
    digitalWrite(LED, low); //Condicional para desligar o LED
     Serial.println("LED Desligado"); //Exibição no Monitor Serial

  }
}
void loop() {

  // put your main code here, to run repeatedly:
  String mensagem = "";                       //Dentro do Loop, para recria-lá
  if (Serial.available() > 0) {               //Caracteres disponíveis na "fila"
    mensagem = Serial.readStringUntil('\n');  //Leitura até "encontrar" o \n, palavra salva dentro da variável "mensagem"
    mensagem = "Beatriz: " + mensagem;       //exibição ao leitor
    mqtt.publish("Miguel",mensagem.c_str()); //enviar para o broker
  }
  mqtt.loop();
}

void connecttoBroker(){
  Serial.println("Conectando ao broker...");
}