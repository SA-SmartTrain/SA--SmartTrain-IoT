#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

WiFiClientSecure wifi_client; // conecta a internet 
PubSubClient mqtt(wifi_client);

const int trig_1 = 12; // declaração do pino do trig
const int echo_1 = 13; // declaração do pino do echo

const int trig_2 = 14; // declaração do pino do trig
const int echo_2 = 15; // declaração do pino do echo

const byte LED_PIN = 18; //declaração do LED

const byte led_r = 25; // declaração dos pinos do LED RGB
const byte led_g = 26;
const byte led_b = 27;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // inicia a comunicação na velocidade de 115200 e permite que envie a msg para o computador e visualize no monitor
  wifi_client.setInsecure();
  pinMode(LED_PIN, OUTPUT);  // define o pino do LED como saída        
  digitalWrite(LED_PIN, LOW); // garante que o LED comece apagado
  WiFi.begin(WIFI_SSID, WIFI_PASS); // inicia a conexão com o wifi usando o nome e senha e imprime a mensagem de conectado
  Serial.println("Conectando no WiFi");
  while(WiFi.status() != WL_CONNECTED){ //O código fica verificando até que o Wi-Fi conecte, imprimindo pontos no serial enquanto espera.
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!"); // Ele cria um ID único para o dispositivo e tenta conectar ao servidor MQTT até conseguir.
  mqtt.setServer(BROKER_URL,BROKER_PORT);
  String clientID = "S2"; 
  clientID += String(random(0xffff),HEX);
  while (mqtt.connect(clientID.c_str(), BROKERUSR_ID, BROKER_PASS_USR_PASS) == 0){
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(TOPIC_LUMINOSIDADE1); // o dispositivo se increve no tópico luminosidade da placa S1 indicando que recebe mensagens enviadas para esse tópico.
  mqtt.setCallback(callback); //Define qual função será chamada sempre que chegar uma mensagem no tópico assinado.
  Serial.println("\nConectado ao Broker!"); //Apenas imprime no monitor serial que a conexão com o broker foi concluída com sucesso.
}

void loop() {
  long dist_1 = lerDistancia(trig_1, echo_1);  //O código mede a distância e, se for menor que 10 cm, envia a mensagem “Presente” para o tópico MQTT.
    if (dist_1 < 10){
     mqtt.publish(TOPIC_ULTRASSONICO1, "Presente");
  }
 

  long dist_2 = lerDistancia(trig_2, echo_2);  // Ele mede a distância, envia “Presente” se for menor que 10 cm e mantém o MQTT funcionando.
    if (dist_2 < 10){
     mqtt.publish(TOPIC_ULTRASSONICO2, "Presente");
  }
  Serial.println(dist_1, dist_2);
  mqtt.loop();
}

long lerDistancia(byte trig_pin, byte echo_pin) { //A função envia um pulso pelo sensor ultrassônico, mede o tempo de retorno e calcula a distância em centímetros.
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  long duracao = pulseIn(echo_pin, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  
  return distancia;
}


void callback(char* topic, byte* payload, unsigned long length) {  //A função recebe a mensagem do MQTT, converte o payload em texto e imprime no monitor serial.
  String MensagemRecebida = "";
  for(int i = 0; i < length; i++){
    MensagemRecebida += (char) payload[i]; 
  }
  Serial.println(MensagemRecebida);

  if( topic == TOPIC_LUMINOSIDADE1){ //O código verifica se a mensagem recebida é do tópico certo e acende ou apaga o LED conforme o comando.
    if(MensagemRecebida == "Acender"){
      digitalWrite(LED_PIN, HIGH);
    } else if(MensagemRecebida == "Apagar"){
      digitalWrite(LED_PIN, LOW);
  }
}
}
void connectToBrooker(){
  Serial.println("Conectado ao Brooker..."); //A função apenas exibe no serial que o dispositivo está conectado ao broker.
}




