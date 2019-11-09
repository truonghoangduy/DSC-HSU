//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "iot-thcntt1.firebaseio.com"
#define FIREBASE_AUTH "tlumFf4Akz9mRrsgixc546bxGzXshIXY1MntCHzO"
//#define FIREBASE_AUTH ""
#define WIFI_SSID "HSU_Students"
#define WIFI_PASSWORD "dhhs12cnvch"
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

#define dht_dpin 0
DHT dht(dht_dpin, DHTTYPE); 

void setup() {
  Serial.begin(9600);
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  dht.begin();
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setInt("temp",99);
}

int n = 0;
void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Firebase.setFloat("sensor/temp",t);
  delay(300);
  Firebase.setFloat("sensor/hum",h);
  delay(300);
  Firebase.setInt("temp",n++);
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  else
  {
//  Serial.print(Firebase.getFloat("duy/sensor/temp"));
  Serial.print("temp : ");
  Serial.println(t);
  Serial.print("hum : ");
  Serial.println(h);
  
  }
  delay(1000);
}
