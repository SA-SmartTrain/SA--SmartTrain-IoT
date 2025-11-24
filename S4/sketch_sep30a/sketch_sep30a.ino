// #define TOPIC_TREMVEL "Trem/Vel" - COLOCAR NO ENV.H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "env.h"   // WIFI_SSID, WIFI_PASS, BROKER_URL, BROKER_PORT, TOPIC_TREMVEL

WiFiClientSecure wifiClient;
PubSubClient mqtt(wifiClient);

// --- PINOS ---
const int IN1 = 25;  // Direção 1 (ponte H)
const int IN2 = 26;  // Direção 2 (ponte H)
const int ENA = 27;  // PWM velocidade

const int LED_VERDE = 21;
const int LED_VERMELHO = 19;

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  
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
