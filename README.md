# ESP32ColorReceiver

Firmware universal para controle de fitas LED RGB via Wi-Fi (Web Server) compatível com **ESP32**, **ESP8266** e **M5Stack**.

## 🚀 Funcionalidades

- **Controle PWM**: Ajuste de intensidade de 0-255 para os canais R, G e B.
- **Auto-Configuração**: Detecta automaticamente se está rodando em M5Stack ou ESP genérico.
- **Ponto de Acesso (AP)**: Cria uma rede Wi-Fi própria com senha aleatória gerada no boot.
- **Display Informativo**: Exibe SSID, Senha e IP no LCD do M5Stack ou no monitor Serial.
- **Web Server Assíncrono**: Respostas rápidas e sem bloqueio do loop principal.

## 🛰️ Protocolo de Comunicação & Compatibilidade

O dispositivo aceita parâmetros via GET em maiúsculo ou minúsculo. Isso garante compatibilidade total com o [ESP32ColorSender](https://github.com/wendelmax/ESP32ColorSender):

```http
GET /?R=255&G=0&B=128
```
ou
```http
GET /?r=255&g=0&b=128
```

## 📋 Requisitos

- [PlatformIO](https://platformio.org/)
- Bibliotecas:
  - `ESP Async WebServer`
  - `ESP32 AnalogWrite`
  - `M5Stack` (apenas para dispositivos M5)

## 🔧 Configuração

O arquivo `platformio.ini` já vem pré-configurado com os seguintes ambientes:

- `esp32dev`: Para ESP32 genérico.
- `esp8266`: Para NodeMCU/D1 Mini.
- `m5stack-core-esp32`: Para dispositivos M5Stack.

## 📄 Licença

Este projeto está sob a licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.

---
Desenvolvido por **Jackson Wendel Santos Sá**
