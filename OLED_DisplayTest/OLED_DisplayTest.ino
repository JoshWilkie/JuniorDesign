#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Serial.begin(9600);
  
  // initialize the OLED object
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
  delay(5000);
  display.clearDisplay();

  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Bot is at State");
  display.println("");

  display.setCursor(60, 16);
  display.setTextSize(4);
  display.println("1");

  display.setTextSize(1);
  display.println("");
  display.println("Moving Forward!");
  display.display();
  display.startscrollright(0x00, 0x00);
}

void loop() {
}