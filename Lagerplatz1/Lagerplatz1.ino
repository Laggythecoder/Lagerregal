#include <SPI.h>
#include <LoRa.h>


#define SCLK     5     // GPIO5  -- SX1278's SCK
#define MISO    19    // GPIO19 -- SX1278's MISO
#define MOSI    27    // GPIO27 -- SX1278's MOSI
#define SS      18    // GPIO18 -- SX1278's CS
#define RST     14    // GPIO14 -- SX1278's RESET
#define DIO0    26    // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define SDA_PIN 21
#define SCL_PIN 22
#define LED_PIN1 17     // LED connected to GPIO17 Blau
#define LED_PIN2 16     // LED connected to GPIO16 Grün
#define LED_PIN3 0     // LED connected to GPIO018 Rot

//RGB_PWM
//#define Led0Pin 17
//#define Led1Pin 16
//#define Led2Pin 18

#define Off 255
#define Half 126
#define Full 0

// define the LoRa frequency
#define LORA_FREQ 866E6

// define the LoRa spreading factor
#define LORA_SF   12

// PWM channel 0 parameter
//const int freq = 5000; // 5000 Hz
//const int ledChannel0 = 0;
//const int ledChannel1 = 1;
//const int ledChannel2 = 2;
//const int resolution = 8; // 8-bit resolution

int counter = 0;
int prev_counter = 0;
bool led1_on = false;
bool led2_on = false;
int node_id = 3; // set a unique ID for each board
unsigned long last_led_toggle_time = 0;

void setup() {
  
 
  Serial.begin(115200);
  while (!Serial); // wait for Serial Monitor to open


  pinMode(LED_PIN1, OUTPUT);
  digitalWrite(LED_PIN1, HIGH);

  pinMode(LED_PIN2, OUTPUT);
  digitalWrite(LED_PIN2, HIGH);

  pinMode(LED_PIN3, OUTPUT);
  digitalWrite(LED_PIN3, HIGH);

  // Initialize LoRa module
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Set transmit power to 20dBm = 100mw
  LoRa.setTxPower(20);

  // Set Signal Bandwith
  LoRa.setSignalBandwidth(250000);

  // set spreading factor and bandwidth
  LoRa.setSpreadingFactor(LORA_SF);
 

  LoRa.enableCrc();
  Serial.println("LoRa initialized!");

    //RGB_PWM Setup
      // Configure the channel 0
    //ledcSetup(ledChannel0, freq, resolution);
    //ledcSetup(ledChannel1, freq, resolution);
    //ledcSetup(ledChannel2, freq, resolution);

    // Attach the channel 0 on the 3 pins
    //ledcAttachPin(Led0Pin, ledChannel0);
    //ledcAttachPin(Led1Pin, ledChannel1);
    //ledcAttachPin(Led2Pin, ledChannel2);

    //ledcWrite(ledChannel0, Off);
    //ledcWrite(ledChannel1, Off);
    //ledcWrite(ledChannel2, Off);
}



void loop() {
  // see if a packet was received
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // if a packet size is defined, a packet was received
    Serial.println("");
    Serial.println("Received packet!");
    // read the packet
    String messageA = "";
    String messageB = "";
    String message1 = messageA += (int)LoRa.read();
    String message2 = messageB += (int)LoRa.read();
    
    // print the Packet and RSSI
    Serial.println("counter: " + message1);
    Serial.println("userId: " + message2);
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());
    // check if message is for this node

    int received_node_id = message1.toInt(); // convert received message to integer
    int received_user_id = message2.toInt();

// switch case block


// if block
if (received_user_id == 2) {
  if (received_node_id == node_id) {
    //digitalWrite(LED_PIN1, LOW);
   // digitalWrite(LED_PIN3, LOW);
    //ledcWrite(ledChannel0, Full);
    //ledcWrite(ledChannel1, Full);
    led1_on = true;
  } else {
    //digitalWrite(LED_PIN1, HIGH);
    //digitalWrite(LED_PIN3, HIGH);
    //ledcWrite(ledChannel0, Off);
    //ledcWrite(ledChannel1, Off);
    led1_on = false;
  }
}

if (received_user_id == 1) {
  if (received_node_id == node_id) {
    //digitalWrite(LED_PIN1, LOW);
    //digitalWrite(LED_PIN2, LOW);
  //ledcWrite(ledChannel1, Full);
  //ledcWrite(ledChannel2, Full);
    led2_on = true;
  } else {
    //digitalWrite(LED_PIN1, HIGH);
    //digitalWrite(LED_PIN2, HIGH);
    //ledcWrite(ledChannel1, Off);
    //ledcWrite(ledChannel2, Off);
    led2_on = false;
  }
}

if (led1_on && led2_on) {
  unsigned long current_time = millis();
  if (current_time - last_led_toggle_time >= 200) {
    last_led_toggle_time = current_time;
    digitalWrite(LED_PIN2, !digitalRead(LED_PIN2));
    digitalWrite(LED_PIN3, !digitalRead(LED_PIN3));
  }
}

if (!led1_on && led2_on){
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, HIGH);
  
}
if (led1_on && !led2_on){
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, HIGH);
    digitalWrite(LED_PIN3, LOW);
}
}
}
