/*********
  Jackson Wendel Santos Sá
  Projeto em github.com/wendelmax/ESPColorReceiver  
*********/

#include <Arduino.h>

// --- Board Detection & Dependencies ---
#if defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE)
  #include <M5Stack.h>
  #define HAS_DISPLAY_M5
#elif defined(ESP32)
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
  #include <analogWrite.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncWebServer.h>
  #include <ESPAsyncTCP.h>
#endif

// --- Default Pin Mapping ---
#if defined(HAS_DISPLAY_M5)
  const int pinoR = 2, pinoG = 5, pinoB = 13, power = 12; // Example M5Stack pins
#else
  const int pinoR = 13, pinoG = 12, pinoB = 14, power = 17; // Generic defaults
#endif

// --- Server & State ---
AsyncWebServer server(80);
int rVal = 0, gVal = 0, bVal = 0;
bool powerState = false;
unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_INTERVAL = 1000;
const char* ssid = "ESPColorReceiver";
String apPassword;

// --- Display Abstraction ---
void initBoard() {
#if defined(HAS_DISPLAY_M5)
  M5.begin();
  M5.Lcd.setTextSize(2);
#else
  Serial.begin(115200);
#endif
}

void updateDisplay() {
  String ip = WiFi.softAPIP().toString();
  String clients = String(WiFi.softAPgetStationNum());

#if defined(HAS_DISPLAY_M5)
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.printf("SSID: %s\nPass: %s\nIP: %s\nClients: %s", ssid, apPassword.c_str(), ip.c_str(), clients.c_str());
#else
  static String lastIp;
  if (lastIp != ip) {
    Serial.println("AP IP: " + ip);
    lastIp = ip;
  }
#endif
}

void setup() {
  initBoard();
  
  randomSeed(analogRead(0));
  apPassword = String(random(10000000, 99999999));

  pinMode(pinoR, OUTPUT);
  pinMode(pinoG, OUTPUT);
  pinMode(pinoB, OUTPUT);
  pinMode(power, OUTPUT);

  WiFi.softAP(ssid, apPassword.c_str(), 6, false, 8);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    bool changed = false;
    
    // Suporte para r e R
    if (request->hasParam("r")) { rVal = request->getParam("r")->value().toInt(); changed = true; }
    else if (request->hasParam("R")) { rVal = request->getParam("R")->value().toInt(); changed = true; }
    
    // Suporte para g e G
    if (request->hasParam("g")) { gVal = request->getParam("g")->value().toInt(); changed = true; }
    else if (request->hasParam("G")) { gVal = request->getParam("G")->value().toInt(); changed = true; }
    
    // Suporte para b e B
    if (request->hasParam("b")) { bVal = request->getParam("b")->value().toInt(); changed = true; }
    else if (request->hasParam("B")) { bVal = request->getParam("B")->value().toInt(); changed = true; }
    
    powerState = changed;
    request->send(200, "text/plain", "OK");
  });


  server.begin();
  updateDisplay();
}

void loop() {
#if defined(HAS_DISPLAY_M5)
  M5.update();
#endif

  if (millis() - lastDisplayUpdate >= DISPLAY_INTERVAL) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }

  int outR = powerState ? rVal : 0;
  int outG = powerState ? gVal : 0;
  int outB = powerState ? bVal : 0;

  digitalWrite(power, powerState ? HIGH : LOW);
  
#if defined(ESP8266)
  analogWrite(pinoR, outR);
  analogWrite(pinoG, outG);
  analogWrite(pinoB, outB);
#else
  ::analogWrite(pinoR, outR);
  ::analogWrite(pinoG, outG);
  ::analogWrite(pinoB, outB);
#endif
}


