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

const int bluePin = 11; // Analog out
const int redPin = 5;
const int leftPTPin = 3; // Analog in
const int rightPTPin = 4; // Analog in

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
float blackBMaxRight   = 0.00;
float blackBMinRight   = 10000.0;
float blackRMaxRight   = 0.00;
float blackRMinRight   = 10000.0;
float redBMaxRight     = 0.00;
float redBMinRight     = 10000.0;
float redRMaxRight     = 0.00;
float redRMinRight     = 10000.0;
float blueBMaxRight    = 0.00;
float blueBMinRight    = 10000.0;
float blueRMaxRight    = 0.00;
float blueRMinRight    = 10000.0;
float yellowBMaxRight  = 0.00;
float yellowBMinRight  = 10000.0;
float yellowRMaxRight  = 0.00;
float yellowRMinRight  = 10000.0;

float blackBMaxLeft   = 0.00;
float blackBMinLeft   = 10000.0;
float blackRMaxLeft   = 0.00;
float blackRMinLeft   = 10000.0;
float redBMaxLeft     = 0.00;
float redBMinLeft     = 10000.0;
float redRMaxLeft     = 0.00;
float redRMinLeft     = 10000.0;
float blueBMaxLeft    = 0.00;
float blueBMinLeft    = 10000.0;
float blueRMaxLeft    = 0.00;
float blueRMinLeft    = 10000.0;
float yellowBMaxLeft  = 0.00;
float yellowBMinLeft  = 10000.0;
float yellowRMaxLeft  = 0.00;
float yellowRMinLeft  = 10000.0;

// const int IROutputPin = 0;
const int IRInputPin = 0; // Analog in

const float wallThreshold = 500.0;

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

void getColor(int *leftID, int *rightID);
void followColor(int targetColorID);

float collectLight(int pin);
void collectAmbient();

void collectMinMax();
void printMinMax();
void resetColor();

void printToScreen(String text);

void setup() {
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(leftPTPin, INPUT);
  pinMode(rightPTPin, INPUT);
  pinMode(motorLeftBottom, OUTPUT);
  pinMode(motorLeftTop, OUTPUT);
  pinMode(motorRightBottom, OUTPUT);
  pinMode(motorRightTop, OUTPUT);
  // pinMode(battVoltPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);

  reset();

  Serial.begin(9600);

  digitalWrite(LED_BUILTIN, HIGH);

  // WiFi Setup //
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }
  digitalWrite(LED_BUILTIN, LOW);
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
  // analogWrite(5, 171);

  client.beginMessage(TYPE_TEXT);
  client.print("Ready to receive instruction");
  client.endMessage();
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
      }else if(message.endsWith("minMaxSizz")){
        collectMinMax();
      }else if(message.endsWith("printMinMaxSizz")){
        printMinMax();
      }
    }
  }

  delay(100);
  // Serial.print("Still looping\n");
}

void soloDemo(){
  String outText = "Wall Detection Value is: ";
  float wallDetectionVal = 0;
  // Read in and trash first 50 wall IR input readings
  for(int i = 0; i < 50; i++){
    wallDetectionVal = analogRead(IRInputPin);
    delay(10);
    
    client.beginMessage(TYPE_TEXT);
    client.print(outText + wallDetectionVal);
    client.endMessage();
  }
  printToScreen("Going", "Forward", "To Wall");
  // float ambientIR;
  // for(int i = 0; i < 5; i++){
  //   ambientIR += analogRead(IRInputPin);
  //   delay(10);
  // }
  // ambientIR /= 5;

  // outText = outText + wallDetectionVal;
  
  client.beginMessage(TYPE_TEXT);
  client.print(outText + wallDetectionVal);
  client.endMessage();

  // Go forward until reaching wall
  forward();
  // while(wallDetectionVal > wallThreshold){
  //   wallDetectionVal = 0;
  //   for(int i = 0; i < 5; i++){
  //     wallDetectionVal += analogRead(IRInputPin);
  //     delay(10);
  //   }
  //   wallDetectionVal /= 5;
  //   // Serial.print("\nwallDetectionVal is: ");
  //   // Serial.print(wallDetectionVal);

  //   // outText = "Wall Detection Value is: " + wallDetectionVal;
    
  //   client.beginMessage(TYPE_TEXT);
  //   client.print(outText + wallDetectionVal);
  //   client.endMessage();
  // }
  while(wallDetectionVal > wallThreshold){
    wallDetectionVal = analogRead(IRInputPin);
    delay(10);
  }
  reset();

  // Spin in place then go back towards where we started
  printToScreen("Turning", "Around", "");
  delay(100);
  rotateRight();
  // How long we rotate for
  delay(2300);
  reset();
  delay(100);
  printToScreen("Finding", "Red Lane", "");
  forward();

  // delay(4000);
  // reset();

  // Go forward until we reach red
  int leftColorID = 0;
  int rightColorID = 0;
  while(rightColorID != 2){
    getColor(&leftColorID, &rightColorID);
  }
  reset();

  printToScreen("Turning", "Onto Red", "Lane");
  delay(100);
  rotateLeft();
  delay(1200);
  reset();
  delay(100);

  // Follow red
  printToScreen("Following", "Red Lane", "");
  followColor(2);

  // Spin in place then go towards yellow line
  printToScreen("Turning", "To Yellow", "Lane");
  delay(100);
  rotateLeft();
  // TODO: Make sure good turn radius (probably want to overturn)
  delay(1400);
  reset();
  delay(100);
  printToScreen("Finding", "Yellow", "Lane");
  forward();

  // Go forward until we reach yellow
  leftColorID = 0;
  rightColorID = 0;
  while(rightColorID != 4){
    getColor(&leftColorID, &rightColorID);
  }
  reset();

  // Turn towards yellow line
  printToScreen("Turning", "Onto Yel-", "low Lane");
  delay(100);
  rotateLeft();
  delay(1200);
  reset();
  delay(100);

  // Follow yellow
  printToScreen("Following", "Yellow", "Lane");
  followColor(4);
  reset();

  // Turn towards original position
  printToScreen("Turning", "Towards", "Origin");
  delay(100);
  rotateLeft();
  // Tweak turn duration
  delay(1200);
  reset();
  delay(100);
  printToScreen("Returning", "To Origin", "");
  forward();

  wallDetectionVal = analogRead(IRInputPin);
  // Go forward until wall
  while(wallDetectionVal > wallThreshold){
    wallDetectionVal = analogRead(IRInputPin);
    delay(10);
  }

  // Now back where we started
  reset();
  printToScreen("Finished!", "", "    :)");
}

void teamDemoRed(){
  
}

void teamDemoBlue(){
  
}

void getColor(int *leftID, int *rightID){
  int colorDelay = 50;
  // Flash blue on
  analogWrite(bluePin, 172);
  analogWrite(redPin, 0);
  delay(colorDelay);
  // Read blue for right and left
  float leftBlueReading = analogRead(leftPTPin);
  float rightBlueReading = analogRead(rightPTPin);
  delay(colorDelay);
  // Flash red on
  digitalWrite(bluePin, 0);
  digitalWrite(redPin, 172);
  delay(colorDelay);
  // Read red for right and left
  float leftRedReading = analogRead(leftPTPin);
  float rightRedReading = analogRead(rightPTPin);
  delay(colorDelay);
  // Turn off both LEDs
  digitalWrite(bluePin, 0);
  digitalWrite(redPin, 0);

  // Code for checking left color
  if(leftBlueReading < blackBMaxLeft && leftBlueReading > blackBMinLeft && leftRedReading < blackRMaxLeft && leftRedReading > blackRMinLeft){
    *leftID = 1; // Reads Black
  }else if(leftBlueReading < redBMaxLeft && leftBlueReading > redBMinLeft && leftRedReading < redRMaxLeft && leftRedReading > redRMinLeft){
    *leftID = 2; // Reads Red
  }else if(leftBlueReading < blueBMaxLeft && leftBlueReading > blueBMinLeft && leftRedReading < blueRMaxLeft && leftRedReading > blueRMinLeft){
    *leftID = 3; // Reads Blue
  }else if(leftBlueReading < yellowBMaxLeft && leftBlueReading > yellowBMinLeft && leftRedReading < yellowRMaxLeft && leftRedReading > yellowRMinLeft){
    *leftID = 4; // Reads Yellow
  }

  // Code for checking right color
  if(rightBlueReading < blackBMaxRight && rightBlueReading > blackBMinRight && rightRedReading < blackRMaxRight && rightRedReading > blackRMinRight){
    *rightID = 1; // Reads Black
  }else if(rightBlueReading < redBMaxRight && rightBlueReading > redBMinRight && rightRedReading < redRMaxRight && rightRedReading > redRMinRight){
    *rightID = 2; // Reads Red
  }else if(rightBlueReading < blueBMaxRight && rightBlueReading > blueBMinRight && rightRedReading < blueRMaxRight && rightRedReading > blueRMinRight){
    *rightID = 3; // Reads Blue
  }else if(rightBlueReading < yellowBMaxRight && rightBlueReading > yellowBMinRight && rightRedReading < yellowRMaxRight && rightRedReading > yellowRMinRight){
    *rightID = 4; // Reads Yellow
  }
}

// void resetColor(){
//   blackBMaxRight   = 0.00;
//   blackBMinRight   = 10000.0;
//   blackRMaxRight   = 0.00;
//   blackRMinRight   = 10000.0;
//   redBMaxRight     = 0.00;
//   redBMinRight     = 10000.0;
//   redRMaxRight     = 0.00;
//   redRMinRight     = 10000.0;
//   blueBMaxRight    = 0.00;
//   blueBMinRight    = 10000.0;
//   blueRMaxRight    = 0.00;
//   blueRMinRight    = 10000.0;
//   yellowBMaxRight  = 0.00;
//   yellowBMinRight  = 10000.0;
//   yellowRMaxRight  = 0.00;
//   yellowRMinRight  = 10000.0; 
//   blackBMaxLeft   = 0.00;
//   blackBMinLeft   = 10000.0;
//   blackRMaxLeft   = 0.00;
//   blackRMinLeft   = 10000.0;
//   redBMaxLeft     = 0.00;
//   redBMinLeft     = 10000.0;
//   redRMaxLeft     = 0.00;
//   redRMinLeft     = 10000.0;
//   blueBMaxLeft    = 0.00;
//   blueBMinLeft    = 10000.0;
//   blueRMaxLeft    = 0.00;
//   blueRMinLeft    = 10000.0;
//   yellowBMaxLeft  = 0.00;
//   yellowBMinLeft  = 10000.0;
//   yellowRMaxLeft  = 0.00;
//   yellowRMinLeft  = 10000.0;
// }

void collectMinMax(){
  float leftReading = 0;
  float rightReading = 0;

  // Calibrate min/max values for red LED on black surface
  // Turn on red LED both sides
  analogWrite(redPin, 172);
  analogWrite(bluePin, 0);
  delay(250);
  for(int i = 0; i < 100; i++){
    delay(50);
    leftReading = analogRead(leftPTPin);
    rightReading = analogRead(rightPTPin);
    if(leftReading > blackRMaxLeft){
      blackRMaxLeft = leftReading;
    }
    if(leftReading < blackRMinLeft){
      blackRMinLeft = leftReading;
    }
    if(rightReading > blackRMaxRight){
      blackRMaxRight = rightReading;
    }
    if(rightReading < blackRMinRight){
      blackRMinRight = rightReading;
    }
  }

  // Calibrate min/max values for blue LED on black surface
  // Turn on blue LED both sides
  analogWrite(redPin, 0);
  analogWrite(bluePin, 172);
  delay(250);
  for(int i = 0; i < 100; i++){
    delay(50);
    leftReading = analogRead(leftPTPin);
    rightReading = analogRead(rightPTPin);
    if(leftReading > blackBMaxLeft){
      blackBMaxLeft = leftReading;
    }
    if(leftReading < blackBMinLeft){
      blackBMinLeft = leftReading;
    }
    if(rightReading > blackBMaxRight){
      blackBMaxRight = rightReading;
    }
    if(rightReading < blackBMinRight){
      blackBMinRight = rightReading;
    }
  }

  // Turn on both LEDs to signify to change surface color
  analogWrite(redPin, 172);
  analogWrite(bluePin, 172);

  delay(5000);

  // Calibrate min/max values for red LED on red surface
  // Turn on red LED both sides
  analogWrite(redPin, 172);
  analogWrite(bluePin, 0);
  delay(250);
  for(int i = 0; i < 100; i++){
    delay(50);
    leftReading = analogRead(leftPTPin);
    rightReading = analogRead(rightPTPin);
    if(leftReading > redRMaxLeft){
      redRMaxLeft = leftReading;
    }
    if(leftReading < redRMinLeft){
      redRMinLeft = leftReading;
    }
    if(rightReading > redRMaxRight){
      redRMaxRight = rightReading;
    }
    if(rightReading < redRMinRight){
      redRMinRight = rightReading;
    }
  }

  // Calibrate min/max values for blue LED on red surface
  // Turn on blue LED both sides
  analogWrite(redPin, 0);
  analogWrite(bluePin, 172);
  delay(250);
  for(int i = 0; i < 100; i++){
    delay(50);
    leftReading = analogRead(leftPTPin);
    rightReading = analogRead(rightPTPin);
    if(leftReading > redBMaxLeft){
      redBMaxLeft = leftReading;
    }
    if(leftReading < redBMinLeft){
      redBMinLeft = leftReading;
    }
    if(rightReading > redBMaxRight){
      redBMaxRight = rightReading;
    }
    if(rightReading < redBMinRight){
      redBMinRight = rightReading;
    }
  }

  // Turn on both LEDs to signify to change surface color
  analogWrite(redPin, 172);
  analogWrite(bluePin, 172);

  delay(5000);

  // Calibrate min/max values for red LED on blue surface
  // Turn on red LED both sides
  analogWrite(redPin, 172);
  analogWrite(bluePin, 0);
  delay(250);
  for(int i = 0; i < 100; i++){
    delay(50);
    leftReading = analogRead(leftPTPin);
    rightReading = analogRead(rightPTPin);
    if(leftReading > blueRMaxLeft){
      blueRMaxLeft = leftReading;
    }
    if(leftReading < blueRMinLeft){
      blueRMinLeft = leftReading;
    }
    if(rightReading > blueRMaxRight){
      blueRMaxRight = rightReading;
    }
    if(rightReading < blueRMinRight){
      blueRMinRight = rightReading;
    }
  }

  // Calibrate min/max values for blue LED on blue surface
  // Turn on blue LED both sides
  analogWrite(redPin, 0);
  analogWrite(bluePin, 172);
  delay(250);
  for(int i = 0; i < 100; i++){
    delay(50);
    leftReading = analogRead(leftPTPin);
    rightReading = analogRead(rightPTPin);
    if(leftReading > blueBMaxLeft){
      blueBMaxLeft = leftReading;
    }
    if(leftReading < blueBMinLeft){
      blueBMinLeft = leftReading;
    }
    if(rightReading > blueBMaxRight){
      blueBMaxRight = rightReading;
    }
    if(rightReading < blueBMinRight){
      blueBMinRight = rightReading;
    }
  }

  // Turn on both LEDs to signify to change surface color
  analogWrite(redPin, 172);
  analogWrite(bluePin, 172);

  delay(5000);

  // Calibrate min/max values for red LED on yellow surface
  // Turn on red LED both sides
  analogWrite(redPin, 172);
  analogWrite(bluePin, 0);
  delay(250);
  for(int i = 0; i < 100; i++){
    delay(50);
    leftReading = analogRead(leftPTPin);
    rightReading = analogRead(rightPTPin);
    if(leftReading > yellowRMaxLeft){
      yellowRMaxLeft = leftReading;
    }
    if(leftReading < yellowRMinLeft){
      yellowRMinLeft = leftReading;
    }
    if(rightReading > yellowRMaxRight){
      yellowRMaxRight = rightReading;
    }
    if(rightReading < yellowRMinRight){
      yellowRMinRight = rightReading;
    }
  }

  // Calibrate min/max values for blue LED on yellow surface
  // Turn on blue LED both sides
  analogWrite(redPin, 0);
  analogWrite(bluePin, 172);
  delay(250);
  for(int i = 0; i < 100; i++){
    delay(50);
    leftReading = analogRead(leftPTPin);
    rightReading = analogRead(rightPTPin);
    if(leftReading > yellowBMaxLeft){
      yellowBMaxLeft = leftReading;
    }
    if(leftReading < yellowBMinLeft){
      yellowBMinLeft = leftReading;
    }
    if(rightReading > yellowBMaxRight){
      yellowBMaxRight = rightReading;
    }
    if(rightReading < yellowBMinRight){
      yellowBMinRight = rightReading;
    }
  }

  // Turn off both LEDs, we're done here
  analogWrite(redPin, 0);
  analogWrite(bluePin, 0);
}

void printMinMax(){
  String tempText = "Black Surface Left Red LED Max: ";
  String minText = " & Min: ";
  // String avgText = " & Avg: ";
  String rightText = "\n Right LED Max: ";
  String outText = tempText + blackRMaxLeft + minText + blackRMinLeft + rightText + blackRMaxRight + minText + blackRMinRight;

  Serial.print(outText);

  tempText = "\nBlack Surface Left Blue LED Max: ";
  outText = tempText + blackBMaxLeft + minText + blackBMinLeft + rightText + blackBMaxRight + minText + blackBMinRight;

  Serial.print(outText);
  
  tempText = "\nRed Surface Left Red LED Max: ";
  outText = tempText + redRMaxLeft + minText + redRMinLeft + rightText + redRMaxRight + minText + redRMinRight;

  Serial.print(outText);
  
  tempText = "\nRed Surface Left Blue LED Max: ";
  outText = tempText + redBMaxLeft + minText + redBMinLeft + rightText + redBMaxRight + minText + redBMinRight;

  Serial.print(outText);

  tempText = "\nBlue Surface Left Red LED Max: ";
  outText = tempText + blueRMaxLeft + minText + blueRMinLeft + rightText + blueRMaxRight + minText + blueRMinRight;

  Serial.print(outText);

  tempText = "\nBlue Surface Left Blue LED Max: ";
  outText = tempText + blueBMaxLeft + minText + blueBMinLeft + rightText + blueBMaxRight + minText + blueBMinRight;

  Serial.print(outText);

  tempText = "\nYellow Surface Left Blue LED Max: ";
  outText = tempText + yellowBMaxLeft + minText + yellowBMinLeft + rightText + yellowBMaxRight + minText + yellowBMinRight;

  Serial.print(outText);

  tempText = "\nYellow Surface Left Red LED Max: ";
  outText = tempText + yellowRMaxLeft + minText + yellowRMinLeft + rightText + yellowRMaxRight + minText + yellowRMinRight;

  Serial.print(outText);
}

// void minMaxHelper(int color){
//   float *BMaxLeft;
//   float *BMinLeft;
//   float *BMaxRight;
//   float *BMinRight;
//   float *RMaxLeft;
//   float *RMinLeft;
//   float *RMaxRight;
//   float *RMinRight;
//   if(color == 1){
//     BMaxLeft  = &blackBMaxLeft;
//     BMinLeft  = &blackBMinLeft;
//     BMaxRight = &blackBMaxRight;
//     BMinRight = &blackBMinRight;
//     RMaxLeft  = &blackRMaxLeft;
//     RMinLeft  = &blackRMinLeft;
//     RMaxRight = &blackRMaxRight;
//     RMinRight = &blackRMinRight;
//   }else if(color == 2){
//     BMaxLeft  = redBMaxLeft;
//     BMinLeft  = redBMinLeft;
//     BMaxRight = redBMaxRight;
//     BMinRight = redBMinRight;
//     RMaxLeft  = redRMaxLeft;
//     RMinLeft  = redRMinLeft;
//     RMaxRight = redRMaxRight;
//     RMinRight = redRMinRight;
//   }else if (color == 3){
//     BMaxLeft  = blueBMaxLeft;
//     BMinLeft  = blueBMinLeft;
//     BMaxRight = blueBMaxRight;
//     BMinRight = blueBMinRight;
//     RMaxLeft  = blueRMaxLeft;
//     RMinLeft  = blueRMinLeft;
//     RMaxRight = blueRMaxRight;
//     RMinRight = blueRMinRight;
//   }else{
//     BMaxLeft  = yellowBMaxLeft;
//     BMinLeft  = yellowBMinLeft;
//     BMaxRight = yellowBMaxRight;
//     BMinRight = yellowBMinRight;
//     RMaxLeft  = yellowRMaxLeft;
//     RMinLeft  = yellowRMinLeft;
//     RMaxRight = yellowRMaxRight;
//     RMinRight = yellowRMinRight;
//   }
//   analogWrite(redPin, 172);
//   analogWrite(bluePin, 0);
//   delay(250);
//   for(int i = 0; i < 100; i++){
//     delay(50);
//     leftReading = analogRead(leftPTPin);
//     rightReading = analogRead(rightPTPin);
//     if(leftReading > blackRMaxLeft){
//       blackRMaxLeft = leftReading;
//     }
//     if(leftReading < blackRMinLeft){
//       blackRMinLeft = leftReading;
//     }
//     if(rightReading > blackRMaxRight){
//       blackRMaxRight = rightReading;
//     }
//     if(rightReading < blackRMinRight){
//       blackRMinRight = rightReading;
//     }
//   }

//   // Calibrate min/max values for blue LED on black surface
//   // Turn on blue LED both sides
//   analogWrite(redPin, 0);
//   analogWrite(bluePin, 172);
//   delay(250);
//   for(int i = 0; i < 100; i++){
//     delay(50);
//     leftReading = analogRead(leftPTPin);
//     rightReading = analogRead(rightPTPin);
//     if(leftReading > blackBMaxLeft){
//       blackBMaxLeft = leftReading;
//     }
//     if(leftReading < blackBMinLeft){
//       blackBMinLeft = leftReading;
//     }
//     if(rightReading > blackBMaxRight){
//       blackBMaxRight = rightReading;
//     }
//     if(rightReading < blackBMinRight){
//       blackBMinRight = rightReading;
//     }
//   }

//   // Turn on both LEDs to signify to change surface color
//   analogWrite(redPin, 172);
//   analogWrite(bluePin, 172);

//   delay(5000);
// }

// TODO: Update followColor to use logic on which side is black and which side is on color, turn accordingly with color updates
void followColor(int targetColorID){
  // Follow
  float wallDetectionVal = analogRead(IRInputPin);
  int leftColorID = 0;
  int rightColorID = 0;
  while(wallDetectionVal > wallThreshold){
    getColor(&leftColorID, &rightColorID);
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

// void collectAmbient(){
//   blink();
//   blink();
//   digitalWrite(LED_BUILTIN, HIGH);
//   blackAmbient = collectLight(leftPTPin);
//   Serial.print("Black Ambient Light Read: ");
//   Serial.print(blackAmbient);
//   Serial.print("\n");
//   digitalWrite(LED_BUILTIN, LOW);
//   blink();
//   digitalWrite(LED_BUILTIN, HIGH);
//   redAmbient = collectLight(leftPTPin);
//   Serial.print("Red Ambient Light Read: ");
//   Serial.print(redAmbient);
//   Serial.print("\n");
//   digitalWrite(LED_BUILTIN, LOW);
//   blink();
//   digitalWrite(LED_BUILTIN, HIGH);
//   blueAmbient = collectLight(leftPTPin);
//   Serial.print("Blue Ambient Light Read: ");
//   Serial.print(blueAmbient);
//   Serial.print("\n");
//   digitalWrite(LED_BUILTIN, LOW);
//   blink();
//   digitalWrite(LED_BUILTIN, HIGH);
//   yellowAmbient = collectLight(leftPTPin);
//   Serial.print("Yellow Ambient Light Read: ");
//   Serial.print(yellowAmbient);
//   Serial.print("\n");
//   // digitalWrite(LED_BUILTIN, LOW);
// }

// void blink(){
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(300);
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(300);
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(300);
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(300);
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(300);
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(300);
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(300);
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(300);
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(300);
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(300);
// }

// float collectLight(int pin){
//   int totalRead = 0;
//   totalRead += analogRead(pin);
//   delay(100);
//   totalRead += analogRead(pin);
//   delay(100);
//   totalRead += analogRead(pin);
//   delay(100);
//   totalRead += analogRead(pin);
//   delay(100);
//   totalRead += analogRead(pin);
//   delay(100);
//   totalRead += analogRead(pin);
//   delay(100);
//   totalRead += analogRead(pin);
//   delay(100);
//   totalRead += analogRead(pin);
//   delay(100);
//   totalRead += analogRead(pin);
//   delay(100);
//   totalRead += analogRead(pin);
//   return (totalRead / 10.0);
// }

void forward(){
  // Serial.print("In forward\n");
  analogWrite(motorRightTop, midPower);
  digitalWrite(motorRightBottom,LOW);
  analogWrite(motorLeftTop, midPower);
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
