#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h> //Inclusão de Biblioteca (hivemq)
#include "env.h" //Variáveis, inseridas no gitignore
#include <ESP32Servo.h>// Inclusão de Biblioteca Servo Motor

WiFiClientSecure wifi_client; //Criando Cliente WIFI
PubSubClient mqtt(wifi_client); //Criando Cliente MQTT

const int LED = 14; //Definição de pino referente ao LED
const servo1_pin = 26; //Definição do Pino referente ao Servo Motor 1;
const servo2_pin = 27; //Definição do Pino referente ao Servo Motor 2;
const byte TRIGGER_PIN = 5; //Definição do Pino referente ao Sensor Ultrassônico;
const byte ECHO_PIN = 18; //Definição do Pino referente ao Sensor Ultrassônico;


Servo1 servo1_pin; //Servo Motor 1
Servo2 servo2_pin; //Servo Motor 2

void statusLED(byte status) {
    turnOffLEDs();
    switch (status) {
    case 254:  
        setLEDColor(255, 0, 0);
        break;
    case 1:  
        setLEDColor(150, 255, 0);
        break;


//const String brokerUser = "";
//const String brokerPass = "";

void setup() {
  Serial.begin(115200);
  servo1.attach(servo1_pin); //Definição do pino Servo Motor 1;
    servo2.attach(servo2_pin); //Definição do pino Servo Motor 2;
    servo1.write(0); //Posição inicial;
    servo2.write(0);//Posição inicial;
    pinMode(LED, OUTPUT);//Definição do pino de LED como saída;
    pinMode(  TRIGGER_PIN, OUTPUT); //Saída
    pinMode (ECHO_PIN, INPUT);//Entrada


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
    mqtt.setCallback(callback);
  Serial.println("\nConectado ao broker!"); //Mensagem de confirmação exibida no Monitor Serial

  mqtt.subscribe(TOPIC_PRESENCE1());
  mqtt.subscribe(TOPIC_SERVO1()); 
    mqtt.subscribe(TOPIC_SERVO2());

}
void callback(char* topic, byte* payload, unsigned long lenght) {
  String MensagemRecebida = "";
  for (int i = 0; i < lenght; i++){
    MensagemRecebida += (char) payload[i]; //Cada letra de payload e junta na mensagem

     if (topic == TOPIC_LED){ //Condicional SE, conforme Tópico
      if (MensagemRecebida == "Acender"){ //Mensagem 
        digitalWrite(LED, HIGH); //LED Ligado
      } else{
        digitalWrite(LED, LOW); //LED Desligado
      }
    }
  }

String topicStr = String(topic);
if (topicStr == TOPIC_SERVO1) { //Condicional referente ao Servo Motor 1;
  int angulo = 90; //Definição do ângulo solicitado;
  servo1.write(angulo);//Movimentação;
  mqtt.publish(TOPIC__SERVO_1, String(angulo).c_str()); //Publicação do estado do Servo Motor 1;
  return;
} else if (topicStr == TOPIC_SERVO2) { //Condicional referente ao Servo Motor 2;
  int angulo = 90; //Definição do ângulo solicitado;
  servo2.write(angulo); //Movimentação;
  mqtt.publish(TOPIC_SERVO_2, String(angulo).c_str()); //Publicação do estado do Servo Motor 2;
  return;
}

  }
void loop() {

  // put your main code here, to run repeatedly:
  //String mensagem = "";                       //Dentro do Loop, para recriá-la
 // if (Serial.available() > 0) {               //Caracteres disponíveis na "fila"
   // mensagem = Serial.readStringUntil('\n');  //Leitura até "encontrar" o \n, palavra salva dentro da variável "mensagem"
    //mensagem = "Beatriz: " + mensagem;       //exibição ao leitor
   // mqtt.publish("Miguel",mensagem.c_str()); //enviar para o broker
 // }
  long distancia = lerDistancia(TRIGGER_PIN, ECHO_PIN) //Definição conforme os Pinos;
  if (distancia < 10){
       mqtt.publish(TOPIC_ULTRASSONICO, "Presente"); //enviar para o broker
  }
  mqtt.loop(); //Loop;
}

long lerDistancia() { //Função de leitura de distância;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  long duracao = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  
  return distancia; //Retorno da distância, conforme calculado;
}

void connecttoBroker(){
  Serial.println("Conectando ao broker...");
}
