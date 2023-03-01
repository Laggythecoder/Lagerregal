

#include <Wire.h>
#include <SSD1306Wire.h>
#include <SPI.h>
#include <LoRa.h>

// define the pins for LoRa module
#define SCK     5
#define MISO    19
#define MOSI    27
#define SS      18
#define RST     14
#define DIO0    26
#define SDA_PIN 21
#define SCL_PIN 22
#define BUTTON_GREEN 25
#define BUTTON_RED 12


// define the LoRa frequency
#define LORA_FREQ 866E6

// define the LoRa spreading factor
#define LORA_SF   12

//delay between packets
unsigned long previousMillis = 0;  // variable to store the previous time
unsigned long interval = 1000;     // interval at which to send LoRa message

SSD1306Wire display(0x3c, SDA_PIN, SCL_PIN);

int counter = 0;
int buttonGreenState = HIGH;
int buttonRedState = HIGH;
int lastButtonGreenState = HIGH;
int lastButtonRedState = HIGH;
unsigned long lastDebounceTimeGreen = 0;
unsigned long lastDebounceTimeRed = 0;
unsigned long debounceDelay = 50;

// define the Lagerort values in an array
String Lagerort[] = {" ", "1A", "2B", "3C", "4D", "11E", "16F", "7G", "8H", "9I", "10J"};

// define the Menge values in an array
String Menge[] = {" ", "10", "20", "5", "3", "4", "5", "2", "1", "8", "20"};

void setup() {
  // initialize OLED display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.clear();

  // initialize the LoRa module
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(LORA_FREQ)) {
    display.drawString(64, 10, "LoRa init failed");
    display.display();
    while (1);
  }
  LoRa.setSpreadingFactor(LORA_SF);

  // Set transmit power to 20dBm = 100mw
  LoRa.setTxPower(20);

  // Set Signal Bandwith
  LoRa.setSignalBandwidth(250000);

  // initialize button pins
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_RED, INPUT_PULLUP);

  // initialize counter
  counter = 0;
}

void loop() {
  // read the state of the green button
  int readingGreen = digitalRead(BUTTON_GREEN);
  if (readingGreen != lastButtonGreenState) {
    lastDebounceTimeGreen = millis();
  }
  if ((millis() - lastDebounceTimeGreen) > debounceDelay) {
    if (readingGreen != buttonGreenState) {
      buttonGreenState = readingGreen;
      if (buttonGreenState == LOW) {
        counter++;
      }
    }
  }
  lastButtonGreenState = readingGreen;

  // read the state of the red button
  int readingRed = digitalRead(BUTTON_RED);
  if (readingRed != lastButtonRedState) {
    lastDebounceTimeRed = millis();
  }
  if ((millis() - lastDebounceTimeRed) > debounceDelay) {
    if (readingRed != buttonRedState) {
      buttonRedState = readingRed;
      if (buttonRedState == LOW) {
        counter--;
      }
    }
  }
  lastButtonRedState = readingRed;

  // limit the counter to be between 0 and 10
  if (counter < 0) {
    counter = 0;
  } else if (counter > 11) {
    counter = 11;
  }

   // display the Artikel and Lagerort on OLED
  display.clear();
  if (counter == 0) {
    display.drawString(64, 8, "Wegführung");
    display.drawString(64, 28, "starten");
  } 
  else if (counter == 11) {
    display.drawString(64, 8, "Wegführung");
    display.drawString(64, 28, "beendet");
  } else {
    display.drawString(64, 8, "Artikel: " + String(counter));
    display.drawString(64, 28, "Lagerort: " + Lagerort[counter]);
    display.drawString(64, 48, "Menge: " + Menge[counter]);
  }
  display.display();

 // send LoRa message with the counter value to all the V1.0 boards at the specified interval
  unsigned long currentMillis = millis();  // get the current time
  if (currentMillis - previousMillis >= interval) {
    LoRa.beginPacket();
    LoRa.write(counter);
    LoRa.endPacket();
    previousMillis = currentMillis;  // save the current time
  }
    }
  

