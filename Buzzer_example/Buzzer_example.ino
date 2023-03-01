#define RED_PIN 6
#define YELLOW_PIN 5
#define GREEN_PIN 4
#define BUZZ_PIN 3
#define BUTTON_PIN  2


void setup() {
  Serial.begin(9600);
  Serial.println("Commencing startup...");

  // Declare the Buzzer as an output, the Button as an input
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  // Declare the LEDs as outputs
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // Start with only the green light on
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);

  Serial.println("Startup complete.");
}

int button_state = 0;
bool primed = false; // Turn the alarm system on or off

void loop() {
  // Check whether the button is being pressed
  button_state = digitalRead(BUTTON_PIN);

  if(!primed) { // The alarm is off
    if(button_state == HIGH) {
      Serial.println("Turning the alarm on...");
      primed = true;
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
    }
    // Create vibration in the buzzer
    digitalWrite(BUZZ_PIN, HIGH);
    delay(10);
    digitalWrite(BUZZ_PIN, LOW);
    delay(5);
  } else { // The alarm is ON
    if(button_state == LOW) {
      Serial.println("Turning the alarm off...");
      primed = false;
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(YELLOW_PIN, LOW);
    }
  }
}
