#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "pitches.h"

const int sensorpin = A1;
const int sensorpower = 8;
const int pumppin = 11;
const int buzzer = 7;

// delay time between sensor readings (milliseconds)
const int delayTime = 1000;

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int sensor;

int wet = 500;
int dry = 350;
int outputSensorValue;
bool stopWatering;

bool stopWotering;

LiquidCrystal_I2C lcd(0x27,  16, 2);

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(sensorpower,OUTPUT);
  pinMode(pumppin,OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");

  happySong();

  Serial.begin(9600);
}

void loop() {
  
  // power on sensor and wait briefly
  digitalWrite(sensorpower,HIGH);
  delay(10);
  sensor = analogRead(sensorpin);
  outputSensorValue = map(sensor,0,610,0,100);
  // turn sensor off to help prevent corrosion
  digitalWrite(sensorpower,LOW);

  // lcd refresh rate
  delay(1000);

  // Update LCD only if the value has changed
  static int lastOutputSensorValue = -1;
  if (outputSensorValue != lastOutputSensorValue) {
    lcd.setCursor(10, 1);
    lcd.print(outputSensorValue);
    lcd.print("%");
    lastOutputSensorValue = outputSensorValue;
  }

 // Check moisture level and water the plant if necessary
  if (outputSensorValue < 40) {
    lcd.setCursor(0, 1);
    lcd.print(":/");
    stopWatering = false;
    digitalWrite(pumppin, HIGH);
    playWateringMelody();
  } else if (outputSensorValue >= 80) {
    lcd.setCursor(0, 1);
    lcd.print(":)");
    stopWatering = true;
    digitalWrite(pumppin, LOW);
  }

  // wait before taking next reading
  delay(delayTime);  
}

void playWateringMelody() {
  int wateringMelody[] = {
    NOTE_C4, NOTE_G3, NOTE_E3, NOTE_A3, NOTE_B3, NOTE_D4
  };

  int noteDurations[] = {
    4, 8, 8, 4, 4, 4
  };

  // Play the custom watering melody
  for (int i = 0; i < 6; i++) {
    int noteDuration = 1000 / noteDurations[i];
    tone(buzzer, wateringMelody[i], noteDuration);
    delay(noteDuration * 1.30);
    noTone(buzzer);
    delay(50); // Add a small delay between notes
  }
}


void happySong(){
  // iterate over the notes of the melody:
 
  for (int thisNote = 0; thisNote < 8; thisNote++) {
 
    // to calculate the note duration, take one second divided by the note type.
 
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
 
    int noteDuration = 1000 / noteDurations[thisNote];
 
    tone(buzzer, melody[thisNote], noteDuration);
 
    // to distinguish the notes, set a minimum time between them.
 
    // the note's duration + 30% seems to work well:
 
    int pauseBetweenNotes = noteDuration * 1.30;
 
    delay(pauseBetweenNotes);
 
    // stop the tone playing:
 
    noTone(buzzer);
 
  }
}
