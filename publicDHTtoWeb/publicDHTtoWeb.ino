// Loading the ESP8266WiFi library and the PubSubClient library
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// define DHT 11
#define DHTTYPE DHT11 //DHT 11

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

// Connect an LED to each GPIO of your ESP8266
const int LED1 = 13;
const int LED2 = 12;

// DHT Sensor
const int DHTPin = 4;

// Initialize DHT Sensor.
DHT dht(DHTPin, DHTTYPE);

// Timer's auxiliary variables
long now = millis();
long lastMeasure = 0;

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

//This function is executed when some device publishes a message to a topic that your
//ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a
//message to a topic that
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
  Serial.print((char)message[i]);
  messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT
  
  // If a message is received on the topic esp8266/led1 or esp8266/led2, you check if the 
  //message is either 1 or 0 and turn the ESP8266 GPIO according to the message
  if(topic=="esp8266/LED2"){
    Serial.print("Changing LED2 to ");
    if(messageTemp == "1"){
      digitalWrite(LED2, HIGH);
      Serial.print("On");
    }
    else if(messageTemp == "0"){
      digitalWrite(LED2, LOW);
      Serial.print("Off");
    }
  }
  if(topic=="esp8266/LED1"){
    Serial.print("Changing LED1 to ");
     if(messageTemp == "1"){
      digitalWrite(LED1, HIGH);
      Serial.print("On");
     }
     else if(messageTemp == "0"){
      digitalWrite(LED1, LOW);
      Serial.print("Off");
     }
   }
   Serial.println();
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
      // Subscribe or resubscrie to a topic
      // you can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("esp8266/LED1");
      client.subscribe("esp8266/LED2");
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
//  pinMode(LED1, OUTPUT);
//  pinMode(LED2, OUTPUT);
  
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
//  client.setCallback(callback);
} 

//For this project, you don't need to change anything in the loop function.
// Basically it ensures that you ESP is connected to your broker
void loop() {
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
    static char temperatureTemp[7];
    dtostrf(t, 6, 2, temperatureTemp);

    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);

    // Publishes Temperature and Humidity values
    client.publish("esp8266/temperature",temperatureTemp);
    client.publish("esp8266/humidity",humidityTemp);

    Serial.print("Humidity: ");
    Serial.print(h);
    
    Serial.print("%t Temperature: ");
    Serial.print(t);
    Serial.println(" \t\t\t\t\t *C ");
  }
}
