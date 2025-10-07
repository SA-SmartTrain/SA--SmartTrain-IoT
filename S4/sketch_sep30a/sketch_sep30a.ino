#include <WiFi.h> // WiFi library for ESP32 DEVKIT V1
#include <PubSubClient.h> // Client library for MQTT messasing

WiFiClient wifi_client;
PubSubClient mqtt(wifi_client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

// Declaração Broker Fixa
const String brokerURL = "test.mosquitto.org"; // Pública agora | SA diferente;
const int brokerPort = 1883; // Pública agora | SA diferente;

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  // put your setup code here, to run once:
  // Função para verificar se o Wi-fi foi conectado com sucesso
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  Serial.println("Conectando no Wi-fi");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!");
  // Conexão com o broker
  mqtt.setServer(brokerURL.c_str(),brokerPort);
  String clientID = "S4-";
  clientID += String(random(0xffff),HEX);
  Serial.println("Conectando ao Broker");
  while(mqtt.connect(clientID.c_str()) == 0) {
    Serial.println(".");
    delay(200);
  }
  Serial.println("\nConectado com sucesso, broker!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
