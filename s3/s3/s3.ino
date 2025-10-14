#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifi_client;
PubSubClient mqtt(wifi_client);

const String SSID = "FIESC_IOT_EDU";  //nome da rede wifi
const String PASS = "8120gv08";       //senha da rede

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;
const String topic = "beatrizcercal";

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);  //tenta conectar na rede
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!");

  mqtt.setServer(brokerURL.c_str(), brokerPort);
  String clientID = "S3_beatrizcercal";
  clientID += String(random(0xffff), HEX);
  while (mqtt.connect(clientID.c_str()) == 0) {
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(topic.c_str());
  mqtt.setCallback(callback);
  Serial.println("\nConectado ao broker!");
}
void callback(char* topic, byte* payload, unsigned long lenght) {
  String MensagemRecebida = "";
  for (int i = 0; i < lenght; i++){
    MensagemRecebida += (char) payload[i]; //Cada letra de payload e junta na mensagem

  }
  Serial.println(MensagemRecebida);
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