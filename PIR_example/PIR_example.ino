#define PIR_PIN 7
#define RED_PIN 6
#define YELLOW_PIN 5
#define GREEN_PIN 4


void setup() {
  Serial.begin(9600);
  Serial.println("Commencing startup...");
  
  // Declare the LEDs as Outputs
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  // Declare the motion sensor as an input
  pinMode(PIR_PIN, INPUT);

  // Show that the motion sensor is running - leave the yellow LED on
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);

  Serial.println("Startup complete.");
  Serial.println("Searching for movement...");
}

// Variable to store the motion sensor data - HIGH means motion detected, LOW means no movement
int pir_state = 0;
// Variable to remember whether the alarm has been triggered
bool alarm = false;

// Variables to remember whether the system is searching for movement, and how long ago
bool search_movement = false;
unsigned long time_stamp = 0;

void loop() {
  // Read the current value of the PIR
  pir_state = digitalRead(PIR_PIN);
  if(alarm == true) {
    if(pir_state == LOW) {
      // No more movement found
      alarm = false;
      search_movement = false;
      Serial.println("Alarm disengaged.");

      // Set the light to yellow again, turn the others off
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
    }
  } 
  else if(pir_state == HIGH && search_movement == true && millis()-time_stamp > 3000) {
    alarm = true;
    // A movement has been detected for 3s, start alarm by turning red
    Serial.println("Intruder found!");
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
  } 
  else if(pir_state == LOW && search_movement == true && millis()-time_stamp > 300) {
    // After waiting a bit, no more movement found. Disable alarm
    Serial.println("False alarm. Disengaging alert.");
    search_movement = false;
  } 
  else if(pir_state == HIGH && search_movement == false) {
    // Some movement has been found. Wait 3s and check again to verify
    Serial.println("Movement detected. Verifying...");
    search_movement = true;
    // millis gives the current time in milliseconds, for accurate time measurement
    time_stamp = millis();
  }
}
