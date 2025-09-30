#include <WiFi.h>
WiFiClient client; // Carinha que conecta com o Wi-fi
const String SSID = "nome da rede";
const String PASS = "senha da rede";

void setup() {
  // Função para verificar se o Wi-fi foi conectado com sucesso
  Serial.begin(115200);
  client.begin(SSID, PASS);
  Serial.println("Conectando no Wi-fi");
  while(client.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(200);
  }
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

}
