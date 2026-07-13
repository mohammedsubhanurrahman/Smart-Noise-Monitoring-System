#include <LiquidCrystal.h>

// LCD connections: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Pin definitions
const int SOUND_PIN = A0;   // Sound sensor analog output
const int POT_PIN = A1;     // External Potentiometer for threshold
const int RED_LED = 8;
const int GREEN_LED = 9;
const int BUZZER = 10;

// Timing and logic variables
unsigned long noiseStartTime = 0;   // Tracks when voice noise first started
int threshold = 300;               // Variable to store dynamic threshold

void setup() {
  // Set pin modes
  pinMode(SOUND_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Voice Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Ready...");
  delay(2000);
  lcd.clear();
}

// Function to estimate signal frequency using the zero-crossing method
float getFrequency() {
  unsigned long startTime = micros();
  int count = 0;
  int lastState = 0;
  int currentVal;
  
  // Sample the sound wave for 20 milliseconds (1 full loop cycle)
  while (micros() - startTime < 20000) {
    currentVal = analogRead(SOUND_PIN);
    // Determine state relative to the typical midpoint voltage of the sensor
    int state = (currentVal > 512) ? 1 : 0; 
    
    if (state != lastState) {
      count++;
      lastState = state;
    }
  }
  // Formula: Frequency = (Wave Crossings / 2) / Time (0.02 seconds)
  return (count / 2.0) / 0.02; 
}

void loop() {
  // 1. Read Inputs
  int soundValue = analogRead(SOUND_PIN);
  int potValue = analogRead(POT_PIN);
  
  // Map the pot values to a sensible amplitude threshold range
  threshold = map(potValue, 0, 1023, 100, 800);
  
  // Analyze current pitch frequency
  float freq = getFrequency();

  // 2. Evaluate Voice Condition
  // Logic: Must be LOUD (above threshold) AND voice pitch (300Hz - 3000Hz)
  if (soundValue > threshold && freq >= 300.0 && freq <= 3000.0) {
    
    // Start the grace-period timer if this is the first instant of talking
    if (noiseStartTime == 0) {
      noiseStartTime = millis(); 
    }
    
    unsigned long noiseDuration = millis() - noiseStartTime;

    if (noiseDuration < 3000) {
      // --- CONDITION 1: 3-Second Visual Warning Phase ---
      lcd.setCursor(0, 0); 
      lcd.print("VOICE DETECTED! ");
      lcd.setCursor(0, 1);
      lcd.print("Please shhh...  ");

      // Non-blocking flash: toggles every 250ms
      if ((millis() / 250) % 2 == 0) {
        digitalWrite(RED_LED, HIGH);
      } else {
        digitalWrite(RED_LED, LOW);
      }
      
      digitalWrite(GREEN_LED, LOW);
      noTone(BUZZER); // Keep buzzer silent during grace period
    } 
    else {
      // --- CONDITION 2: Sustained Loud Noise Alarm Phase ---
      lcd.setCursor(0, 0); 
      lcd.print("TOO LOUD!!!     ");
      lcd.setCursor(0, 1);
      lcd.print("BUZZER ACTIVE!  ");

      digitalWrite(RED_LED, HIGH);   // Solid Red
      digitalWrite(GREEN_LED, LOW);
      tone(BUZZER, 1000);            // 1kHz loud tone alert
    }
  } 
  else {
    // --- CONDITION 3: Quiet Environment / Ambient Noises ---
    noiseStartTime = 0; // Reset the talking timer completely
    
    lcd.setCursor(0, 0); 
    lcd.print("Please be Silent");
    
    // Display tracking telemetry for debugging/tuning
    lcd.setCursor(0, 1); 
    lcd.print("Th:"); 
    lcd.print(threshold); 
    lcd.print(" Hz:"); 
    lcd.print((int)freq); 
    lcd.print("    "); // Spaces clear trailing characters
    
    digitalWrite(GREEN_LED, HIGH);  // Solid Green indicating safe level
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);
  }

  delay(10); // Ultra-short buffer for ADC stabilization
}