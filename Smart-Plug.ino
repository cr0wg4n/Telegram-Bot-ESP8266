#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 13
DHT dht(dht_dpin, DHTTYPE); 
float t,h;
// Inicializamos la conexion WIFI con el Router
char ssid[] = "xxxxxxxx";     // nombre de tu RED WIFI
char password[] = "xxxxxxx"; // clave de tu RED WIFI
 
// Inicializar Telegram BOT
#define BOTtoken "xxxxxxx"  // la clave de tu BOT 
// si no la tienes revisa el siguiente enlace:
//http://hackcodemy.com/2018/07/29/crear-un-bot-de-telegram/
 
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

String chat_id2;
void handleNewMessages(int numNewMessages) {
  Serial.println("--> Nuevos mensajes");
  Serial.println("    +"+String(numNewMessages));
 
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    chat_id2 = chat_id;
    String text = bot.messages[i].text;
 
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
 
    if (text == "/start") {
      String welcome = "Hola, " + from_name + ".\n";
      welcome += "Introduce /opciones si deseas ver lo que puedo hacer.\n\n";
      bot.sendMessage(chat_id, welcome);
    }
 
    if (text == "/opciones") {
      String keyboardJson = "[[\"/Plug_1_On\", \"/Plug_1_Off\"],[\"/Plug_2_On\", \"/Plug_2_Off\"],[\"/Temperatura_en_Casa\"]]";    //,[\"/sensor\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Selecciona una de las siguientes opciones:", "", keyboardJson, true);
    }
 
    if (text == "/Plug_1_On") {
      digitalWrite(4, LOW);
      bot.sendMessage(chat_id, "Plug 1 Encendido!");
      Serial.println("listo 1 encedido");
    }
    if (text == "/Plug_1_Off") {
      digitalWrite(4, HIGH);
      bot.sendMessage(chat_id, "Plug 1 Apagado!");
      Serial.println("listo 1 apagado");
    }
    if (text == "/Plug_2_On") {
      digitalWrite(14, LOW);
      bot.sendMessage(chat_id, "Plug 2 Encendido!");
      Serial.println("listo 2 encedido");
    } 
    if (text == "/Plug_2_Off") {
      digitalWrite(14, HIGH);
      bot.sendMessage(chat_id, "Plug 2 Apagado!");
      Serial.println("listo 2 apagado");
    }
    if (text == "/Temperatura_en_Casa"){
    bot.sendMessage(chat_id, "Temp: "+String(t)+"°C  Hum: "+String(h)+"%");
    Serial.println("Temp: "+String(t)+"°C  Hum: "+String(h));
    }
  }
}
 
 
void setup() {
  Serial.begin(115200);
  dht.begin();
  // Modo WIFI y desconexion con AP anterior
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
 
  // Conexion a la RED
  Serial.print("Conectando al Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println("");
  Serial.println("WIFI conectado");
  pinMode(4, OUTPUT);
  pinMode(14, OUTPUT);
}
 
void loop() {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("recibiste Respuesta");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    h = dht.readHumidity();
    t = dht.readTemperature();         
}

