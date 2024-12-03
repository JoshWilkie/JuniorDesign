#include <ArduinoHttpClient.h>
#include <WiFi.h>

using namespace std;

const int motorLeftBottom = 9; //Pin 9 on L293
const int motorLeftTop = 10; // Pin 15 on L293
const int motorRightBottom = 6; // Pin 2 on L293
const int motorRightTop = 3; // Pin 7 on L293

// TODO: Update pins to actual pins on Arduino
const int leftBluePin = 0;
const int leftRedPin = 0;
const int leftPTPin = 0; // Analog in
const int rightBluePin = 0;
const int rightRedPin = 0;
const int rightPTPin = 0; // Analog in

const int maxPower = 100;
const int midPower = 70;
const int minPower = 40;

const int testingDelay = 1500;

const int battVoltPin = 5; // Analog in

static float blackAmbient = 0.0;
static float redAmbient = 0.0;
static float blueAmbient = 0.0;
static float yellowAmbient = 0.0;

// TODO - Find actual min max values
static float blackBMax = 100.0;
static float blackBMin = 50.0;
static float blackRMax = 100.0;
static float blackRMin = 50.0;
static float redBMax = 100.0;
static float redBMin = 50.0;
static float redRMax = 100.0;
static float redRMin = 50.0;
static float blueBMax = 100.0;
static float blueBMin = 50.0;
static float blueRMax = 100.0;
static float blueRMin = 50.0;
static float yellowBMax = 100.0;
static float yellowBMin = 50.0;
static float yellowRMax = 100.0;
static float yellowRMin = 50.0;

const int IROutputPin = 0;
const int IRInputPin = 0; // Analog in

static float wallThreshold = 100.0;

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

void soloDemo();
void teamDemoRed();
void teamDemoBlue();

void followColor(int targetColorID);

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
  pinMode(LED_BUILTIN, OUTPUT);

  reset();

  Serial.begin(9600);

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
        delay(testingDelay);
        reset();
      }else if(message.substring(strLength - 12) == "backwardSizz"){
        // Serial.print("Calling backward\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling backward");
        // client.endMessage();
        backward();
        delay(testingDelay);
        reset();
      }else if(message.substring(strLength - 8) == "leftSizz"){
        // Serial.print("Calling left\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling left");
        // client.endMessage();
        turnLeft();
        delay(testingDelay);
        reset();
      }else if(message.substring(strLength - 12) == "wideLeftSizz"){
        // Serial.print("Calling wide left\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling wide left");
        // client.endMessage();
        turnLeftWide();
        delay(testingDelay);
        reset();
      }else if(message.substring(strLength - 9) == "rightSizz"){
        // Serial.print("Calling right\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling right");
        // client.endMessage();
        turnRight();
        delay(testingDelay);
        reset();
      }else if(message.substring(strLength - 13) == "wideRightSizz"){
        // Serial.print("Calling wide right\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling wide right");
        // client.endMessage();
        turnRightWide();
        delay(testingDelay);
        reset();
      }else if(message.substring(strLength - 14) == "rotateLeftSizz"){
        // Serial.print("Calling in place left\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling in place left");
        // client.endMessage();
        rotateLeft();
        delay(testingDelay);
        reset();
      }else if(message.substring(strLength - 15) == "rotateRightSizz"){
        // Serial.print("Calling in place right\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling in place right");
        // client.endMessage();
        rotateRight();
        delay(testingDelay);
        reset();
      }else if(message.substring(strLength - 9) == "resetSizz"){
        // Serial.print("Calling reset\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling reset");
        // client.endMessage();
        reset();
      }else if(message.substring(strLength - 15) == "soloDemoSizz"){
        soloDemo();
      }else if(message.substring(strLength - 15) == "teamDemoSizz"){
        teamDemoRed();
      }else if(message.substring(strLength - 15) == "teamDemoSizz"){
        teamDemoBlue();
      }else if(message.substring(strLength - 11) == "ambientSizz"){
        collectAmbient();
      }
    }
  }

  delay(10);
}

void soloDemo(){
  // Go forward until reaching wall
  forward();
  float wallDetectionVal = 0;
  while(wallDetectionVal < wallThreshold){
    wallDetectionVal = analogRead(IRInputPin);
    delay(10);
  }

  // Spin in place then go back towards where we started
  reset();
  delay(200);
  rotateRight();
  delay(1500);
  reset();
  delay(200);
  forward();

  // Go forward until we reach red
  int leftColorID = 0;
  int rightColorID = 0;
  while(rightColorID != 2){
    rightColorID = getColor(1);
  }

  reset();
  delay(200);
  rotateLeft();
  delay(800);
  reset();
  delay(200);

  // Follow red
  followColor(2);

  // Spin in place then go towards yellow line
  reset();
  delay(200);
  rotateLeft();
  delay(800);
  reset();
  delay(200);
  forward();

  // Go forward until we reach yellow
  leftColorID = 0;
  rightColorID = 0;
  while(rightColorID != 4){
    rightColorID = getColor(1);
  }

  reset();
  delay(200);
  rotateLeft();
  delay(800);
  reset();
  delay(200);

  
}

void followColor(int targetColorID){
  // Follow
  while(wallDetectionVal < wallThreshold){
    leftColorID = getColor(0);
    rightColorID = getColor(1);
    // Went too far right
    if(leftColorID = targetColorID){
      turnLeft();
      delay(100);
      forward();
    }
    // Went too far left
    if(rightColorID != targetColorID){
      turnRight();
      delay(100);
      forward();
    }
    wallDetectionVal = analogRead(IRInputPin);
    delay(10);
  }
}

void teamDemoRed(){
  delay(10);
}

void teamDemoBlue(){
  delay(10);
}

int getColor(int side){
  int bluePin = 0;
  int redPin = 0;
  int PTPin = 0;
  if(side == 0){
    bluePin = leftBluePin;
    redPin  = leftRedPin;
    PTPin   = leftPTPin;
  }else{
    bluePin = rightBluePin;
    redPin  = rightRedPin;
    PTPin   = rightPTPin;
  }
  int colorDelay = 10;
  digitalWrite(bluePin, HIGH);
  digitalWrite(redPin, LOW);
  delay(colorDelay);
  float blueReading = analogRead(PTPin);
  delay(colorDelay);
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, HIGH);
  delay(colorDelay);
  float redReading = analogRead(PTPin);
  delay(colorDelay);
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, LOW);

  if((blueReading - blackAmbient) < blackBMax && (blueReading - blackAmbient) > blackBMin && (redReading - blackAmbient) < blackRMax && (redReading - blackAmbient) > blackRMin){
    return 1; // Reads Black
  }else if((blueReading - redAmbient) < redBMax && (blueReading - redAmbient) > redBMin && (redReading - redAmbient) < redRMax && (redReading - redAmbient) > redRMin){
    return 2; // Reads Red
  }else if((blueReading - blueAmbient) < blueBMax && (blueReading - blueAmbient) > blueBMin && (redReading - blueAmbient) < blueRMax && (redReading - blueAmbient) > blueRMin){
    return 3; // Reads Blue
  }else if((blueReading - yellowAmbient) < yellowBMax && (blueReading - yellowAmbient) > yellowBMin && (redReading - yellowAmbient) < yellowRMax && (redReading - yellowAmbient) > yellowRMin){
    return 4; // Reads Yellow
  }
}

void collectAmbient(){
  blink();
  blink();
  digitalWrite(LED_BUILTIN, HIGH);
  blackAmbient = collectLight(leftPTPin);
  Serial.print("Black Ambient Light Read: ");
  Serial.print(blackAmbient);
  Serial.print("\n");
  digitalWrite(LED_BUILTIN, LOW);
  blink();
  digitalWrite(LED_BUILTIN, HIGH);
  redAmbient = collectLight(leftPTPin);
  Serial.print("Red Ambient Light Read: ");
  Serial.print(redAmbient);
  Serial.print("\n");
  digitalWrite(LED_BUILTIN, LOW);
  blink();
  digitalWrite(LED_BUILTIN, HIGH);
  blueAmbient = collectLight(leftPTPin);
  Serial.print("Blue Ambient Light Read: ");
  Serial.print(blueAmbient);
  Serial.print("\n");
  digitalWrite(LED_BUILTIN, LOW);
  blink();
  digitalWrite(LED_BUILTIN, HIGH);
  yellowAmbient = collectLight(leftPTPin);
  Serial.print("Yellow Ambient Light Read: ");
  Serial.print(yellowAmbient);
  Serial.print("\n");
  // digitalWrite(LED_BUILTIN, LOW);
}

void blink(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
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
  return (totalRead / 10.0);
}

void forward(){
  // Serial.print("In forward\n");
  analogWrite(motorRightTop, maxPower);
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorLeftTop, maxPower);
  digitalWrite(motorLeftBottom,LOW);
}

void backward(){
  // Serial.print("In backward\n");
  analogWrite(motorRightBottom, maxPower);
  digitalWrite(motorRightTop,LOW);
  analogWrite(motorLeftBottom, maxPower);
  digitalWrite(motorLeftTop,LOW);
}

void turnLeft(){
  // Serial.print("In left\n");
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorRightTop, maxPower);
  digitalWrite(motorLeftBottom,LOW);
  analogWrite(motorLeftTop, minPower);
}

void turnLeftWide(){
  // Serial.print("In wide left\n");
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorRightTop, maxPower);
  digitalWrite(motorLeftBottom,LOW);
  analogWrite(motorLeftTop, midPower);
}

void turnRight(){
  // Serial.print("In right\n");
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorRightTop, minPower);
  digitalWrite(motorLeftBottom,LOW);
  analogWrite(motorLeftTop, maxPower);
}

void turnRightWide(){
  // Serial.print("In wide right\n");
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorRightTop, midPower);
  digitalWrite(motorLeftBottom,LOW);
  analogWrite(motorLeftTop, maxPower);
}

void rotateLeft(){
  // Serial.print("In in place left\n");
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorRightTop, midPower);
  analogWrite(motorLeftBottom, midPower);
  digitalWrite(motorLeftTop,LOW);
}

void rotateRight(){
  // Serial.print("In in place right\n");
  analogWrite(motorRightBottom, midPower);
  digitalWrite(motorRightTop,LOW);
  digitalWrite(motorLeftBottom,LOW);
  analogWrite(motorLeftTop, midPower);
}

void reset(){
  // Serial.print("In reset\n");
  digitalWrite(motorLeftBottom,LOW);
  digitalWrite(motorLeftTop,LOW);
  digitalWrite(motorRightBottom,LOW);
  digitalWrite(motorRightTop,LOW);
}
