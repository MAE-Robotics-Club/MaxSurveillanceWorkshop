#include <MFRC522.h>
#include <SPI.h>

// Declare the pins used by the RFID Scanner
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Declare the I/O pins
#define PIR_PIN 7
#define RED_PIN 6
#define YELLOW_PIN 5
#define GREEN_PIN 4
#define BUZZ_PIN 3
#define BUTTON_PIN 2


void setup() {
  // Start the Serial monitor for debugging
  Serial.begin(9600);
  Serial.println("Commencing startup...");
  
  // Initiate the RFID scanner and the method of communicating with it
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Declare all the inputs and outputs (LEDs, Buzzer, & Button)
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  // Start with only the green light on
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);

  Serial.println("Startup complete.");
}

int pir_state = 0;
bool primed = false; // Turn the alarm system on or off
bool alarm = false; // Whether the alarm should be triggered and running right now

bool search_movement = false; // If alarm detects movement, wait a bit before activating alarm
unsigned long time_stamp = 0; 

int button_state = 0; // Store the input from button press

String RFID_code; // To store the RFID code of a scanned card

void loop() {
  if(!primed) {
    button_state = digitalRead(BUTTON_PIN);
    if(button_state == HIGH) {
      Serial.println("Turning the alarm on...");
      primed = true;
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
    }
  } else { // The alarm is ON

    // Check for movement and act upon it
    pir_state = digitalRead(PIR_PIN);
    if(alarm == true) { 
      // There's an alarm! Ring the bells
      digitalWrite(BUZZ_PIN, HIGH);
      delay(2);
      digitalWrite(BUZZ_PIN, LOW);
      delay(2);
    } else if(pir_state == HIGH && search_movement == true && millis()-time_stamp > 3000) {
      // Movement has been detected 1s ago and there is still movement
      // Also start the alarm
      alarm = true;
      Serial.println("Intruder detected! Engaging alarm sequence...");
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);

      digitalWrite(BUZZ_PIN, HIGH);
      delay(2);
      digitalWrite(BUZZ_PIN, LOW);
      delay(2);
    } else if(pir_state == LOW && search_movement == true && millis()-time_stamp > 3000) {
      // False alarm - after waiting a second there's no longer any movement
      Serial.println("No more movement found. Disengaging...");
      search_movement = false;
    } else if(pir_state == HIGH && search_movement == false) {
      // Movement has been detected for the first time - start the wait
      Serial.println("Movement detected. Verifying...");
      search_movement = true;
      time_stamp = millis();
    }

    // Check for card scans & authentication 
    if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) { // A new card is available to read!
      Serial.println("User detected! Scanning card...");
      RFID_code = "";
      for(byte i=0; i<4; i++) {
        RFID_code.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.println("RFID Code is: " + RFID_code);
      if(RFID_code == "169e8d8d") { // Change the string in "" to the one specific to your RFID card
        // The person is authenticated, turn off the alarm
        Serial.println("User Authenticated. Turning off alarm...");
        primed = false;
        alarm = false;
        search_movement = false;
        digitalWrite(RED_PIN, LOW);
        digitalWrite(YELLOW_PIN, LOW);
        digitalWrite(GREEN_PIN, HIGH);
      } else {
        Serial.println("User not recognized. Please try again.");
      }
    }
  }
}
