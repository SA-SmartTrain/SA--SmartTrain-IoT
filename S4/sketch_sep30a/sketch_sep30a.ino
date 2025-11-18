#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "env.h" // Certifique-se de que este arquivo existe e tem as definições!

// =========================
// Definição dos Pinos
// =========================
const int LED_VERMELHO = 18;   // Indica RÉ (GPIO 19)
const int LED_VERDE = 21;      // Indica FRENTE (GPIO 21)


void led_frente();
void led_re();
void led_parar();
void callback(char* topic, byte* payload, unsigned long length);
void reconnect_mqtt(); // Função para tentar reconectar ao MQTT


WiFiClientSecure wifi_client;
PubSubClient mqtt(wifi_client);


void setup()
{
  Serial.begin(115200);
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