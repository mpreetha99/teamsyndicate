
#include <SPI.h>
#include <MFRC522.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>


// Update these with values suitable for your network.

const char* ssid = "AMMUANJU";
const char* password = "bolt123456";
const char* mqtt_server = "192.168.43.185";

WiFiClient espClient;
PubSubClient client(espClient);


#define SS_PIN 2//Arduino Mega, set 10 for Arduino Uno/Nano
#define RST_PIN 0

MFRC522 mfrc522(SS_PIN, RST_PIN);// Create MFRC522 instance.
char message_buff[100];

// Update these with values suitable for your network.
byte mac[] = { 0xDE, 0xED, 0xAA, 0xFE, 0xFE, 0xED };
//byte ip[] = { 10, 0, 0, 177 }; //Static IP
byte server[] = { 10, 0, 0, 10 }; // MQTT Broker
//const int buttonPinOn = 2; // the number of the pushbutton pin
// variables will change:
//int buttonOnState = 0; // variable for reading the pushbutton status

//setup Wifi
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//reconnect
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "reconnect");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);


// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}


void setup()
{
  Serial.begin(115200);// Initialize serial communications with the PC
  SPI.begin();// Init SPI bus
  mfrc522.PCD_Init();// Init MFRC522 card
  Serial.println("Initialiased RFID reader");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  

}


void loop()
{  
  client.loop();
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())return;
  Serial.print("Card UID:");//Dump UID
  String rfidUid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    rfidUid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    rfidUid += String(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.println(rfidUid);
  Serial.println("");
  rfidUid.toCharArray(message_buff, rfidUid.length() + 1);
  client.publish("uidno", message_buff);

  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();// Stop encryption on PCD
}
