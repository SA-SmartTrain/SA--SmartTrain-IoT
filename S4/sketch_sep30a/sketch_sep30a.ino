#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifi_client;
PubSubClient mqtt(wifi_client);

const String SSID = "FIESC_IOT_EDU";  
const String PASS = "8120gv08";       

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;
const String topic = "Miguel";

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);  
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!");

  mqtt.setServer(brokerURL.c_str(), brokerPort);
  String clientID = "S4_MiguelRocha";
  clientID += String(random(0xffff), HEX);
  while (mqtt.connect(clientID.c_str()) == 0) {
    Serial.print(".");
    delay(2000);
  }
  mqtt.subscribe(topic.c_str());
  mqtt.setCallback(callback);
  Serial.println("\nConectado ao broker!");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Código para enviar para o servidor
  String mensagem = "";                      
  if (Serial.available() > 0) {               
    mensagem = Serial.readStringUntil('\n');  
    mensagem = "Miguel: " + mensagem;       
    mqtt.publish("beatrizcercal",mensagem.c_str());
  }
  mqtt.loop();
}


void callback(char* topic, byte* payload, unsigned long length) {
  String mensagem_recebida = "";
  for(int i=0; i< length; i++){
    mensagem_recebida += (char) payload[i]; // Agrupa cada caracter recebida func payload[variavel]
  }
  Serial.println(mensagem_recebida); // Colocar o LED Pino2
}

void connecttoBroker(){
  Serial.println("Conectando ao broker...");
}

