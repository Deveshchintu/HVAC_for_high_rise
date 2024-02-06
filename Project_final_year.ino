#include "DHT.h"
int irPin1 = 4;
int irPin2 = 13;
int count = 0;
int enA = 12;
int in1 = 26;
int in2 = 27;
int enB = 14;
int in3 = 32;
int in4 = 33;
int DHT11pin = 16;
boolean state1 = true;
boolean state2 = true;
boolean insideState = false;
boolean outsideIr = false;
int i = 1;
int s = 0;
int pre=0;
long t = 0;
DHT dht(DHT11pin,DHT11);
void setup() {
  Serial.begin(115200);
   dht.begin();
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  t= millis();
}

void loop() {
  counter();
  delay(100);
  fan();
  delay(100);
  if((millis()-t) > 30000){
  curtain();
  t=millis();
 }

  // Additional logic or actions can be added here
}

void counter() {
// Serial.println("Counting people");
  if (!digitalRead(irPin1) && i == 1 && state1) {
    insideState = true;
    delay(100);
    i++;
    state1 = false;
  } else if (!digitalRead(irPin2) && i == 2 && state2 && insideState) {
    insideState = false;
    delay(100);
    i = 1;
    count++;
    if (count >= 0) {
      Serial.println("Entering inside the room");
      Serial.print("No. of people inside room: ");
      Serial.println(count);
    }
    state2 = false;
  } else if (!digitalRead(irPin2) && i == 1 && state2 && !insideState) {
    outsideIr = true;
    delay(100);
    i = 2;
    state2 = false;
  } else if (!digitalRead(irPin1) && i == 2 && state1 && outsideIr) {
    outsideIr = false;
    delay(100);
    count--;
    if (count >= 0) {
      Serial.println("Exiting from the room");
      Serial.print("No. of people inside room: ");
      Serial.println(count);
    }
    i = 1;
    state1 = false;
  }

  if (digitalRead(irPin1)) {
    state1 = true;
  }
  if (digitalRead(irPin2)) {
    state2 = true;
  }
}
void fan(){
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
s = 51*count;
if(s!=pre){
Serial.print("Speed of fan:");
Serial.println(s);
analogWrite(enB,s);
digitalWrite(in3,HIGH);
digitalWrite(in4,LOW);
pre=s;
}
}
void curtain(){
  delay(2000);  // Wait for 2 seconds between readings

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Adjust the threshold values according to your needs
  if (temperature > 30.0) {
    openCurtain();  // Open the curtain when temperature is above 30°C
  } else if (temperature < 25.0) {
    closeCurtain();  // Close the curtain when temperature is below 25°C
  }
}

void openCurtain() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 255);
  delay(2000);  
  stopMotor();
}

void closeCurtain() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 255);  
  delay(2000); 
  stopMotor();
}

void stopMotor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);  
}
