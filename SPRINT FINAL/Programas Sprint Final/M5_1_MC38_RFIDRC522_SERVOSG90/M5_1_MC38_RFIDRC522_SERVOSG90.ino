/*
  CONEXIONES:
  
  RFID <-> M5Stack
  
  3.3V <-> 3V3
  RST (Reset) <-> G22
  GND <-> GND
  MISO (Master Input Slave Output) <-> G19
  MOSI (Master Output Slave Input) <-> G23
  SCK (Serial Clock) <-> G18
  SDA (Slave select) <-> G21
*/

#include <SPI.h>
#include <MFRC522.h>
#include <M5Stack.h>
#include <ESP32_Servo.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>

#define SafeDom 0xF249    // COLOR SAFEDOM
#define PIN_REST 22       // RST RFID RC522
#define PIN_SDA 21        // SDA RFID RC522
#define PIN_SERVO G5      // MOTOR SERVO SG90
#define PIN_MC38 G2       // SENSOR MAGNETICO MC38
#define PIN_BTN_INT 39    // BOTON INTERRUPCION (cerrar puerta)

MFRC522 RFID(PIN_SDA, PIN_REST);
Servo PUERTA;
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// ----------------------------------------
// -------------- VARIABLES ---------------
// ----------------------------------------

// ----- WIFI -----
char ssid[] = "ALCALA - MAS BLANCO";
char pass[] = "bloque3puerta5";

// ----- BROKER -----
const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic1[] = "safedom/puerta/";
const char topic2[] = "safedom/puerta/ddbb/";
String content;

// ----- RFID -----
String BufferID = "";
const String RFID_Code = "185175180195";

// ----- GENERALES -----
volatile bool tokenPuerta = false;
bool contadorCerrada=true;
bool contadorAbierta=true;

// ****************************************
// **************** SETUP *****************
// ****************************************
void setup() {
  M5.begin(true, false, true, false);
  intro();
  progress(5);
  SPI.begin();
  progress(10);
  
  // ----- WIFI -----
  conexionWiFi();
  progress(39);
  
  // ----- MQTT -----
  conexionMQTT();
  progress(61);
  
  // ----- SERVO -----
  servoSetup();
  progress(74);
  
  // ----- RFID -----
  RFID.PCD_Init();
  progress(87);
  
  // ----- PINMODE / INTERRUPCIONES -----
  pinMode(PIN_BTN_INT, INPUT_PULLDOWN);
  pinMode(PIN_MC38, INPUT_PULLUP);
  attachInterrupt(PIN_BTN_INT, tokenCerrarPuerta, RISING);
  progress(100);
  ending();
}


// ****************************************
// **************** LOOP ******************
// ****************************************
void loop() {
  mqttClient.poll();
  interrupcionCerrarPuerta();
  estadoPuerta();
  lectorRFID();
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
  delay(1000);
  M5.Lcd.progressBar(30,180,240,20,0);
}
void progress(int newProgress){
  M5.Lcd.fillRect(30,180,240,20,0);
  M5.Lcd.progressBar(30,180,240,20,newProgress);
}

// ----------------------------------------
// --------------- ENDING -----------------
// ----------------------------------------
void ending(){
  delay(1500);
  for(int i=0; i<=245; i++){
    M5.Lcd.fillRect(30,180,i,20,SafeDom);
    delay(5);
  }
  delay(1500);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(30,180);
  M5.Lcd.print("Bienvenido");
  delay(3500);
  M5.Lcd.clear();
}

// ----------------------------------------
// ------------- SERVO SETUP --------------
// ----------------------------------------
void servoSetup(){
  PUERTA.attach(PIN_SERVO, 500, 2400);
  PUERTA.write(180);
}

// ----------------------------------------
// ------------ CONEXION WIFI -------------
// ----------------------------------------
void conexionWiFi(){
  Serial.println();
  Serial.print("WIFI: Conectando a la red: ");
  Serial.println(ssid);
  Serial.print("WIFI: ");
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
  mqttClient.setId("12fhrqw213as");
  Serial.print("MQTT: Conectando al broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT: ERROR EN EL BROKER: Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }

  // MESSAGE CALLBACK
  mqttClient.onMessage(onMqttMessage);
  
  Serial.println("MQTT: CONECTADO");
  Serial.println("MQTT: Suscrito al topic: ");
  Serial.print("MQTT: ");
  Serial.println(topic1);
  mqttClient.subscribe(topic1);
  Serial.println();
}

// ----------------------------------------
// ------------ CALLBACK MQTT -------------
// ----------------------------------------
void onMqttMessage(int messageSize){
  content = "";

  // MESSAGE RECIEVED
  Serial.print("MQTT: Mensaje recibido en ");
  Serial.println(mqttClient.messageTopic());
  
  if(mqttClient.messageTopic()==topic1){
    while(mqttClient.available()){
      content.concat((char)mqttClient.read());
    }
    Serial.println(content);
    funcPuerta(content);
  }
}

// ----------------------------------------
// ------ INTERRUPCION CERRAR PUERTA ------
// ----------------------------------------
void tokenCerrarPuerta(){
  tokenPuerta = true;
}
void interrupcionCerrarPuerta(){
  if(tokenPuerta){
    funcPuerta("cerrarpuerta");
    tokenPuerta = false;
  }
}

// ----------------------------------------
// --------- ESTADO DE LA PUERTA ----------
// ----------------------------------------
void estadoPuerta(){
  bool valor = digitalRead(PIN_MC38);
  if(valor){
    if(contadorAbierta){
      contadorAbierta=false;
      contadorCerrada=true;
      mqttClient.beginMessage(topic2);
      mqttClient.print("abierta");
      mqttClient.endMessage();
      delay(250);
    }
  }
  if(!valor){
    if(contadorCerrada){
      contadorCerrada=false;
      contadorAbierta=true;
      mqttClient.beginMessage(topic2);
      mqttClient.print("cerrada");
      mqttClient.endMessage();
      delay(250);
    }
    
  }
}

// ----------------------------------------
// ------------ LECTOR RFID ---------------
// ----------------------------------------
void lectorRFID(){
  if(RFID.PICC_IsNewCardPresent()){
    if(RFID.PICC_ReadCardSerial()){
      BufferID = "";
      Serial.println();
      Serial.print("Card UID: ");
      for(byte i = 0; i < RFID.uid.size; i++){
        BufferID.concat(RFID.uid.uidByte[i]);
      }
      Serial.println(BufferID);
      if(BufferID==RFID_Code){
        Serial.println("OK");
        funcPuerta("abrirpuerta");
      }
      if(BufferID!=RFID_Code){
        Serial.println("DENEGADA");
        M5.Lcd.fillScreen(RED);
        delay(1500);
        M5.Lcd.fillScreen(BLACK);
      }
      // Terminamos la lectura de la tarjeta  actual
      RFID.PICC_HaltA();                                                               
    }      
  } 
}

// ----------------------------------------
// ---------- FUNC ABRIR PUERTA -----------
// ----------------------------------------
void funcPuerta(String content){
  if(content=="abrirpuerta"){
    if(PUERTA.read()==90){
      Serial.println("Puerta ya abierta!");
      for(int i=0; i<4; i++){
        M5.Lcd.fillScreen(BLUE);
        delay(250);
        M5.Lcd.fillScreen(BLACK);
        delay(250);
      }
    }
    else{
      M5.Lcd.fillScreen(GREEN);
      Serial.println("Abriendo puerta...");
      PUERTA.write(90);
      delay(1500);
      M5.Lcd.fillScreen(BLACK);
    }
  }
  if(content=="cerrarpuerta"){
    if(PUERTA.read()==180){
      Serial.println("Puerta ya cerrada!");
      for(int i=0; i<4; i++){
        M5.Lcd.fillScreen(BLUE);
        delay(250);
        M5.Lcd.fillScreen(BLACK);
        delay(250);
      }
    }
    else{
      M5.Lcd.fillScreen(WHITE);
      Serial.println("Cerrando puerta...");
      PUERTA.write(180);
      delay(1500);
      M5.Lcd.fillScreen(BLACK);
    }
  }
}
