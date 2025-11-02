#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h> //Inclusão de Biblioteca (hivemq)
#include "env.h"
#include <ESP32Servo.h>// Inclusão de Biblioteca Servo Motor

WiFiClientSecure wifi_client; //Criando Cliente WIFI
PubSubClient mqtt(wifi_client); //Criando Cliente MQTT

const int LED = 2; //Definição de pino referente ao LED
const servo1 = a0; //Definição do Pino referente ao Servo Motor 1;
const servo2 = a0; //Definição do Pino referente ao Servo Motor 2;
const int iluminacao_led = a0; //Definição do Pino referente ao LED de iluminação;

Servo servo1; //Servo Motor 1
Servo servo2; //Servo Motor 2



const String brokerUser = "";
const String brokerPass = "";

void setup() {
  Serial.begin(115200);
  servo1.attach(servo1); //Definição do pino Servo Motor 1;
    servo2.attach(servo2); //Definição do pino Servo Motor 2;
    servo1.write(0); //Posição inicial;
    servo2.write(0);//Posição inicial;


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
  mqtt.subcribe(TOPIC_SERVO1()); 
    mqtt.subcribe(TOPIC_SERVO2());

  mqtt.setCallback(callback);
  Serial.println("\nConectado ao broker!"); //Mensagem de confirmação exibida no Monitor Serial
}
void callback(char* topic, byte* payload, unsigned long lenght) {
  String MensagemRecebida = "";
  for (int i = 0; i < lenght; i++){
    MensagemRecebida += (char) payload[i]; //Cada letra de payload e junta na mensagem

String topicStr = String(topic);
if (topicStr == TOPIC_SERVO1) { //Condicional referente ao Servo Motor 1;
  int angulo = 90; //Definição do ângulo solicitado;
  servo1.write(angle);//Movimentação;
  mqtt.publish(TOPIC_STATE_SERVO1, String(angulo).c_str()); //Publicação do estado do Servo Motor 1;
  return;
} else if (topicStr == TOPIC_SERVO2) { //Condicional referente ao Servo Motor 2;
  int angulo = 90; //Definição do ângulo solicitado;
  servo2.write(angle); //Movimentação;
  mqtt.publish(TOPIC_STATE_SERVO2, String(angulo).c_str()); //Publicação do estado do Servo Motor 2;
  return;
}

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
  String mensagem = "";                       //Dentro do Loop, para recriá-la
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