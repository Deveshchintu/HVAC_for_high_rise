#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include<WiFi.h>
#include<ThingSpeak.h>
const char* ssid="AD 354";
const char* password="12345678";


#define DHTPIN 25    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// Define motor control pins
int enA = 14;
int in1 = 26;
int in2 = 27;
int enB = 12;
int in3 = 19;
int in4 = 18;

// OLED display settings
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const float temperatureThreshold = 25.0;
boolean curtainOpen = false;
unsigned long mychno =2429419;
const char* apikey="IEH65KKJHJCC57JS";
WiFiClient client;
void setup() {
  Serial.begin(115200);
  dht.begin();
    WiFi.begin(ssid,password);
  ThingSpeak.begin(client);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
}

void loop() {
  delay(2000);  // Wait for 2 seconds between measurements

  float temperature = dht.readTemperature();  // Read temperature in Celsius
  float humidity = dht.readHumidity();        // Read humidity
      ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(3, humidity);
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Display temperature and humidity on OLED
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner
  display.print(F("Temp: "));
  display.print(temperature);
  display.println(" C");
  display.print(F("Humidity: "));
  display.print(humidity);
  display.println(" %");
  display.display();

  // Control curtain based on temperature
  if (temperature > temperatureThreshold && !curtainOpen) {
 openCurtain();  
  } else if (temperature <= temperatureThreshold && curtainOpen) {
     closeCurtain();
  }
}

void openCurtain() {
  Serial.println("Opening the curtain");
  curtainOpen = true;
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); // Motor runs clockwise
  analogWrite(enB, 255);
  delay(5000);  
  stopMotor();
}

void closeCurtain() {
  Serial.println("Closing the curtain");
  curtainOpen = false;
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); // Motor runs anticlockwise
  analogWrite(enB, 255);  
  delay(5000); 
  stopMotor();
}

void stopMotor() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);  
}
//fan logic