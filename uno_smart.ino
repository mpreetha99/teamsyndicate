#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

#define SS_PIN 10//Arduino Mega, set 10 for Arduino Uno/Nano
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);// Create MFRC522 instance.
char message_buff[100];

SoftwareSerial sw(2, 3); // RX, TX
void setup()
{
  Serial.begin(115200);// Initialize serial communications with the PC
  sw.begin(115200);
  SPI.begin();// Init SPI bus
  mfrc522.PCD_Init();// Init MFRC522 card
  Serial.println("Initialiased RFID reader");

}

void loop()
{  
 
  
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
  sw.print(rfidUid);
  sw.println(rfidUid);
  Serial.println("");
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
