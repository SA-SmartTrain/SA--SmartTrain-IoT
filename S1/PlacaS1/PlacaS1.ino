#include <WiFi.h>
#include <PubSubClient.h>


WiFiClient wifi_client;
PubSubClient mqtt(wifi_client);


const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";


const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;
const String topic = "Camafeu";
const byte pino_led = 2;


const String brokerUser = "" ;
const String brokerPass = "" ;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  Serial.println("Conectando no WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!");
  mqtt.setServer(brokerURL.c_str(),brokerPort);
  String clientID = "S1";
  clientID += String(random(0xffff),HEX);
  while (mqtt.connect(clientID.c_str()) == 0){
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(topic.c_str());
  mqtt.setCallback(callback);
  Serial.println("\nConectado ao Broker!");

  pinMode(pino_led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  String mensagem = " ";
  if(Serial.available() > 0) {
    mensagem = Serial.readStringUntil('\n');
    mensagem = "Beatriz: " + mensagem;
    mqtt.publish("S2",mensagem.c_str()); 
  }
  mqtt.loop();

}
void callback(char* topic, byte* payload, unsigned long length){
  String MensagemRecebida = "";
  if(pino_led == HIGH){
    digitalWrite (pino_led,LOW);
    delay(200);
    digitalWrite(pino_led,HIGH);
    MensagemRecebida += (char) payload[pino_led]; //Pega cada letra de payload e junta na mensagem

  }
  Serial.println(MensagemRecebida);
}

void connectToBroker(){
  Serial.println("Conectado ao brooker!");
}
