#include <DHT.h>
#include<WiFi.h>
#define DHTPIN 25    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 
#include<ThingSpeak.h>
const char* ssid="AD 354";
const char* password="12345678";

  // DHT 11

DHT dht(DHTPIN, DHTTYPE);

int fs = 32;       // Flame sensor input pin
int IN3_PIN = 2;   // Motor control pin
int IN4_PIN = 23;  // Motor control pin
int ENB_PIN = 5;   // Motor speed control pin
unsigned long mychno =2429419;
const char* apikey="IEH65KKJHJCC57JS";
WiFiClient client;
void setup() {
  Serial.begin(115200);
   WiFi.begin(ssid,password);
  ThingSpeak.begin(client);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(fs, INPUT);
  dht.begin();
}

void loop() {
  // Read the flame sensor value
  int flameDetected = digitalRead(fs);
  ThingSpeak.writeField(mychno, 5, flameDetected, apikey);
// Read temperature and humidity from DHT11 sensor
  float temperature = dht.readTemperature();  // Read temperature in Celsius
  float humidity = dht.readHumidity();        // Read humidity

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the sensor values to the Serial Monitor
  Serial.print("Flame detected: ");
  Serial.println(flameDetected);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(2000);

  // Control the pump based on the flame sensor and temperature
  if (flameDetected == 0 && temperature > 70.0) {  // If flame is detected and temperature is greater than 70°C
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, HIGH);
    analogWrite(ENB_PIN, 255);  // Full speed
  } else {  // If no flame is detected or temperature is not greater than 70°C
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, LOW);
    analogWrite(ENB_PIN, 0);  // Motor off
  }
}
