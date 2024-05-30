#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include<WiFi.h>
#include<ThingSpeak.h>
const char* ssid="AD 354";
const char* password="12345678";

// OLED display settings
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int l = 33;
int irPin1 = 4;
int irPin2 = 34;
int count = 0;
boolean state1 = true;
boolean state2 = true;
boolean insideState = false;
boolean outsideIr = false;
int i = 1;
unsigned long mychno =2429419;
const char* apikey="IEH65KKJHJCC57JS";
WiFiClient client;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  ThingSpeak.begin(client);
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);
  pinMode(l,OUTPUT);
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
  int t = digitalRead(irPin1);
  int h = digitalRead(irPin2);
  counter();
  updateDisplay();
  delay(100); 
  int s=51*count;
  analogWrite(l,s);
  if(count>5){
    analogWrite(l,255);
    }
    ThingSpeak.setField(1, t);
    ThingSpeak.setField(3, h);
  ThingSpeak.writeFields(mychno, apikey);
  delay(1000);// Update the display every 100ms
}

void counter() {
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

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner
  display.print(F("People: "));
  display.println(count);
  display.display();
}
