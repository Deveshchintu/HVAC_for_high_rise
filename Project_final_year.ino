int irPin1 = 4;
int irPin2 = 13;
int count = 0;
int enA = 12;
int in1 = 26;
int in2 = 27;
int enB = 14;
int in3 = 32;
int in4 = 33;
int DHT11 = 16;
boolean state1 = true;
boolean state2 = true;
boolean insideState = false;
boolean outsideIr = false;
int i = 1;
int s;

void setup() {
  Serial.begin(115200);
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);
}

void loop() {
  counter();
  // fan();
  // curtain();// Additional logic or actions can be added here
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