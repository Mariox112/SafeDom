#include <FirebaseESP32.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>

#define Firebase_Host "https://safedom-b7c1c-default-rtdb.europe-west1.firebasedatabase.app"
//#define Firebase_Auth "Ji5BY3xGWXxrW8hKnUcKdKBNv5lj7iviDkNfLQGZ"
#define Firebase_Auth "AIzaSyBH-isWNbiV0IzKYMVN8dydjNVrInEKyis"

char ssid[] = "ALCALA - MAS BLANCO";
char pass[] = "bloque3puerta5";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
FirebaseData firebaseData;

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic1[] = "safedom/luces/";
const char topic2[] = "safedom/puerta/ddbb/";
const char topic3[] = "safedom/DHT11/Temperatura/";
const char topic4[] = "safedom/DHT11/Humedad/";
const char topic5[] = "safedom/HX711/actividad/";
const char topic6[] = "safedom/HX711/tiempoActivo/";
bool iterar = true;


//------------------------------
//----------PIN-----------------
//------------------------------
const int ledPINCom = 3;
const int ledPINCocR = 1;
const int ledPINCocG = 16;
const int ledPINCocB = 17;
const int ledPINDormitorioP = 2;
const int ledPINDormitorio1R = 5;
const int ledPINDormitorio1G = 25;
const int ledPINDormitorio1B = 26;
const int ledPINDormitorio2 = 35;
const int ledPINEntrada = 36;

//-----------------------------------
//----------TodasLuzON------------
//-----------------------------------
void todasLuzON(){
  digitalWrite(ledPINCom , HIGH);
  digitalWrite(ledPINCocR,   255);
  digitalWrite(ledPINCocG, 255);
  digitalWrite(ledPINCocB,  255);
  digitalWrite(ledPINDormitorioP , HIGH);
   digitalWrite(ledPINDormitorio1R,   255);
  digitalWrite(ledPINDormitorio1G, 255);
  digitalWrite(ledPINDormitorio1B,  255);
  digitalWrite(ledPINDormitorio2 , HIGH);
  digitalWrite(ledPINEntrada , HIGH);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Todas_Luces", "Encendida");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Comedor_Luces", "Encendida");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Cocina_Luces", "Encendida");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/DormitorioPrincipal_Luces", "Encendida");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Dormitorio1_Luces", "Encendida");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Dormitorio2_Luces", "Encendida");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Entrada_Luces", "Encendida");
  
}

//------------------------------------
//----------TodasLuzOFF------------
//------------------------------------
   void todasLuzOFF(){
  digitalWrite(ledPINCom , LOW);
  digitalWrite(ledPINCocR,   0);
  digitalWrite(ledPINCocG, 0);
  digitalWrite(ledPINCocB,  0);
  digitalWrite(ledPINDormitorioP , LOW);
   digitalWrite(ledPINDormitorio1R,   0);
  digitalWrite(ledPINDormitorio1G, 0);
  digitalWrite(ledPINDormitorio1B,  0);
  digitalWrite(ledPINDormitorio2 , LOW);
  digitalWrite(ledPINEntrada , LOW);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Todas_Luces", "Apagada");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Todas_Luces", "Apagada");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Comedor_Luces", "Apagada");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Cocina_Luces", "Apagada");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/DormitorioPrincipal_Luces", "Apagada");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Dormitorio1_Luces", "Apagada");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Dormitorio2_Luces", "Apagada");
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Entrada_Luces", "Apagada");
}


//-----------------------------------
//-----------ComedorLuzON------------
//-----------------------------------
void comedorLuzON(){
  digitalWrite(ledPINCom , HIGH);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Comedor_Luces", "Encendida");
}

//------------------------------------
//-----------ComedorLuzOFF------------
//------------------------------------
    void comedorLuzOFF(){
  digitalWrite(ledPINCom , LOW);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Comedor_Luces", "Apagada");
}

//-----------------------------------
//----------CocinaLuzON------------
//-----------------------------------
void cocinaLuzON(){
  digitalWrite(ledPINCocR,   255);
  digitalWrite(ledPINCocG, 255);
  digitalWrite(ledPINCocB,  255);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Cocina_Luces", "Encendida");
}

//------------------------------------
//----------CocinaLuzOFF------------
//------------------------------------
  void cocinaLuzOFF(){
  digitalWrite(ledPINCocR,0);
  digitalWrite(ledPINCocG,0);
  digitalWrite(ledPINCocB,0);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Cocina_Luces", "Apagada");
}

//--------------------------------------
//----------DormitorioPLuzON------------
//--------------------------------------
void dormitorioPLuzON(){
  digitalWrite(ledPINDormitorioP , HIGH);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/DormitorioPrincipal_Luces", "Encendida");
}

//---------------------------------------
//----------DormitorioPLuzOFF------------
//---------------------------------------
    void dormitorioPLuzOFF(){
  digitalWrite(ledPINDormitorioP , LOW);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/DormitorioPrincipal_Luces", "Apagada");
}

//--------------------------------------
//----------Dormitorio1LuzON------------
//--------------------------------------
void dormitorio1ON(){
  digitalWrite(ledPINDormitorio1R,   255);
  digitalWrite(ledPINDormitorio1G, 255);
  digitalWrite(ledPINDormitorio1B,  255);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Dormitorio1_Luces", "Encendida");
}

//---------------------------------------
//----------Dormitorio1LuzOFF------------
//---------------------------------------
  void dormitorio1OFF(){
  digitalWrite(ledPINDormitorio1R,   0);
  digitalWrite(ledPINDormitorio1G, 0);
  digitalWrite(ledPINDormitorio1B,  0);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Dormitorio1_Luces", "Apagada");
}

//--------------------------------------
//----------Dormitorio2LuzON------------
//--------------------------------------
void dormitorio2LuzON(){
  digitalWrite(ledPINDormitorio2 , HIGH);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Dormitorio2_Luces", "Encendida");
}

//---------------------------------------
//----------Dormitorio2LuzOFF------------
//---------------------------------------
    void dormitorio2LuzOFF(){
  digitalWrite(ledPINDormitorio2 , LOW);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Dormitorio2_Luces", "Apagada");
}

//--------------------------------------
//----------DormitorioELuzON------------
//--------------------------------------
void dormitorioELuzON(){
  digitalWrite(ledPINEntrada , HIGH);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Entrada_Luces", "Encendida");
}

//---------------------------------------
//----------DormitorioELuzOFF------------
//---------------------------------------
    void dormitorioELuzOFF(){
  digitalWrite(ledPINEntrada , LOW);
  Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Entrada_Luces", "Apagada");
}




void setup() {
  Serial.begin(115200);

  // WIFI NETWORK
  Serial.print("Conectando a SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(3000);
  }
  Serial.println("CONECTADO AL WIFI");
  Serial.println();

  // BROKER NETWORK
  Serial.print("Conectando al broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("ERROR EN LA CONEXION: Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }
  Serial.println("CONECTADO AL BROKER");
  Serial.println();
  
//  PIN
  pinMode(ledPINCom , OUTPUT); 
  pinMode(ledPINCocR , OUTPUT); 
  pinMode(ledPINCocG , OUTPUT); 
  pinMode(ledPINCocB , OUTPUT); 
  pinMode(ledPINDormitorioP , OUTPUT); 
  pinMode(ledPINDormitorio1R, OUTPUT); 
  pinMode(ledPINDormitorio1G , OUTPUT); 
  pinMode(ledPINDormitorio1B , OUTPUT); 
  pinMode(ledPINDormitorio2 , OUTPUT); 
  pinMode(ledPINEntrada , OUTPUT); 

  // MESSAGE CALLBACK
  mqttClient.onMessage(onMqttMessage);

  // SUBSCRIBE TO TOPIC
  Serial.println("Subscribiendose al topic: ");
  Serial.println(topic1);
  Serial.println(topic2);
  Serial.println(topic3);
  Serial.println(topic4);
  Serial.println(topic5);
  Serial.println(topic6);
  Serial.println();
  mqttClient.subscribe(topic1);
  mqttClient.subscribe(topic2);
  mqttClient.subscribe(topic3);
  mqttClient.subscribe(topic4);
  mqttClient.subscribe(topic5);
  mqttClient.subscribe(topic6);
  

  // FIREBASE 
  Firebase.begin(Firebase_Host, Firebase_Auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  mqttClient.poll();
}

// --------------------------------
// --------------------------------
// --------------------------------
void onMqttMessage(int messageSize) {

  // VARIABLES
  String content = "";
  
  // MESSAGE RECIEVED
  Serial.print("Mensaje recibido en ");
  Serial.println(mqttClient.messageTopic());
  
  if(mqttClient.messageTopic()==topic1){
    while(mqttClient.available()){
      content.concat((char)mqttClient.read());
    }
    Serial.println(content);
    if(content=="encencdercomedorluz"){
       comedorLuzON();
    }
    else if(content=="apagarcomedorluz"){
       comedorLuzOFF();
    }
    else if(content=="encencderdormitoriopluz"){
       dormitorioPLuzON();
    }
    else if(content=="apagardormitoriopluz"){
       dormitorioPLuzOFF();
    }
     else if(content=="encencderdormitorio2luz"){
      dormitorio2LuzON();
    }
     else if(content=="apagardormitorio2luz"){
      dormitorio2LuzOFF();
    }
     else if(content=="encencderentradaluz"){
      dormitorioELuzON();
    }
     else if(content=="apagarentradaluz"){
      dormitorioELuzOFF();
    }
    else if(content=="encencdercocinaluz"){
      cocinaLuzON();
    }
     else if(content=="apagarcocinaluz"){
      cocinaLuzOFF();
    }
    else if(content=="encencderdormitorio1luz"){
      dormitorio1ON();
    }
     else if(content=="apagardormitorio1luz"){
      dormitorio1OFF();
    }
    else if(content=="encencdertodasluz"){
      todasLuzON();
    }
     else if(content=="apagartodasluz"){
      todasLuzOFF();
    }
  }
  
  if(mqttClient.messageTopic()==topic2){
    while(mqttClient.available()){
      content.concat((char)mqttClient.read());
    }
    Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Puerta_Principal", content);
    Serial.println(content);
  }
  
  if(mqttClient.messageTopic()==topic3){
    while(mqttClient.available()){
      content.concat((char)mqttClient.read());
    }
    Serial.println(content.toFloat());
    Firebase.setFloat(firebaseData, "/Casas/Clot, 68,20,5/Temperatura", content.toFloat());
  }

  if(mqttClient.messageTopic()==topic4){
    while(mqttClient.available()){
      content.concat((char)mqttClient.read());
    }
    Serial.println(content.toFloat());
    Firebase.setFloat(firebaseData, "/Casas/Clot, 68,20,5/Humedad", content.toFloat());
  }

  if(mqttClient.messageTopic()==topic5){
    while(mqttClient.available()){
      content.concat((char)mqttClient.read());
    }
    Serial.println(content.toInt());
    Firebase.setString(firebaseData, "/Casas/Clot, 68,20,5/Sensor_Peso", content);
  }

  if(mqttClient.messageTopic()==topic6){
    while(mqttClient.available()){
      content.concat((char)mqttClient.read());
    }
    Serial.println(content.toInt());
    Firebase.setInt(firebaseData, "/Casas/Clot, 68,20,5/Sensor_Peso_TiempoActivo", content.toInt());
  }
}
