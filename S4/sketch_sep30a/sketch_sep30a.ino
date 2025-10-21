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

const byte pinLED = 2; // 

void setup() {
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);
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
  void loop() {
  mqtt.loop();

  // Ler dados da Serial e enviar
  if (Serial.available() > 0) {               
    String mensagem = Serial.readStringUntil('\n');  
    mensagem.trim(); // Remove quebras de linha extras
    // Lógica para ligar/desligar o LED da dupla enviando uma mensagem em números true/false
    if (mensagem == "1") {
      digitalWrite(pinLED, HIGH);
      Serial.println("LED LIGADO (comando serial)");
      mqtt.publish(topic.c_str(), "LED ON");
    } 
    else if (mensagem == "0") { 
      digitalWrite(pinLED, LOW);
      Serial.println("LED DESLIGADO (comando serial)");
      mqtt.publish(topic.c_str(), "LED OFF");
    } 
    else {
      String texto = "Miguel: " + mensagem;
      mqtt.publish("beatrizcercal", texto.c_str());
      Serial.print("Mensagem enviada: ");
      Serial.println(texto);
    }
  }
}
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


