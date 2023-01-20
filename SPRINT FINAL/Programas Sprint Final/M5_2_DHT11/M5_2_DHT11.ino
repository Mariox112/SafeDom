#include <DHT.h>
#include <ArduinoMqttClient.h>
#include <M5Stack.h>
#include <WiFi.h>

#define SafeDom 0xF249    // COLOR SAFEDOM
 
#define DHTPIN 5
#define DHTTYPE DHT11

// DEEP SLEEP
#define deepSleepConvTime 1000000
#define sleepTime 15

DHT dht(DHTPIN, DHTTYPE);
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// ----------------------------------------
// -------------- VARIABLES ---------------
// ----------------------------------------

// ----- WIFI -----
char ssid[] = "soy muy sabroso";
char pass[] = "soytuputito";

// ----- BROKER -----
const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic1[] = "safedom/DHT11/Temperatura/";
const char topic2[] = "safedom/DHT11/Humedad/";

// ****************************************
// **************** SETUP *****************
// ****************************************
void setup() {
  M5.begin(true, false, true, false);
  intro();
  Serial.println();
  Serial.println("M5Stack: Despertando...");
  dht.begin();
  conexionWiFi();
  conexionMQTT();
  esp_sleep_enable_timer_wakeup(sleepTime*deepSleepConvTime);
  ending();
}

// ****************************************
// **************** LOOP ******************
// ****************************************
void loop() {
  context();
  // READ TEMPERATURE AND HUMIDITY
  float h = dht.readHumidity();
  float t = dht.readTemperature();
 
  // CHECK FOR READ ERRORS IN VARIABLES
  if (isnan(t)||isnan(h)) {
    Serial.println("DHT: Error obteniendo los datos del sensor DHT11");
    M5.Lcd.setCursor(30,130);
    M5.Lcd.print("Error");
    delay(5000);
    endContext();
    esp_deep_sleep_start();
  }

  mqttSend(t,h);
  printValuesLCD(t,h);
  endContext();
  esp_deep_sleep_start();
}

// ----------------------------------------
// ---------------- INTRO -----------------
// ----------------------------------------
void intro(){
  delay(1000);
  M5.Lcd.fillScreen(SafeDom);
  M5.Lcd.setTextSize(6);
  M5.Lcd.setTextColor(WHITE, SafeDom);
  M5.Lcd.setCursor(30, 40);
  M5.Lcd.print("SafeDom");
}

// ----------------------------------------
// --------------- ENDING -----------------
// ----------------------------------------
void ending(){
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(30,180);
  M5.Lcd.print("Despertando");
  for(int i=0; i<3; i++){
    delay(1000);
    M5.Lcd.print(".");
  }
}

// ----------------------------------------
// -------------- CONTEXT -----------------
// ----------------------------------------
void context(){
  M5.Lcd.clear();
  M5.Lcd.fillScreen(SafeDom);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE, SafeDom);
  M5.Lcd.setCursor(30, 40);
  M5.Lcd.println("Tomando");
  M5.Lcd.setCursor(30, 70);
  M5.Lcd.print("medidas");
  for(int i=0; i<3; i++){
    delay(1000);
    M5.Lcd.print(".");
  }
}

// ----------------------------------------
// ------------ END CONTEXT ---------------
// ----------------------------------------
void endContext(){
  M5.Lcd.clear();
  M5.Lcd.fillScreen(SafeDom);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(30,40);
  M5.Lcd.print("Durmiendo");
  for(int i=0; i<3; i++){
    delay(1000);
    M5.Lcd.print(".");
  }
  delay(1000);
}

// ----------------------------------------
// ------------- MQTT SEND ----------------
// ----------------------------------------
void mqttSend(float temperature, float humidity){
  mqttClient.beginMessage(topic1);
  mqttClient.print(temperature);
  mqttClient.endMessage();
  mqttClient.beginMessage(topic2);
  mqttClient.print(humidity);
  mqttClient.endMessage();
}

// ----------------------------------------
// ---------- PRINT VALUES LCD ------------
// ----------------------------------------
void printValuesLCD(float temperature, float humidity){
  M5.Lcd.setCursor(30,130);
  M5.Lcd.print("T: ");
  M5.Lcd.print(temperature);
  M5.Lcd.println(" C");
  M5.Lcd.setCursor(30,160);
  M5.Lcd.print("H: ");
  M5.Lcd.print(humidity);
  M5.Lcd.println(" %");
  delay(5000);
}

// ----------------------------------------
// ------------ CONEXION WIFI -------------
// ----------------------------------------
void conexionWiFi(){
  Serial.println();
  Serial.print("WIFI: Conectando a la red: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("WIFI: CONECTADO");
  Serial.println();
}

// ----------------------------------------
// ------------ CONEXION MQTT -------------
// ----------------------------------------
void conexionMQTT(){
  mqttClient.setId("821amqwkhy443");
  Serial.print("MQTT: Conectando al broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT: ERROR EN EL BROKER: Error code = ");
    Serial.print(mqttClient.connectError());
    while (1);
  }
  Serial.println("MQTT: CONECTADO");
  Serial.println();
}
