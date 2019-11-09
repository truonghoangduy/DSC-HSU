/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11 

// Update these with values suitable for your network.

const char* ssid = "NTS";
const char* password = "0935118509";
const char* mqtt_server = "192.168.1.138";
DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msgt[50];//temp
char msgh[50];//hump
char msgg[50];//msgg gas
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println(BUILTIN_LED);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print(BUILTIN_LED);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("Sensor", "hello world");
      // ... and resubscribe
      //client.subscribe("User101/Sensor/1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(5, OUTPUT);
  dht.begin();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
    long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    int gas = analogRead(A0); 
    delay(5000);
    float t = dht.readTemperature();
    delay(400);
    float h = dht.readHumidity();
   // ++value;
     snprintf (msgh, 50, "%.2f", h);
     snprintf (msgt, 50, "%.2f", t);
     snprintf (msgg, 50, "%d", gas);
     value++;
    Serial.print("Publish message:");
    Serial.print(value);
    Serial.println();
    Serial.print("temp: ");
    Serial.println(msgt);
    Serial.print("hum: ");
    Serial.println(msgh);
    Serial.print("gas: ");
    Serial.println(msgg);
    client.publish("User/101/Sensor/2/cb", msgt);
    delay(300);
    client.publish("User/101/Sensor/5/cb", msgh);
    delay(300);
    client.publish("User/101/Sensor/4/cb", msgg);
  }
  client.loop();


  
}
