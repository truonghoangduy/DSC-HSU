#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

#define dht_dpin 0
DHT dht(dht_dpin, DHTTYPE); 
#define pumPin 16

String answer="";
float h=0;
float t =0;
float soildHum=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  pinMode(A0, INPUT);
  pinMode(pumPin,OUTPUT);
  dht.begin();
  delay(700);
  digitalWrite(pumPin,LOW);
  
}

void loop() {
  if(Serial.available()){
      answer=Serial.readString();
      if(answer=="0"){
           h = dht.readHumidity();
           t= dht.readTemperature();   
//          Serial.println("Hello world");
            Serial.println("{\"temp\":"+ String(t)+",\"hum\":"+String(h)+"}");
        }
      if(answer=="pump"){
          digitalWrite(pumPin,HIGH);
          delay(500);
          Serial.println("{\"pump\":"+String(true)+"}");
        }
      if(answer=="stoppump"){
          digitalWrite(pumPin,LOW);
          delay(500);
          Serial.println("{\"pump\":"+String(false)+"}");
        }
      if(answer=="soildum"){
          soildHum = analogRead(A0);
          Serial.println("{\"soildum\":"+String(soildHum)+"}");
        
        }
      
    }
}
