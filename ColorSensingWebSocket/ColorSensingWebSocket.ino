#include <ArduinoHttpClient.h>
#include <WiFi.h>

using namespace std;

const int motorLeftBottom = 9; //Pin 9 on L293
const int motorLeftTop = 10; // Pin 15 on L293
const int motorRightBottom = 6; // Pin 2 on L293
const int motorRightTop = 4; // Pin 7 on L293

// TODO: Update pins to actual pins on Arduino
const int leftBluePin = 0;
const int leftRedPin = 0;
const int leftPTPin = 0; // Analog in
const int rightBluePin = 0;
const int rightRedPin = 0;
const int rightPTPin = 0; // Analog in

const int battVoltPin = 5; // Analog in
const int signifierLEDPin = 13;

static float leftBlackAmbient = 0.0;
static float leftRedAmbient = 0.0;
static float leftBlueAmbient = 0.0;
static float leftYellowAmbient = 0.0;
static float rightBlackAmbient = 0.0;
static float rightRedAmbient = 0.0;
static float rightBlueAmbient = 0.0;
static float rightYellowAmbient = 0.0;

// Websocket Settings //
char ssid[] = "tufts_eecs";
char pass[] = "foundedin1883";

char serverAddress[] = "34.28.153.91";  // server address
int port = 80;

WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = "828BD9E1B7C7"; //Insert your Server ID Here!
int status = WL_IDLE_STATUS;


// initialize digital pins for motors as an output.
void forward();
void backward();
void turnLeft();
void turnLeftWide();
void turnRight();
void turnRightWide();
void rotateLeft();
void rotateRight();
void reset();
void kill();

float collectLight(int pin);
void collectAmbient();

void setup() {
  pinMode(leftBluePin, OUTPUT);
  pinMode(leftRedPin, OUTPUT);
  pinMode(leftPTPin, INPUT);
  pinMode(rightBluePin, OUTPUT);
  pinMode(rightRedPin, OUTPUT);
  pinMode(rightPTPin, INPUT);
  pinMode(motorLeftBottom, OUTPUT);
  pinMode(motorLeftTop, OUTPUT);
  pinMode(motorRightBottom, OUTPUT);
  pinMode(motorRightTop, OUTPUT);
  pinMode(battVoltPin, INPUT);
  pinMode(signifierLEDPin, OUTPUT);

  reset();

  Serial.begin(9600);
  // collectAmbient();

  // WiFi Setup //
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Initial Websocket Test Print //
  Serial.println("starting WebSocket client");
  client.begin();
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();
}

void loop() {
  // readLight();

  int messageSize = client.parseMessage();
  if (messageSize > 0) {
    String message = client.readString();
    int strLength = message.length();
    if(message.substring(strLength - 4) == "Sizz"){
      Serial.println("Received a message:");
      Serial.println(message);
      if(message.substring(strLength - 11) == "forwardSizz"){
        // Serial.print("Calling forward\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling forward");
        // client.endMessage();
        forward();
      }else if(message.substring(strLength - 12) == "backwardSizz"){
        // Serial.print("Calling backward\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling backward");
        // client.endMessage();
        backward();
      }else if(message.substring(strLength - 8) == "leftSizz"){
        // Serial.print("Calling left\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling left");
        // client.endMessage();
        turnLeft();
      }else if(message.substring(strLength - 12) == "wideLeftSizz"){
        // Serial.print("Calling wide left\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling wide left");
        // client.endMessage();
        turnLeftWide();
      }else if(message.substring(strLength - 9) == "rightSizz"){
        // Serial.print("Calling right\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling right");
        // client.endMessage();
        turnRight();
      }else if(message.substring(strLength - 13) == "wideRightSizz"){
        // Serial.print("Calling wide right\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling wide right");
        // client.endMessage();
        turnRightWide();
      }else if(message.substring(strLength - 14) == "rotateLeftSizz"){
        // Serial.print("Calling in place left\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling in place left");
        // client.endMessage();
        rotateLeft();
      }else if(message.substring(strLength - 15) == "rotateRightSizz"){
        // Serial.print("Calling in place right\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling in place right");
        // client.endMessage();
        rotateRight();
      }else if(message.substring(strLength - 9) == "resetSizz"){
        // Serial.print("Calling reset\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling reset");
        // client.endMessage();
        reset();
      }
    }
  }

  if (true) {             // left and right PT read same color

    // Go forward until color change or collision
    // forward();

    if (true) { // If left was just on a color and now both on black

      // Turn right decent amount
      // Go forward

    } else if (false) {   // If right was just on color and now both on black

      // Turn left decent amount
      // Go forward

    }

  } else if (false) {     // left and right PT read diff colors
    // Bot just turned onto or off of a color path

    if (true) { // If was on all color before and left is now on black

      // Turn right slightly
      // Go forward

    } else if (false) {   // If was on all color before and right is now on black

      // Turn left slightly
      // Go forward

    }
  } else if (false) {
    
  } else if (false) {
    
  } else if (false) {
    
  } else if (false) {
    
  } else if (false) {
    
  }
  delay(10);
}

void kill(){
  // Serial.print("In kill\n");
  // Serial.print("Kill Button Read: ");
  // Serial.print(digitalRead(KILLSWITCH_PIN));
  // Serial.print("\n");
  reset();
}

void collectAmbient(){
  blink();
  blink();
  digitalWrite(signifierLEDPin, HIGH);
  leftBlackAmbient = collectLight(leftPTPin);
  rightBlackAmbient = collectLight(rightPTPin);
  Serial.print("Black Ambient Light Read: ");
  Serial.print(leftBlackAmbient);
  Serial.print("\n");
  digitalWrite(signifierLEDPin, LOW);
  blink();
  digitalWrite(signifierLEDPin, HIGH);
  leftRedAmbient = collectLight(leftPTPin);
  rightRedAmbient = collectLight(rightPTPin);
  Serial.print("Red Ambient Light Read: ");
  Serial.print(leftRedAmbient);
  Serial.print("\n");
  digitalWrite(signifierLEDPin, LOW);
  blink();
  digitalWrite(signifierLEDPin, HIGH);
  leftBlueAmbient = collectLight(leftPTPin);
  rightBlueAmbient = collectLight(rightPTPin);
  Serial.print("Blue Ambient Light Read: ");
  Serial.print(leftBlueAmbient);
  Serial.print("\n");
  digitalWrite(signifierLEDPin, LOW);
  blink();
  digitalWrite(signifierLEDPin, HIGH);
  leftYellowAmbient = collectLight(leftPTPin);
  rightYellowAmbient = collectLight(rightPTPin);
  Serial.print("Yellow Ambient Light Read: ");
  Serial.print(leftYellowAmbient);
  Serial.print("\n");
  // digitalWrite(signifierLEDPin, LOW);
}

void blink(){
  digitalWrite(signifierLEDPin, HIGH);
  delay(300);
  digitalWrite(signifierLEDPin, LOW);
  delay(300);
  digitalWrite(signifierLEDPin, HIGH);
  delay(300);
  digitalWrite(signifierLEDPin, LOW);
  delay(300);
  digitalWrite(signifierLEDPin, HIGH);
  delay(300);
  digitalWrite(signifierLEDPin, LOW);
  delay(300);
  digitalWrite(signifierLEDPin, HIGH);
  delay(300);
  digitalWrite(signifierLEDPin, LOW);
  delay(300);
  digitalWrite(signifierLEDPin, HIGH);
  delay(300);
  digitalWrite(signifierLEDPin, LOW);
  delay(300);
}

float collectLight(int pin){
  int totalRead = 0;
  totalRead += analogRead(pin);
  delay(100);
  totalRead += analogRead(pin);
  delay(100);
  totalRead += analogRead(pin);
  delay(100);
  totalRead += analogRead(pin);
  delay(100);
  totalRead += analogRead(pin);
  delay(100);
  totalRead += analogRead(pin);
  delay(100);
  totalRead += analogRead(pin);
  delay(100);
  totalRead += analogRead(pin);
  delay(100);
  totalRead += analogRead(pin);
  delay(100);
  totalRead += analogRead(pin);
  return (totalRead / 5.0);
}

void forward(){
  // Serial.print("In forward\n");
  analogWrite(motorRightTop,150);
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorLeftTop,150);
  digitalWrite(motorLeftBottom,LOW);
}

void backward(){
  // Serial.print("In backward\n");
  analogWrite(motorRightBottom,150);
  digitalWrite(motorRightTop,LOW);
  analogWrite(motorLeftBottom,150);
  digitalWrite(motorLeftTop,LOW);
}

void turnLeft(){
  // Serial.print("In left\n");
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorRightTop,230);
  digitalWrite(motorLeftBottom,LOW);
  analogWrite(motorLeftTop,100);
}

void turnLeftWide(){
  // Serial.print("In wide left\n");
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorRightTop,230);
  digitalWrite(motorLeftBottom,LOW);
  analogWrite(motorLeftTop,150);
}

void turnRight(){
  // Serial.print("In right\n");
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorRightTop,100);
  digitalWrite(motorLeftBottom,LOW);
  analogWrite(motorLeftTop,230);
}

void turnRightWide(){
  // Serial.print("In wide right\n");
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorRightTop,150);
  digitalWrite(motorLeftBottom,LOW);
  analogWrite(motorLeftTop,230);
}

void rotateLeft(){
  // Serial.print("In in place left\n");
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorRightTop,230);
  analogWrite(motorLeftBottom,100);
  digitalWrite(motorLeftTop,LOW);
}

void rotateRight(){
  // Serial.print("In in place right\n");
  analogWrite(motorRightBottom,100);
  digitalWrite(motorRightTop,LOW);
  digitalWrite(motorLeftBottom,LOW);
  analogWrite(motorLeftTop,100);
}

void reset(){
  // Serial.print("In reset\n");
  digitalWrite(motorLeftBottom,LOW);
  digitalWrite(motorLeftTop,LOW);
  digitalWrite(motorRightBottom,LOW);
  digitalWrite(motorRightTop,LOW);
}
