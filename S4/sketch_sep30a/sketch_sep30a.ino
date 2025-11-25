// #define TOPIC_TREMVEL "Trem/Vel" - COLOCAR NO ENV.H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "env.h" // Certifique-se de que este arquivo existe e tem as definições!

const int LED_VERMELHO = 18;   // Indica RÉ (GPIO 19)
const int LED_VERDE = 21;      // Indica FRENTE (GPIO 21)


void led_frente();
void led_re();
void led_parar();
void callback(char* topic, byte* payload, unsigned long length);
void reconnect_mqtt(); // Função para tentar reconectar ao MQTT

WiFiClientSecure wifiClient;
PubSubClient mqtt(wifiClient);

const int IN1 = 25;  // Direção 1 (ponte H)
const int IN2 = 26;  // Direção 2 (ponte H)
const int ENA = 27;  // PWM velocidade
>>>>>>> 231ae3278ae1866214ca33c62ed1648f2a13b826

const int LED_VERDE = 21;
const int LED_VERMELHO = 19;

void setup() {
  Serial.begin(115200);
<<<<<<< HEAD
  delay(1000);

  // O uso de setInsecure() desativa a verificação do certificado TLS/SSL.
  // Use apenas para fins de teste.
  wifi_client.setInsecure();

  // Pinos de Saída
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  // Inicializa todos os LEDs desligados
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);

  // Conexão WiFi
  Serial.println("Conectando no WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  // Tenta conectar, mostrando progresso
  int tentativas_wifi = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    tentativas_wifi++;
    if (tentativas_wifi > 20) { // Timeout de 10 segundos (20 * 500ms)
        Serial.println("\nFalha ao conectar no WiFi. Verifique o SSID/PASS.");
        // Não trava aqui, mas continua tentando no loop
        break; 
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nWiFi conectado! IP: ");
    Serial.println(WiFi.localIP());

    // Configuração MQTT
    mqtt.setServer(BROKER_URL, BROKER_PORT);
    mqtt.setCallback(callback);

    reconnect_mqtt(); // Tenta a conexão inicial com o Broker
  } else {
    Serial.println("WiFi não conectado. MQTT não inicializado.");
  }
}

void loop()
{
  // Se não estiver conectado, tenta reconectar
  if (!mqtt.connected()) {
    reconnect_mqtt();
  }
  
  // Processa a fila de mensagens do MQTT (manutenção da conexão)
  mqtt.loop();

  // Processamento de comandos pela Serial (para testes)
  if (Serial.available())
  {
    String msg = Serial.readStringUntil('\n');
    msg.trim(); // Remove espaços em branco no início e fim

    Serial.print("Comando Serial Recebido: ");
    Serial.println(msg);

    // Converte para maiúsculas para garantir a comparação
    msg.toUpperCase(); 

    if (msg == "FRENTE") led_frente();
    else if (msg == "RE" || msg == "RÉ") led_re();
    else if (msg == "PARAR") led_parar();
    else Serial.println("Comando Serial inválido.");
  }
}

// Função chamada quando uma mensagem MQTT é recebida
void callback(char* topic, byte* payload, unsigned long length)
{
  String mensagem = "";
  for (int i = 0; i < length; i++)
    mensagem += (char)payload[i];

  mensagem.trim(); // Limpa espaços
  mensagem.toUpperCase(); // Padroniza para maiúsculas

  Serial.print("MQTT Recebido no Tópico [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensagem);

  // Lógica de controle baseada na mensagem
  if (mensagem == "FRENTE") led_frente();
  else if (mensagem == "RE") led_re();
  else if (mensagem == "PARAR") led_parar();
  else Serial.println("Comando MQTT inválido.");
}

// Função para tentar reconectar ao MQTT
void reconnect_mqtt() {
  // Verifica o WiFi antes de tentar o MQTT
  if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi desconectado. Não foi possível reconectar ao MQTT.");
      return;
  }
  
  Serial.print("Conectando ao broker...");
  // Loop para tentar reconectar
  while (!mqtt.connected()) {
    
    // Tenta conectar usando o ID, User e Senha
    if (mqtt.connect("SmartTrain_S4", BROKER_USR_ID, BROKER_USR_PASS)) {
      Serial.println("\nConectado ao MQTT!");
      
      // Assina o tópico
      mqtt.subscribe(TOPIC_PONTEH);
      Serial.print("Assinado o tópico: ");
      Serial.println(TOPIC_PONTEH);
      
    } else {
      Serial.print(".");
      delay(5000); 
    }
  }
}


void led_frente()
{
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, LOW);
  Serial.println("LED -> FRENTE (verde)");
}

void led_re()
{
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, HIGH);
  Serial.println("LED -> RÉ (vermelho)");
}

void led_parar()
{
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  Serial.println("LED -> PARADO (apagado)");
}

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  // LED inicial
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, HIGH);

  // WiFi
  wifiClient.setInsecure();
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }
  Serial.println("\nWiFi conectado!");

  // MQTT
  mqtt.setServer(BROKER_URL.c_str(), BROKER_PORT);
  mqtt.setCallback(callback);

  Serial.println("Conectando ao broker...");
  while (!mqtt.connect("ESP32_TremVel")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConectado ao MQTT!");

  mqtt.subscribe(TOPIC_TREMVEL.c_str());
}

void loop() {
  mqtt.loop();
}

void callback(char *topic, byte *payload, unsigned long length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.print("Recebido em ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  if (msg == "PARAR") {
    pararTrem();
  }
  else if (msg == "FRENTE") {
    moverFrente(255);
  }
  else if (msg.startsWith("FRENTE:")) {
    int vel = msg.substring(7).toInt();
    moverFrente(vel);
  }
  else if (msg.startsWith("RE:")) {
    int vel = msg.substring(3).toInt();
    moverRe(vel);
  }
  else if (msg == "RE") {
    moverRe(200);
  }
}

void moverFrente(int velocidade) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, velocidade);

  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, HIGH);

  Serial.printf("Trem indo pra frente | Vel: %d\n", velocidade);
}

void moverRe(int velocidade) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, velocidade);

  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, HIGH);

  Serial.printf("Trem ré | Vel: %d\n", velocidade);
}

void pararTrem() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, HIGH);

  Serial.println("Trem parado!");
}
