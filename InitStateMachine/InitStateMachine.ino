#define BUTTON_PIN 3 // GREY WIRE
#define KILLSWITCH_PIN 2 // WHITE WIRE

const int motorPin1 = 5; //Pin 7 on L293
const int motorPin2 = 6; // Pin 2 on L293
const int motorPin3 = 10; // Pin 15 on L293
const int motorPin4 = 9; // Pin 10 on L293

volatile int currState = 0;
volatile bool justChanged = false;

// Debouncing variables
volatile unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 250;  // 250 milliseconds debounce time

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

void setup() {
  pinMode(KILLSWITCH_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), cycleState, RISING);
  attachInterrupt(digitalPinToInterrupt(KILLSWITCH_PIN), kill, RISING);
  Serial.begin(9600);
}

void loop() {
  // Serial.print("Cycle Button Read: ");
  // Serial.print(digitalRead(BUTTON_PIN));
  // Serial.print("\n");
  // Serial.print("Kill Button Read: ");
  // Serial.print(digitalRead(KILLSWITCH_PIN));
  // Serial.print("\n");
  // Serial.print("Curr state: ");
  // Serial.print(currState);
  // Serial.print("\n");
  // delay(800);
  // Serial.print("\n");

  if((currState < 0) or (currState > 8)){
    // Serial.print("Out of range");
    currState = 1;
  }

  if(currState == 1){
    // Serial.print("Calling forward\n");
    forward();
  }else if(currState == 2){
    // Serial.print("Calling backward\n");
    backward();
  }else if(currState == 3){
    // Serial.print("Calling left\n");
    rotate_left();
  }else if(currState == 4){
    // Serial.print("Calling wide left\n");
    rotate_wide_left();
  }else if(currState == 5){
    // Serial.print("Calling right\n");
    rotate_right();
  }else if(currState == 6){
    // Serial.print("Calling wide right\n");
    rotate_wide_right();
  }else if(currState == 7){
    // Serial.print("Calling in place left\n");
    rotate_in_place_left();
  }else if(currState == 8){
    // Serial.print("Calling in place right\n");
    rotate_in_place_right();
  }else{
    // Serial.print("Outside range of acceptable states, calling reset\n");
    // Serial.print("Curr state: ");
    // Serial.print(currState);
    // Serial.print("\n");
    reset();
  }

  if(!digitalRead(BUTTON_PIN)){
    unsigned long currentTime = millis();
    if(justChanged){
      Serial.print("Button just read low\n");
    }
    if ((currentTime - lastDebounceTime) > debounceDelay) {
      // Only update change if debounce delay has passed
      if(justChanged){
        Serial.print("Updated just changed\n");
      }
      lastDebounceTime = currentTime;
      justChanged = false;
    }
  }
}

void cycleState() {
  // Serial.print("In cycleState\n");
  if(!justChanged){
    currState++;
    justChanged = true;
    Serial.print("Just changed state to: ");
    Serial.print(currState);
    Serial.print("\n");
  }
}

void kill(){
  Serial.print("In kill\n");
  Serial.print("Kill Button Read: ");
  Serial.print(digitalRead(KILLSWITCH_PIN));
  Serial.print("\n");
  currState = 0;
  reset();
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
