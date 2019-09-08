/*********
  Jackson Wendel Santos Sá
  Projeto em github.com/wendelmax/ESPColorReceiver  
*********/

// Load Wi-Fi library
#include <heltec.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <analogWrite.h>
String parmValor[3];
bool chave = false;
//Gerador de senhas
String randomNumber = String(random(0,999999999),HEX);
const char* password = randomNumber.c_str();
//const char* password = "123456789";
//nome da rede wi-fi
const char* ssid = "ESP32ColorReceiver";
// Defindo os pinos para RGB
const int pinoR = 13;
const int pinoG = 12;
const int pinoB = 14;
const int power = 17;
// servidor web na porta 80
AsyncWebServer server(80);
//Variável para receber ip
IPAddress IP;
//método para exibir IP correto no display
String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

void tela() {
  Heltec.display->clear();
  Heltec.display -> drawString(0, 3, "SSID: "+String(ssid));
  Heltec.display -> drawString(0, 15, "Senha: " + String(password));
  Heltec.display -> drawString(0, 27, "IP: "+String(ipToString(IP)));
  Heltec.display -> drawString(0, 39, "MAC: "+String(WiFi.softAPmacAddress()));
  Heltec.display -> drawString(0, 51, "Clientes: " +String(WiFi.softAPgetStationNum()));
  Heltec.display -> display();
}

void setup() {
  Serial.begin(115200);
  //configuração inicial da placa
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
//Configurando pinagem
  pinMode(pinoR,OUTPUT);
  pinMode(pinoG,OUTPUT);
  pinMode(pinoB,OUTPUT);
  pinMode(power,OUTPUT);
  // Criando rede wi-fi
  WiFi.softAP(ssid, password, 6, false, 8);
  //WiFi.softAP(ssid);
  delay(300);
  //carregando ip na variavel
  IP = WiFi.softAPIP();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){

    int paramsNr = request->params();
    Serial.println(paramsNr);
 
    for(int i=0;i<paramsNr;i++){
         
        AsyncWebParameter* p = request->getParam(i);
        Serial.print("Param name: ");
        Serial.println(i);
        Serial.print("Param name: ");
        Serial.println(p->name());
        Serial.print("Param value: ");
        Serial.println(p->value());
        Serial.println("------");
        chave = true;
    } 
  request->send(200, "text/plain", "Recebido com Sucesso");
  });
  tela(); 
  server.begin();
}

void loop(){
  tela();
    if (chave)
    {
    digitalWrite(power,HIGH);
    digitalWrite(pinoR,parmValor[0].toInt());
    digitalWrite(pinoG,parmValor[1].toInt());
    digitalWrite(pinoB,parmValor[2].toInt());
    }
    else{
    digitalWrite(power,LOW);
    }
}