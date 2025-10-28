#include <WiFi.h>
#include <PubSubClient.h>
#include <WifiClientSecure.h>
#include "env.h"

WiFiClientSecure wifi_client;
PubSubClient mqtt(wifi_client);

const byte pinLED = 2; //
const int IN1 = 25;    // Direção 1
const int IN2 = 26;    // Direção 2
const int ENA = 27;    // Enable (PWM)

void setup()
{
  Serial.begin(115200);
  wifi_client.setInsecure(); // Ignora o certificado de notificação do Broker com a placa
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!");

  mqtt.setServer(BROKER_URL.c_str(), BROKER_PORT);
  String clientID = "S4_MiguelRocha";
  clientID += String(random(0xffff), HEX);
  while (mqtt.connect(clientID.c_str()) == 0)
  {
    Serial.print(".");
    delay(2000);
  }
  mqtt.subscribe(TOPIC_PRESENCE1.c_str());
  mqtt.setCallback(callback);
  Serial.println("\nConectado ao broker!");

  // Inscrevendo os tópicos presence MQTT
  mqtt.subscribe(TOPIC_PONTEH); // Ponte H
}

void loop()
{
  mqtt.loop();

  // Ler dados da Serial e enviar
  if (Serial.available() > 0)
  {
    String mensagem = Serial.readStringUntil('\n');
    mensagem.trim(); // Remove quebras de linha extras

    // Lógica para ligar/desligar LED
    if (mensagem == "1")
    {
      digitalWrite(pinLED, HIGH);
      Serial.println("LED LIGADO (comando serial)");
      mqtt.publish(TOPIC_PRESENCE11, "LED ON");
    }
    else if (mensagem == "0")
    {
      digitalWrite(pinLED, LOW);
      Serial.println("LED DESLIGADO (comando serial)");
      mqtt.publish(TOPIC_PRESENCE11, "LED OFF");
    }
    else if (mensagem == "FRENTE")
    {
      ponteH_frente();
      mqtt.publish(TOPIC_PONTEH, "FRENTE");
    }
    else if (mensagem == "RE")
    {
      ponteH_re();
      mqtt.publish(TOPIC_PONTEH, "RE");
    }
    else if (mensagem == "PARAR")
    {
      ponteH_parar();
      mqtt.publish(TOPIC_PONTEH, "PARAR");
    }
    else
    {
      String texto = "Miguel: " + mensagem;
      mqtt.publish("beatrizcercal", texto.c_str());
      Serial.print("Mensagem enviada: ");
      Serial.println(texto);
    }
  }
}

void callback(char* topic, byte* payload, unsigned long length) {
  String mensagem_recebida = "";
  for (int i = 0; i < length; i++) {
    mensagem_recebida += (char)payload[i];
  }

  Serial.print("Mensagem MQTT recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensagem_recebida);

  if (String(topic) == TOPIC_PONTEH) {
    if (mensagem_recebida == "FRENTE") ponteH_frente();
    else if (mensagem_recebida == "RE") ponteH_re();
    else if (mensagem_recebida == "PARAR") ponteH_parar();
  }
  else if (String(topic) == TOPIC_PRESENCE11) {
    if (mensagem_recebida == "LED ON") digitalWrite(pinLED, HIGH);
    else if (mensagem_recebida == "LED OFF") digitalWrite(pinLED, LOW);
  }
}

// --- Funções da Ponte H ---
void ponteH_frente(int velocidade = 255)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, velocidade);
  Serial.println("Trem seguindo para frente");
}

void ponteH_re(int velocidade = 200)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, velocidade);
  Serial.println("Trem voltando");
}

void ponteH_parar()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  Serial.println("Trem parado");
}

void connecttoBroker()
{
  Serial.println("Conectando ao broker...");
}
