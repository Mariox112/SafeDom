#include <HX711.h>
#include <ArduinoMqttClient.h>
#include <M5Stack.h>
#include <WiFi.h>

#define SafeDom 0xF249    // COLOR SAFEDOM

#define DOUT 5
#define CLK 2

HX711 balanza;
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
const char topic1[] = "safedom/HX711/actividad/";
const char topic2[] = "safedom/HX711/tiempoActivo/";

// ----- GENERALES -----
bool contadorWeight=false;
bool contadorWeight2=true;
int timerActivo = 0;

// ****************************************
// **************** SETUP *****************
// ****************************************
void setup() {
  M5.begin(true, false, true, false);
  intro();
  conexionWiFi();
  conexionMQTT();
  medium();
  calibracionHX711();
  ending();
  context();  
}

// ****************************************
// **************** LOOP ******************
// ****************************************
void loop() {
  float weight = balanza.get_units(20);
  Serial.println(weight);
  if(weight>=1){
    timerActivo += 2;
    if(contadorWeight){
      contadorWeight=false;
      contadorWeight2=true;
      mqttClient.beginMessage(topic1);
      mqttClient.print("activo");
      mqttClient.endMessage();
    }
  }
  if(weight<1){
    if(contadorWeight2){
      contadorWeight=true;
      contadorWeight2=false;
      mqttClient.beginMessage(topic1);
      mqttClient.print("inactivo");
      mqttClient.endMessage();
      mqttClient.beginMessage(topic2);
      mqttClient.print(timerActivo);
      mqttClient.endMessage();
      timerActivo = 0;
    }
  }
  mostrarLCD(weight);
}

// ----------------------------------------
// ---------------- INTRO -----------------
// ----------------------------------------
void intro(){
  M5.Lcd.fillScreen(SafeDom);
  M5.Lcd.setTextSize(6);
  M5.Lcd.setTextColor(WHITE, SafeDom);
  M5.Lcd.setCursor(30, 40);
  M5.Lcd.print("SafeDom");
}

// ----------------------------------------
// --------------- MEDIUM -----------------
// ----------------------------------------
void medium(){
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(30,150);
  M5.Lcd.println("Conectando,");
  M5.Lcd.setCursor(30,180);
  M5.Lcd.print("espere");
  for(int i=0; i<5; i++){
    delay(1000);
    M5.Lcd.print(".");
  }
}

// ----------------------------------------
// --------------- ENDING -----------------
// ----------------------------------------
void ending(){
  M5.Lcd.clear();
  intro();
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(30,150);
  M5.Lcd.println("Calibrando,");
  M5.Lcd.setCursor(30,180);
  M5.Lcd.print("espere");
  for(int i=0; i<6; i++){
    delay(1000);
    M5.Lcd.print(".");
  }
}

// ----------------------------------------
// -------------- CONTEXT -----------------
// ----------------------------------------
void context(){
  M5.Lcd.clear();
  intro();
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE, SafeDom);
  M5.Lcd.setCursor(30, 150);
  M5.Lcd.println("Calibrado");
  M5.Lcd.setCursor(30, 180);
  M5.Lcd.print("correctamente");
  delay(5000);
  M5.Lcd.clear();
}

// ----------------------------------------
// ---------- CALIBRACION HX711 -----------
// ----------------------------------------
void calibracionHX711(){
  balanza.begin(DOUT, CLK);
  balanza.set_scale(-395584); // Escala
  balanza.tare(20);           // Tara
}

// ----------------------------------------
// ------------- mostrarLCD ---------------
// ----------------------------------------
void mostrarLCD(float peso){
  intro();
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(30, 150);
  M5.Lcd.println("Peso:");
  M5.Lcd.fillRect(30,180,240,20,SafeDom);
  M5.Lcd.setCursor(30, 180);
  M5.Lcd.print(peso);
  M5.Lcd.println(" Kg");
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
