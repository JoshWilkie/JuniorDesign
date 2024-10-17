const int motorPin1 = 5; //Pin 7 on L293
const int motorPin2 = 6; // Pin 2 on L293
const int motorPin3 = 10; // Pin 15 on L293
const int motorPin4 = 9; // Pin 10 on L293

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


// initialize digital pins for motors as an output.
void forward();
void backward();
void rotate_left();
void rotate_wide_left();
void rotate_right();
void rotate_wide_right();
void rotate_in_place_left();
void rotate_in_place_right();
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
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  Serial.begin(9600);
  collectAmbient();
  
}

void loop() {

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
  digitalWrite(signifierLEDPin, LOW);
  blink();
  digitalWrite(signifierLEDPin, HIGH);
  leftBlueAmbient = collectLight(leftPTPin);
  rightBlueAmbient = collectLight(rightPTPin);
  digitalWrite(signifierLEDPin, LOW);
  blink();
  digitalWrite(signifierLEDPin, HIGH);
  leftBlueAmbient = collectLight(leftPTPin);
  rightBlueAmbient = collectLight(rightPTPin);
  digitalWrite(signifierLEDPin, LOW);
  blink();
  digitalWrite(signifierLEDPin, HIGH);
  leftYellowAmbient = collectLight(leftPTPin);
  rightYellowAmbient = collectLight(rightPTPin);
  digitalWrite(signifierLEDPin, LOW);
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
  analogWrite(motorPin4,150);
  digitalWrite(motorPin3,LOW);
  analogWrite(motorPin2,150);
  digitalWrite(motorPin1,LOW);
}

void backward(){
  // Serial.print("In backward\n");
  analogWrite(motorPin3,150);
  digitalWrite(motorPin4,LOW);
  analogWrite(motorPin1,150);
  digitalWrite(motorPin2,LOW);
}

void rotate_left(){
  // Serial.print("In left\n");
  digitalWrite(motorPin3,LOW);
  analogWrite(motorPin4,230);
  digitalWrite(motorPin1,LOW);
  analogWrite(motorPin2,100);
}

void rotate_wide_left(){
  // Serial.print("In wide left\n");
  digitalWrite(motorPin3,LOW);
  analogWrite(motorPin4,230);
  digitalWrite(motorPin1,LOW);
  analogWrite(motorPin2,150);
}

void rotate_right(){
  // Serial.print("In right\n");
  digitalWrite(motorPin3,LOW);
  analogWrite(motorPin4,100);
  digitalWrite(motorPin1,LOW);
  analogWrite(motorPin2,230);
}

void rotate_wide_right(){
  // Serial.print("In wide right\n");
  digitalWrite(motorPin3,LOW);
  analogWrite(motorPin4,150);
  digitalWrite(motorPin1,LOW);
  analogWrite(motorPin2,230);
}

void rotate_in_place_left(){
  // Serial.print("In in place left\n");
  digitalWrite(motorPin3,LOW);
  analogWrite(motorPin4,230);
  analogWrite(motorPin1,100);
  digitalWrite(motorPin2,LOW);
}

void rotate_in_place_right(){
  // Serial.print("In in place right\n");
  analogWrite(motorPin3,100);
  digitalWrite(motorPin4,LOW);
  digitalWrite(motorPin1,LOW);
  analogWrite(motorPin2,100);
}

void reset(){
  // Serial.print("In reset\n");
  digitalWrite(motorPin1,LOW);
  digitalWrite(motorPin2,LOW);
  digitalWrite(motorPin3,LOW);
  digitalWrite(motorPin4,LOW);
}
