//Green wire is 5V, white is GND (for LED)

#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#define buttonInput 2 
#define ledTestPin 13
#define randPin A0
#define numTracks 5 //SWITCH DEPENDING ON NUM OF AUDIO FILES RECIEVED

#define BREAKOUT_RESET  9     
#define BREAKOUT_CS     10     
#define BREAKOUT_DCS    8      

#define CARDCS 4    
#define DREQ 3       

Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

int ledTestState = HIGH;
int buttonState;
int lastButtonState = LOW; //start the button unpressed

unsigned long previousDebounceT = 0;
unsigned long debounceDelay = 50; //measured in milliseconds
unsigned long randNumber;

char track[] = "track000.mp3";

void setup() {

  Serial.begin(9600);
  Serial.println("Boot-up successfull");
    
  pinMode(buttonInput, INPUT);
  pinMode(ledTestPin, OUTPUT);

  digitalWrite(ledTestPin, ledTestState);

  musicPlayerBoot();

  randomSeed(analogRead(randPin));
}

void loop() {
  
  int reading = digitalRead(buttonInput);

  if (reading != lastButtonState) {
    previousDebounceT = millis();
  }

  if ((millis() - previousDebounceT) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) { 
        ledTestState = !ledTestState;
        playMusic();
      }
    }
  }

  digitalWrite(ledTestPin, ledTestState);

  lastButtonState = reading;
}

void musicPlayerBoot() {
  
  if (! musicPlayer.begin()) { // initialize the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  
  musicPlayer.setVolume(5,5); //lower the number, the higher the volume
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
}

void playMusic() {
  
  randNumber = random(1, (numTracks + 1)); //need +1 since random is exclusive
  String number;
  number += randNumber;
  char numBuf[3];
  number.toCharArray(numBuf, 3);
  
  if (randNumber < 10) {
   track[6] = '0'; 
   track[7] = numBuf[0];
  }
  else {
   track[6] = '1';
   track[7] = numBuf[1];
  }
  musicPlayer.playFullFile(track); 
}
