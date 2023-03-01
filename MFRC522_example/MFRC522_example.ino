#include <MFRC522.h>
#include <SPI.h>

// Declare the pins involved with communicating to the RFID scanner
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("Commencing startup...");

  // Initiate the RFID and the serial data protocol
  // SPI - Serial Peripheral Interface
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Startup complete.");
  Serial.println("Place card next to reader");
}

void loop() {
  delay(300);
  // Check if a) A new card is present on the reader and
  // b) if we can read it's serial code
  if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) { 
    // New card to read!
    Serial.println("User detected! Scanning card...");
    String RFID_code = "";
    for(byte i=0; i<4; i++) {
      // An RFID code consists of 4 hexadecimal numbers - add these to the input
      // Loop through the UID (Unique Identifier) to obtain all the values 
      RFID_code.concat(String(mfrc522.uid.uidByte[i], HEX));

      // Put a space between the numbers for readability
      if(i<3) {RFID_code.concat(" ");}
    }

    RFID_code.toUpperCase();
    Serial.println("RFID Code is: " + RFID_code);

    // Check whether the keycard is authorized
    if(RFID_code == "16 9E 8D 8D") { // Change value here to card reading
      Serial.println("User Authorized");
    } else {
      Serial.println("Access Denied");
    }
  }
}
