
#include <LiquidCrystal.h>

// LCD connections: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Pin definitions
const int SOUND_PIN = A0;
const int POT_PIN = A1;
const int RED_LED = 8;
const int BUZZER = 10;

// Noise monitoring settings
int threshold = 50;
int soundLevel = 0;

// Timing variables
unsigned long noiseStartTime = 0;
const unsigned long WARNING_TIME = 3000;

// Noise detection state
bool noiseActive = false;

// Sampling window
const unsigned long SAMPLE_WINDOW = 50;

// Function to measure sound signal variation
int getSoundLevel() {
  int minValue = 1023;
  int maxValue = 0;

  unsigned long startTime = millis();

  while (millis() - startTime < SAMPLE_WINDOW) {
    int sample = analogRead(SOUND_PIN);

    if (sample < minValue) {
      minValue = sample;
    }

    if (sample > maxValue) {
      maxValue = sample;
    }
  }

  return maxValue - minValue;
}

// Function to display a message on LCD
void displayMessage(const char* line1, const char* line2) {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print(line1);

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void setup() {
  pinMode(SOUND_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.begin(16, 2);

  Serial.begin(9600);

  digitalWrite(RED_LED, LOW);
  noTone(BUZZER);

  displayMessage("SMART NOISE", "MONITOR SYSTEM");

  delay(2000);
  lcd.clear();
}

void loop() {

  // 1. Read sound sensor
  soundLevel = getSoundLevel();

  // 2. Read external potentiometer
  int potValue = analogRead(POT_PIN);

  // Adjust noise threshold
  threshold = map(potValue, 0, 1023, 10, 300);

  // 3. Detect excessive noise
  if (!noiseActive && soundLevel >= threshold) {
    noiseActive = true;
    noiseStartTime = millis();
  }

  // Reset when sound falls sufficiently below threshold
  if (noiseActive && soundLevel < (threshold * 0.8)) {
    noiseActive = false;
    noiseStartTime = 0;
  }

  // 4. Calculate noise duration
  unsigned long noiseDuration = 0;

  if (noiseActive) {
    noiseDuration = millis() - noiseStartTime;
  }

  // 5. Normal environment
  if (!noiseActive) {

    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);

    char line2[17];
    snprintf(line2, sizeof(line2), "Lvl:%d Th:%d",
             soundLevel, threshold);

    displayMessage("NOISE: NORMAL", line2);
  }

  // 6. Warning phase (first 3 seconds)
  else if (noiseDuration < WARNING_TIME) {

    displayMessage("NOISE DETECTED!", "PLEASE BE QUIET");

    // Blink red LED every 250 milliseconds
    if ((millis() / 250) % 2 == 0) {
      digitalWrite(RED_LED, HIGH);
    } else {
      digitalWrite(RED_LED, LOW);
    }

    noTone(BUZZER);
  }

  // 7. Excessive noise alarm
  else {

    displayMessage("NOISE TOO HIGH!", "BUZZER ACTIVE");

    digitalWrite(RED_LED, HIGH);

    tone(BUZZER, 1000);
  }

  // 8. Serial monitoring
  Serial.print("Sound Level: ");
  Serial.print(soundLevel);

  Serial.print(" | Threshold: ");
  Serial.print(threshold);

  Serial.print(" | Noise Duration: ");
  Serial.println(noiseDuration);

}
