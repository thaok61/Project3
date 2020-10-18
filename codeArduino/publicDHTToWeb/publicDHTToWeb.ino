// Loading the ESP8266WiFi library and the PubSubClient library
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "AESLib.h"
// define DHT 11
#define DHTTYPE DHT11 //DHT 11

AESLib aesLib;
byte aes_key[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

//Change the credientials below, so your ESP8266connects to your router
const char* ssid = "test";
const char* password = "12345678";

//Change the variable to yur Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.43.179";
const char* mqtt_user = "thao";
const char* mqtt_pass = "1";

// Intializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

// DHT Sensor
const int DHTPin = 4;

// Initialize DHT Sensor.
DHT dht(DHTPin, DHTTYPE);

// Timer's auxiliary variables
long now = millis();
long lastMeasure = 0;


String encrypt(char * msg, uint16_t msgLen, byte iv[]) {
  int cipherlength = aesLib.get_cipher64_length(msgLen);
  char encrypted[cipherlength]; // AHA! needs to be large, 2x is not enough
  aesLib.encrypt64(msg, msgLen, encrypted, aes_key, sizeof(aes_key), iv);
  Serial.print("encrypted = "); Serial.println(encrypted);
  return String(encrypted);
}

String decrypt(char * msg, uint16_t msgLen, byte iv[]) {
  char decrypted[msgLen];
  aesLib.decrypt64(msg, msgLen, decrypted, aes_key, sizeof(aes_key), iv);
  return String(decrypted);
}

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
} 

//This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    if (client.connect("ESP8266lights", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} 

//The setup function sets your ESP GPIOs to Outputs, starts the serial communication at 
//a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  dht.begin();
  
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  aesLib.set_paddingmode(paddingMode::Array);
} 

void wait(unsigned long milliseconds) {
  unsigned long timeout = millis() + milliseconds;
  while (millis() < timeout) {
    yield();
  }
}

//For this project, you don't need to change anything in the loop function.
// Basically it ensures that you ESP is connected to your broker
void loop() {
  byte key[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };
    
  // put your setup code here, to run once:
  byte temp_iv[N_BLOCK] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  byte humidity_iv[N_BLOCK] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  if (!client.connected()) {
    reconnect();
  }
  if (!client.loop())
    client.connect("ESP8266lights", mqtt_user, mqtt_pass);
  now = millis();
  // Pulishes new temperature and humidity every 5 seonds
  if (now - lastMeasure > 5000) {
    lastMeasure = now;
    // Read the humidity values
    float h = dht.readHumidity();
    // Read the temperature as Celsius (the default)
    float t= dht.readTemperature();
    // Checking the readings(to tru again)
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor");
      return;
    }
    static char temperatureTemp[256];
    dtostrf(t, 6, 2, temperatureTemp);

    static char humidityTemp[256];
    dtostrf(h, 6, 2, humidityTemp);

    uint16_t tempLength = String(temperatureTemp).length();
    uint16_t humidLength = String(humidityTemp).length();
    
    String encTemperature = encrypt(temperatureTemp, tempLength, temp_iv);
    String encHumidity = encrypt(humidityTemp, humidLength, humidity_iv);

    int str_LengthTemperature = encTemperature.length() + 1;
    int str_LenghtHumidity = encHumidity.length() + 1;
    
    char encTemperatureChar[str_LengthTemperature];
    char encHumidityChar[str_LenghtHumidity];

    encTemperature.toCharArray(encTemperatureChar, str_LengthTemperature);
    encHumidity.toCharArray(encHumidityChar, str_LenghtHumidity);
    
    // Publishes Temperature and Humidity values
    client.publish("esp8266/temperature",encTemperatureChar);
    client.publish("esp8266/humidity",encHumidityChar);

    Serial.print("Humidity: ");
    Serial.print(h);
    
    Serial.print("%t Temperature: ");
    Serial.print(t);
    Serial.println(" \t\t\t\t\t *C ");
  }
}
