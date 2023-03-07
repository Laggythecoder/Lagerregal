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
#define LED_PIN 0     // LED connected to GPIO2

// define the LoRa frequency
#define LORA_FREQ 866E6

// define the LoRa spreading factor
#define LORA_SF   12

int counter = 0;
int prev_counter = 0;
bool led_on = false;
int node_id = 3; // set a unique ID for each board

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for Serial Monitor to open


  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

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


    if (received_node_id == node_id) { // compare received message with node_id
      digitalWrite(LED_PIN, HIGH); 
      led_on = true;
    } else if (led_on) {
      digitalWrite(LED_PIN, LOW);
      led_on = false;
    }
  }
   
  
}