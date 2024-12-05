#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

// const int battVoltPin = 5; // Analog in

float blackAmbient = 0.0;
float redAmbient = 0.0;
float blueAmbient = 0.0;
float yellowAmbient = 0.0;

// TODO - Find actual min max values
float blackBMax = 100.0;
float blackBMin = 50.0;
float blackRMax = 100.0;
float blackRMin = 50.0;
float redBMax = 100.0;
float redBMin = 50.0;
float redRMax = 100.0;
float redRMin = 50.0;
float blueBMax = 100.0;
float blueBMin = 50.0;
float blueRMax = 100.0;
float blueRMin = 50.0;
float yellowBMax = 100.0;
float yellowBMin = 50.0;
float yellowRMax = 100.0;
float yellowRMin = 50.0;

const int IROutputPin = 0;
const int IRInputPin = 5; // Analog in

const float wallThreshold = 800.0;

// Websocket Settings //
char ssid[] = "tufts_eecs";
char pass[] = "foundedin1883";

char serverAddress[] = "34.28.153.91";  // server address
int port = 80;

WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = "828BD9E1B7C7"; //Insert your Server ID Here!
int status = WL_IDLE_STATUS;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

void printToScreen(String text);

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
  // pinMode(battVoltPin, INPUT);
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

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer.
  display.clearDisplay();

  // Display Text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(32,28);
  display.println("Hello World!");
  display.display();
  analogWrite(5, 171);
}

void loop() {
  int messageSize = client.parseMessage();
  if (messageSize > 0) {
    String message = client.readString();
    if(message.endsWith("Sizz")){
      Serial.println("Received a message:");
      Serial.println(message);
      if(message.endsWith("forwardSizz")){
        // Serial.print("Calling forward\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling forward");
        // client.endMessage();
        forward();
        delay(testingDelay);
        reset();
      }else if(message.endsWith("backwardSizz")){
        // Serial.print("Calling backward\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling backward");
        // client.endMessage();
        backward();
        delay(testingDelay);
        reset();
      }else if(message.endsWith("leftSizz")){
        // Serial.print("Calling left\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling left");
        // client.endMessage();
        turnLeft();
        delay(testingDelay);
        reset();
      }else if(message.endsWith("wideLeftSizz")){
        // Serial.print("Calling wide left\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling wide left");
        // client.endMessage();
        turnLeftWide();
        delay(testingDelay);
        reset();
      }else if(message.endsWith("rightSizz")){
        // Serial.print("Calling right\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling right");
        // client.endMessage();
        turnRight();
        delay(testingDelay);
        reset();
      }else if(message.endsWith("wideRightSizz")){
        // Serial.print("Calling wide right\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling wide right");
        // client.endMessage();
        turnRightWide();
        delay(testingDelay);
        reset();
      }else if(message.endsWith("rotateLeftSizz")){
        // Serial.print("Calling in place left\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling in place left");
        // client.endMessage();
        rotateLeft();
        delay(testingDelay);
        reset();
      }else if(message.endsWith("rotateRightSizz")){
        // Serial.print("Calling in place right\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling in place right");
        // client.endMessage();
        rotateRight();
        delay(testingDelay);
        reset();
      }else if(message.endsWith("resetSizz")){
        // Serial.print("Calling reset\n");
        // client.beginMessage(TYPE_TEXT);
        // client.print("Calling reset");
        // client.endMessage();
        reset();
      }else if(message.endsWith("soloDemoSizz")){
        soloDemo();
      }else if(message.endsWith("redDemoSizz")){
        teamDemoRed();
      }else if(message.endsWith("blueDemoSizz")){
        teamDemoBlue();
      }else if(message.endsWith("ambientSizz")){
        collectAmbient();
      }else if(message.endsWith("displayTestSizz")){
        printToScreen("Waiting", "For", "Companion");
      // }else if(message.endsWith("IRTestSizz")){
      //   while(true){
      //     delay(250);
      //     float irRead;
      //     irRead = analogRead(IRInputPin);
      //     Serial.print("\nRead in IR value: ");
      //     Serial.print(irRead);
      //   }
      }
    }
  }

  delay(100);
  Serial.print("Still looping\n");
}

void soloDemo(){
  // Go forward until reaching wall
  float wallDetectionVal = 0;
  for(int i = 0; i < 10; i++){
    wallDetectionVal = analogRead(IRInputPin);
  }
  printToScreen("Going", "Forward", "To Wall");
  forward();
  while(wallDetectionVal < wallThreshold){
    wallDetectionVal = analogRead(IRInputPin);
    delay(10);
  }
  reset();

  // Spin in place then go back towards where we started
  printToScreen("Turning", "Around", "");
  delay(200);
  rotateRight();
  delay(1500);
  reset();
  delay(200);
  printToScreen("Finding", "Red Lane", "");
  forward();

  // Go forward until we reach red
  int leftColorID = 0;
  int rightColorID = 0;
  while(rightColorID != 2){
    rightColorID = getColor(1);
  }
  reset();

  printToScreen("Turning", "Onto Red", "Lane");
  delay(200);
  rotateLeft();
  delay(800);
  reset();
  delay(200);

  // Follow red
  printToScreen("Following", "Red Lane", "");
  followColor(2);

  // Spin in place then go towards yellow line
  printToScreen("Turning", "To Yellow", "Lane");
  delay(200);
  rotateLeft();
  delay(800);
  reset();
  delay(200);
  printToScreen("Finding", "Yellow", "Lane");
  forward();

  // Go forward until we reach yellow
  leftColorID = 0;
  rightColorID = 0;
  while(rightColorID != 4){
    rightColorID = getColor(1);
  }
  reset();

  // Turn towards yellow line
  printToScreen("Turning", "Onto Yel-", "low Lane");
  delay(200);
  rotateLeft();
  delay(800);
  reset();
  delay(200);

  // Follow yellow
  printToScreen("Following", "Yellow", "Lane");
  followColor(4);
  reset();

  // Turn towards original position
  printToScreen("Turning", "Towards", "Origin");
  delay(200);
  rotateLeft();
  delay(800);
  reset();
  delay(200);
  printToScreen("Returning", "To Origin", "");
  forward();

  wallDetectionVal = analogRead(IRInputPin);
  // Go forward until wall
  while(wallDetectionVal < wallThreshold){
    wallDetectionVal = analogRead(IRInputPin);
    delay(10);
  }

  // Now back where we started
  reset();
  printToScreen("Finished!", "", "    :)");
}

void teamDemoRed(){
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

  // Send message to companion bot that we found our line
  client.beginMessage(TYPE_TEXT);
  client.print("foundRedSizz");
  client.endMessage();

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

  // Turn towards yellow line
  reset();
  delay(200);
  rotateLeft();
  delay(800);
  reset();
  delay(200);

  String message = "";
  while(!message.endsWith("foundBlueSizz")){
    int messageSize = client.parseMessage();
    if (messageSize > 0) {
      String message = client.readString();
    }
  }

  client.beginMessage(TYPE_TEXT);
  client.print("acknowledgedSizz");
  client.endMessage();

  // Follow yellow
  followColor(4);

  // Turn towards original position
  reset();
  delay(200);
  rotateLeft();
  delay(800);
  reset();
  delay(200);
  forward();

  wallDetectionVal = analogRead(IRInputPin);
  // Go forward until wall
  while(wallDetectionVal < wallThreshold){
    wallDetectionVal = analogRead(IRInputPin);
    delay(10);
  }

  // Now back where we started
  reset();

  client.beginMessage(TYPE_TEXT);
  client.print("bot1ReturnedSizz");
  client.endMessage();

  while(!message.endsWith("bot2ReturnedSizz")){
    int messageSize = client.parseMessage();
    if (messageSize > 0) {
      String message = client.readString();
    }
  }

  client.beginMessage(TYPE_TEXT);
  client.print("acknowledgedSizz");
  client.endMessage();
}

void teamDemoBlue(){
  // Wait for companion bot to find red
  String message = "";
  while(!message.endsWith("foundRedSizz")){
    int messageSize = client.parseMessage();
    if (messageSize > 0) {
      String message = client.readString();
    }
  }

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
  rotateLeft();
  delay(1500);
  reset();
  delay(200);
  forward();

  // Go forward until we reach blue
  int leftColorID = 0;
  int rightColorID = 0;
  while(leftColorID != 3){
    leftColorID = getColor(1);
  }

  reset();
  delay(200);
  rotateRight();
  delay(800);
  reset();
  delay(200);

  // Send message to companion bot that we found our line
  client.beginMessage(TYPE_TEXT);
  client.print("foundBlueSizz");
  client.endMessage();

  // Follow blue
  followColor(3);

  // Spin in place then go towards yellow line
  reset();
  delay(200);
  rotateRight();
  delay(800);
  reset();
  delay(200);
  forward();

  // Go forward until we reach yellow
  leftColorID = 0;
  rightColorID = 0;
  while(leftColorID != 4){
    leftColorID = getColor(1);
  }

  // Turn towards yellow line
  reset();
  delay(200);
  rotateRight();
  delay(800);
  reset();
  delay(200);

  while(!message.endsWith("bot1ReturnedSizz")){
    int messageSize = client.parseMessage();
    if (messageSize > 0) {
      String message = client.readString();
    }
  }

  // Follow yellow
  followColor(4);

  // Turn towards original position
  reset();
  delay(200);
  rotateRight();
  delay(800);
  reset();
  delay(200);
  forward();

  wallDetectionVal = analogRead(IRInputPin);
  // Go forward until wall
  while(wallDetectionVal < wallThreshold){
    wallDetectionVal = analogRead(IRInputPin);
    delay(10);
  }

  // Now back where we started
  reset();

  client.beginMessage(TYPE_TEXT);
  client.print("bot2ReturnedSizz");
  client.endMessage();
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
  int colorDelay = 12;
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

void followColor(int targetColorID){
  // Follow
  float wallDetectionVal = analogRead(IRInputPin);
  while(wallDetectionVal < wallThreshold){
    int leftColorID = getColor(0);
    int rightColorID = getColor(1);
    // Went too far right
    if(leftColorID != targetColorID){
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
  }
  reset();
}

void printToScreen(String text1, String text2, String text3){
  display.clearDisplay();

  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("Bot State:");
  display.println("");

  display.setCursor(0, 16);
  display.println(text1);
  display.setCursor(0, 32);
  display.println(text2);
  display.setCursor(0, 48);
  display.println(text3);

  // display.setTextSize(1);
  // display.println("");
  // display.println("Moving Forward!");
  display.display();
  // display.startscrollright(0x10, 0x20);
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
