#ifdef ESP32 
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif

#include "SPI.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

// Libreria de servo
#include <ESP32Servo.h>

//variables de mensaje 1
int msjcont = 0;
int conold = 0;
static char contchar[7];

//variables de mensaje 2
int msjtemp = 0;

// Variables de sesor de ultrasonico 1
#define triger 27
#define echo 26
int distancia;
int duracion;

// Variables de sesor de ultrasonico 2
#define tring 15
#define echu 2
int dis;
int tiempo;
int contador = 0;
int buzzer = 32;
int led = 25;

// Variables de Sensor de temperatura
#define DHTPIN 22

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
String disp = "1";

/* Añade tu SSID & Clave para acceder a tu Wifi */
const char* ssid = "Cris";  // Tu SSID
const char* password = "12345678";  //Tu Clave
const char* serverName = "http://cirstopher-martinez-proyectos.atwebpages.com/peticiones.php";
const char* serverName1 = "http://cirstopher-martinez-proyectos.atwebpages.com/peticiones3.php";
const char* serverName2 = "http://cirstopher-martinez-proyectos.atwebpages.com/peticiones4.php";

WebServer server(80);

uint8_t LED1pin = 19;
bool LED1Estado = LOW;
//uint8_t LED2pin = 5;
//bool LED2Estado = LOW;
String estatuspuerta;

void devolver_info() 
{
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  
  float d = distancia;
  String datos;
  if(digitalRead(LED1pin)==LOW)
  {
    estatuspuerta = "CERRADO";
  }
  else
  {
    estatuspuerta = "ABIERTO";
  }

  // Armando el json
  datos = "{\"distancia\": \"" + String(d) + "\",\"contador\": \"" + 
  String(contador)+"\",\"puerta\": \"" + estatuspuerta + "\",\"temperatura\": \"" + String(t) + "\"}";      
  Serial.println(datos); // 2
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200, "application/json", datos); // 3
}

void abrir_puerta() 
{
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  digitalWrite(LED1pin,HIGH); //1
  //variable de la distancia
  float d = distancia;
  if(digitalRead(LED1pin)==LOW)
  {
    estatuspuerta = "CERRADO";
  }
  else
  {
    estatuspuerta = "ABIERTO";
  }


  myservo.write(70);

    if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // La URL de tu dominio
    http.begin(client, serverName1);
    
    // Especificar el content type para el header del POST
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Preparar los datos POST a enviar
     String httpRequestData = "estado_puerta=" + String(estatuspuerta);
    
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Envia los datos por POST
    int httpResponseCode = http.POST(httpRequestData);

    // Si la respuesta no dio error
    if (httpResponseCode>0) {
      Serial.print("HTTP Codigo de respuesta: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Codigo de error: ");
      Serial.println(httpResponseCode);
    }
    // Termina la conexion del POST
    http.end();
  }

  String datos;
  // Armando el json
  datos = "{\"distancia\": \"" + String(d) + "\",\"contador\": \"" + 
  String(contador)+"\",\"puerta\": \"" + estatuspuerta + "\",\"temperatura\": \"" + String(t) + "\"}";       
  Serial.println("PUERTA Estado: ABIERTO"); // 2
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200, "application/json", datos ); //3
  delay(30000); 
}

void cerrar_puerta() 
{
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  digitalWrite(LED1pin,LOW); //1
  if(digitalRead(LED1pin)==LOW)
  {
    estatuspuerta = "CERRADO";
    
  }
  else
  {
    estatuspuerta = "ABIERTO";
  }

   myservo.write(pos);

  float d = distancia;

  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // La URL de tu dominio
    http.begin(client, serverName1);
    
    // Especificar el content type para el header del POST
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Preparar los datos POST a enviar
     String httpRequestData = "estado_puerta=" + String(estatuspuerta);
    
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Envia los datos por POST
    int httpResponseCode = http.POST(httpRequestData);

    // Si la respuesta no dio error
    if (httpResponseCode>0) {
      Serial.print("HTTP Codigo de respuesta: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Codigo de error: ");
      Serial.println(httpResponseCode);
    }
    // Termina la conexion del POST
    http.end();
  }
  
  String datos;
  // Armando el json
  datos = "{\"distancia\": \"" + String(d) + "\",\"contador\": \"" + 
  String(contador)+"\",\"puerta\": \"" + estatuspuerta + "\",\"temperatura\": \"" + String(t) + "\"}";         
  Serial.println("Puerta Estado: CERRADO"); // 2
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200, "application/json", datos); //3
   delay(30000); 
}

void setup() {
  /*
   * Declaracion de la velocidad de comunicacion entre Arduino IDE y ESP32
   * Configura el comportamiento de los pines
   */
  Serial.begin(115200);

   WiFi.begin(ssid, password);
  Serial.println("Intentando conectar a la red");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conexion completada, la direccion ip es: ");
  Serial.println(WiFi.localIP());
  //inicar sensor de temperatura
  dht.begin();

    //pin de servo
  myservo.attach(13);

  pinMode(LED1pin, OUTPUT);

  pinMode(triger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(led, OUTPUT);

  pinMode(tring, OUTPUT);
  pinMode(echu, INPUT);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  server.on("/", devolver_info); // 1
  server.on("/puerta/ABIERTO", abrir_puerta); // 2
  server.on("/puerta/CERRADO", cerrar_puerta); // 2

/*
 * Arrancamos el Servicio WEB
 */
  server.begin();
  Serial.println("Servidor HTTP iniciado");

/*
 * Para gestionar las la peticiones HTTP es necesario llamar al metodo "handleClient"
 * de la libreria WebServer que se encarga de recibir las peticiones y lanzar las fuciones
 * de callback asociadas tipo "handle_led1on()" "handle_led2on()" etc
 * Tambien ejecutan el cambio de estado de los pines y por lo tanto hacen que los 
 * LEDs se enciendan o apaguen
 */
  
}

void loop() {

  server.handleClient();

// Sesor de ultrasonico 1
  digitalWrite(triger, LOW);
  delayMicroseconds(4);
  digitalWrite(triger, HIGH);
  delayMicroseconds(10);
  digitalWrite(triger, LOW);
  duracion = pulseIn(echo, HIGH);
  duracion = duracion / 2;
  distancia = duracion / 29.2;
  //Serial.println("la distancia es ");
  //Serial.println(distancia);

  if (distancia <= 20) {
    myservo.write(70);
  }
  if (distancia > 20) {
    myservo.write(pos);
  }

  // Sesor de ultrasonico 2
  digitalWrite(tring, LOW);
  delayMicroseconds(4);
  digitalWrite(tring, HIGH);
  delayMicroseconds(10);
  digitalWrite(tring, LOW);
  tiempo = pulseIn(echu, HIGH);
  tiempo = tiempo / 2;
  dis = tiempo / 29.2;
  //Serial.println("la distancia 2 es ");
  //Serial.println(dis);

  if (dis <= 5) {
    contador = contador + 1;
    digitalWrite(led,HIGH);
    Serial.print("El contador es ");
    Serial.println(contador);
    delay(500);
    digitalWrite(led,LOW);
    delay(1000);

    if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // La URL de tu dominio
    http.begin(client, serverName2);
    
    // Especificar el content type para el header del POST
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Preparar los datos POST a enviar
     String httpRequestData = "contador=" + String(contador);
    
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Envia los datos por POST
    int httpResponseCode = http.POST(httpRequestData);

    // Si la respuesta no dio error
    if (httpResponseCode>0) {
      Serial.print("HTTP Codigo de respuesta: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Codigo de error: ");
      Serial.println(httpResponseCode);
    }
    // Termina la conexion del POST
    http.end();
  }
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Error obteniendo los datos del sensor DHT11"));
    return;
  }

 //Serial.println(t);
 if (t <= 25){
    digitalWrite(triger, LOW);
    digitalWrite(echo, LOW);
    myservo.write(70);
    digitalWrite(buzzer, HIGH);
    delay (5000);
    digitalWrite(buzzer, LOW);

    if (msjtemp ==0){

    if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // La URL de tu dominio
    http.begin(client, serverName);
    
    // Especificar el content type para el header del POST
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Preparar los datos POST a enviar
    String httpRequestData = "temp=" + String(t) + "&hum=" + String(h) + "&disp=" + String(disp);
    
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Envia los datos por POST
    int httpResponseCode = http.POST(httpRequestData);

    // Si la respuesta no dio error
    if (httpResponseCode>0) {
      Serial.print("HTTP Codigo de respuesta: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Codigo de error: ");
      Serial.println(httpResponseCode);
    }
    // Termina la conexion del POST
    http.end();
  }
  
  else {
    Serial.println("Desconectado del wifi");
  }
  //Tiempo de espera entre solicitudes al servidor
      msjtemp = 1;
    }//fin de if para enviar mensaje de temperatura
 
  } else if(t >=20) msjtemp = 0; 
    //Si esta conectado a internet
 
  }
