//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __|
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/
//
// The Unnamed Circuit
//
// Made by Daniel Dmitrzak
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/4675022-the-unnamed-circuit

#define ANGERY_PUDELKO

#ifdef ANGERY_PUDELKO
#include <Adafruit_NeoPixel.h>
#define DIODE 6
#define WS_PIN 11
#define WS_COLOR 0xFF0000
#define WS_BRIGHTNESS 255
#endif 

#define F_CPU 8000000

#define LAMP_1 12
#define LAMP_2 11
#define LAMP_3 10
#define LAMP_4 9
#define VOLTMETER 5

#define PUSH 14
#define SELECTOR 17
#define POWERLED 19
#define POWERSW 18

#define MAIN_DELAY 6
#define SECONDARY_DELAY 0 
#define TELEPORT_DELAY 1000

#define STEP 4
#define DIR 3
#define DISK 7
#define LED 2

unsigned int counterCurrent = 0;
unsigned int counterDelta = 1;

void setup() {
  // Lampy and shit
  pinMode(PUSH, INPUT_PULLUP);
  pinMode(POWERSW, INPUT_PULLUP);
  pinMode(SELECTOR, INPUT_PULLUP);
  pinMode(POWERLED, OUTPUT);
  pinMode(LAMP_1, OUTPUT);
  pinMode(LAMP_2, OUTPUT);
  pinMode(LAMP_3, OUTPUT);
  pinMode(LAMP_4, OUTPUT);
  pinMode(VOLTMETER, OUTPUT);
  // FDD
  pinMode(STEP, OUTPUT);
  pinMode(DIR,  OUTPUT);
  pinMode(DISK, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  // FFD Setup
  digitalWrite(LED, HIGH);
  digitalWrite(DIR, HIGH);
  tone(STEP, 1000, 1000);

  // Początek programu
  // 
  //while(digitalRead(POWERSW));
  digitalWrite(POWERLED, HIGH);

  // Na fkon 2020 - pudelko zawirusowane, czerwone lampki na diodach WS1228cośtamB
  #ifdef ANGERY_PUDELKO
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(DIODE, WS_PIN, NEO_GRB + NEO_KHZ800);
  pixels.setPixelColor(0, WS_COLOR);
  pixels.setPixelColor(1, WS_COLOR);
  pixels.setPixelColor(2, WS_COLOR);
  pixels.setPixelColor(3, WS_COLOR);
  while(1) pixels.show();
  #endif
  // Ładowanie
  for(counterCurrent = 0; counterCurrent <= 765; counterCurrent += counterDelta)
  {
    analogWrite(VOLTMETER, counterCurrent / 3);
    analogWrite(LAMP_1, constrain(counterCurrent, 0, 255));
    if(counterCurrent > 255) analogWrite(LAMP_2, constrain(counterCurrent - 255, 0, 255));
    if(counterCurrent > 510) analogWrite(LAMP_3, constrain(counterCurrent - 510, 0, 255));
    delay(MAIN_DELAY);
  }
  // Oczekiwanie na naciśnięcie przycisku
  while(digitalRead(PUSH) || digitalRead(SELECTOR))
  {
    analogWrite(VOLTMETER, random(200, 255));
  }
  // Teleportacja
  for(uint8_t i = 0; i < 255; i++)
  {
    analogWrite(LAMP_4, i);
    delay(SECONDARY_DELAY);  
  }
  delay(TELEPORT_DELAY);
  // Rozładowanie
  for(uint8_t i = 765; i > 0; i--){
    analogWrite(VOLTMETER, i / 3);
    analogWrite(LAMP_1, constrain(i, 0, 255));
    if(counterCurrent > 255) analogWrite(LAMP_2, constrain(i - 255, 0, 255));
    if(counterCurrent > 510) analogWrite(LAMP_3, constrain(i - 510, 0, 255));
  }
  digitalWrite(POWERLED, LOW);
  analogWrite(LAMP_4, LOW);
  digitalWrite(LAMP_1, LOW);
}

void loop() {
  if(!digitalRead(DISK)){
    //digitalWrite(MOTOR, LOW); 
    tone(STEP, 1000);  
    for(uint8_t i = 0; i < 10; i++){
      if(digitalRead(DISK)) break;
      digitalWrite(DIR, LOW); 
      digitalWrite(LED, LOW);
      delay(random(100,400));
      
      if(digitalRead(DISK)) break;
      digitalWrite(DIR, HIGH); 
      digitalWrite(LED, HIGH);
      delay(random(100,400));
    }
    noTone(STEP);
    digitalWrite(LED, HIGH);
    while(!digitalRead(DISK));
    delay(500);
  }
}
