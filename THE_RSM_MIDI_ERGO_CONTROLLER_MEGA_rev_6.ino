// THE RSM ERGO MIDI COONTROLLER => Gemaakt door R.J. Smits op basis van een Arduino Mega board + eigen DIN midi in/ou/thru interface met I2C LCD 16x2 scherm
// Gemaakt voor gebruik met de DAW van Reaper op basis action template met midi CC's.
// Ergo => ergonomische door plaatsing werkvlak onder een hoek ter compensatie pronatiebeperking en gelijk toepassing USB trackpad (los van de Mega; Mega heeft geen native midi over USB)  

#include <MIDI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27 // I2C-adres van het LCD-scherm
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

MIDI_CREATE_DEFAULT_INSTANCE();
const int channelM = 1; //master fader & track selected buttons
const int channel = 16;

unsigned long previousMillis = 0; // voor timer pauze TFT
unsigned long currentMillis; // voor timer pauze TFT
const long interval = 6000; // 1 minuut in milliseconden voor TFT timer

const int faderPin = A0;
const int panPotentiometerPin = A1;
const int masterVolumePotPin = A2;
int faderValue = 0;
int panValue = 0;
int masterVolumeValue = 0;
int i = 0;
int j;
int k = 0; //counter foor timer scherm leeg maken
int oldFaderValue = 0;
int oldPanValue = 0;
int oldMasterVolumeValue = 0;

// status arrary buttons, ader, pan en MV potmeter om LCD idle status te latern zien
int midiDataSendOut[4] = {0,0,0,0}; // geheugen plaats voor 0 is niet verzonden data in loop; 1 is wel verzonden midi data => idle time trigger

//  index voor knop#     1  2  3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20 
int midiChButtons[20] = {1, 1, 1, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}; //Reaper master fader & track selected buttons CH 1, rest ch 16

int midiChPotmeters[3] = {1, 1, 16};
int buttonPins[20] = {34, 2, 3, 4, 5, 6, 36, 40, 37, 38, 39, 35, 26, 27, 28, 29, 30, 31, 32, 33};

//  index voor knop#       1   2   3   4    5    6    7   8   9    10   11  12   13   14   15   16 17  18  19   20 
int buttonCCValues[20] = {64, 48, 32, 54, 123, 121, 109, 53, 124, 122, 108, 12, 125, 106, 107, 16, 17, 18, 19, 126}; // CC's per knop (van 1-20)
int lastButtonState[20] = {LOW};

int lastSentMIDICode = -1;
unsigned long lastDisplayChangeTime = 0;

// MIDI MMC command codes in sysEx
/*
byte dataSysEx01[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x01, 0xF7 }; // MMC code Stop
byte dataSysEx02[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x02, 0xF7 }; // MMC code Play 
byte dataSysEx03[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x03, 0xF7 }; // MMC code Deferred Play
byte dataSysEx04[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x04, 0xF7 }; // MMC code Fast Forward
byte dataSysEx05[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x05, 0xF7 }; // MMC code Rewind
byte dataSysEx06[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x06, 0xF7 }; // MMC code Record Strobe Punch In
byte dataSysEx07[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x07, 0xF7 }; // MMC code Record Exit Punch Out
byte dataSysEx08[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x08, 0xF7 }; // MMC code Record pause
byte dataSysEx09[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x09, 0xF7 }; // MMC code Pause
byte dataSysEx10[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x0A, 0xF7 }; // MMC code Eject
byte dataSysEx11[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x0B, 0xF7 }; // MMC code Chase
byte dataSysEx12[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x0C, 0xF7 }; // MMC code Command Error Reset
byte dataSysEx13[] = { 0xF0, 0x7F, 0x7F, 0x06, 0x0D, 0xF7 }; // MMC code MMC Rest
*/

void showWelcomeText();

void setup() {
  
lcd.begin(); // LCD-initialisatie
showWelcomeText(); //start tekst laten zien op LCD

MIDI.begin(); // MIDI-begin met standaardinstellingen
for (int i = 0; i < 20; i++) {
pinMode(buttonPins[i], INPUT_PULLUP); // Knoppen aansluiten op de juiste digitale pinnen met voorschakelweerstanden
}

Serial.begin(31250); 
delay (100);
Serial.print("Startwaarde K = ");
Serial.println(k);
}


void loop() {

//k = 0;
//previousMillis = millis();

//MIDI arcade drukknoppen voor midi control CC's
for (int i = 0; i < 20; i++){
  
  int currentButtonState = digitalRead(buttonPins[i]);
      if (currentButtonState != lastButtonState[i]) {
        if (currentButtonState == LOW) {
          lastDisplayChangeTime == millis();
          lastSentMIDICode = buttonCCValues[i];
          MIDI.sendControlChange(lastSentMIDICode, 127, midiChButtons[i]);
          
          digitalWrite(midiDataSendOut[0], 1);
          int temp = digitalRead (midiDataSendOut[0]);      
          Serial.println(temp);

        }
      }  
    lastButtonState[i] = currentButtonState;
  
// Update LCD with last sent MIDI CC value
  lcd.setCursor(12, 0);
  lcd.print("[CC]");

  if (millis() - lastDisplayChangeTime < 1000){
  
   if(lastSentMIDICode >-1 && lastSentMIDICode <10){
    lcd.setCursor(12, 1);  
    lcd.print(lastSentMIDICode);
    lcd.setCursor(13, 1);
    lcd.print("  ");
  }
   if(lastSentMIDICode >9 && lastSentMIDICode <100){ //CC button print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters
    lcd.setCursor(12, 1);
    lcd.print(lastSentMIDICode);
    lcd.setCursor(14, 1);
    lcd.print(" ");
  }
   if(lastSentMIDICode >99){ //CC button print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters
    lcd.setCursor(12, 1);
    lcd.print(lastSentMIDICode);
   }
  }    
  else {
    lcd.setCursor(12, 1);
    lcd.print("---");
  }
  
 //Fader en draaiknoppen instellingen en werking uitzenden midi codes  

  //FADER MIDI CC out & value to LCD 
  int faderValue = analogRead(faderPin);
  int tmpFader = (oldFaderValue - faderValue);
  if(tmpFader >= 8 || tmpFader <= -8) { //fader blijft gelezen worden => probleem in DAW die CC blijft ontvangen => met marge =/-8 stop de CC stroom totdat de echt fader bewogen wordt
    int faderMapped = map(faderValue, 0, 1023, 127, 0); // analog pin waarde van 0-1023 omzetten naar 0 to 127 midi waarde via mapping functie

    MIDI.sendControlChange(120, faderMapped, midiChPotmeters[0]); //Fader potmeter send CC
    
    if(faderMapped >-1 && faderMapped <10){ //Fader potmeter print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters
      lcd.setCursor(0, 0);
      lcd.print("[F]");
      lcd.setCursor(0, 1);
      lcd.print(faderMapped);
      lcd.setCursor(1, 1);
      lcd.print("  ");
    }
    if(faderMapped >9 && faderMapped <100){ //Fader potmeter print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters
      lcd.setCursor(0, 0);
      lcd.print("[F]");
      lcd.setCursor(0, 1);
      lcd.print(faderMapped);
      lcd.setCursor(2, 1);
      lcd.print(" "); 
    }
    else{
      lcd.setCursor(0, 0);
      lcd.print("[F]");
      lcd.setCursor(0, 1); //Fader potmeter print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters
      lcd.print(faderMapped); 
    }
    oldFaderValue = faderValue; //gelezen fader waarde wordt nu oude fader waarde 
  } 
  //PANPOT MIDI CC out & value to LCD 
  int panValue = analogRead(panPotentiometerPin);
  int tmpPan = (oldPanValue - panValue);
  if(tmpPan >= 8 || tmpPan <= -8) { //fader blijft gelezen worden => probleem in DAW die CC blijft ontvangen => met marge =/-8 stop de CC stroom totdat de echt fader bewogen wordt
    int panMapped = map(panValue, 0, 1023, 127, 0);
 
    MIDI.sendControlChange(101, panMapped, midiChPotmeters[1]); //PAN potmeter send CC + print value on LCD
    
    if(panMapped >-1 && panMapped <10){ //Pan potmeter print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters
      lcd.setCursor(4, 0);
      lcd.print("[P]");
      lcd.setCursor(4, 0);
      lcd.print(panMapped);
      lcd.setCursor(5, 1);
      lcd.print("  ");
    }
    if(panMapped >9 && panMapped <100){ //Pan potmeter print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters
      lcd.setCursor(4, 0);
      lcd.print("[P]");
      lcd.setCursor(4, 1);
      lcd.print(panMapped);
      lcd.setCursor(6, 1);
      lcd.print(" "); 
    }
    else{ 
      lcd.setCursor(4, 0);
      lcd.print("[P]");
      lcd.setCursor(4, 1); //Pan potmeter print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters 
      lcd.print(panMapped); 
    }
    oldPanValue = panValue;
  }
  //MASTER VOLUME POT MIDI CC out & value to LCD
  int masterVolumeValue = analogRead(masterVolumePotPin); 
  int tmpMasterVolume = (oldMasterVolumeValue - masterVolumeValue);
  if(tmpMasterVolume >= 8 || tmpMasterVolume <= -8) { //fader blijft gelezen worden => probleem in DAW die CC blijft ontvangen => met marge =/-8 stop de CC stroom totdat de echt fader bewogen wordt
    int masterVolumeMapped = map(masterVolumeValue, 0, 1023, 127, 0);

    MIDI.sendControlChange(52, masterVolumeMapped, midiChPotmeters[2]); //volume potmeter send CC

    if(masterVolumeMapped >-1 && masterVolumeMapped <10){ //Pan potmeter print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters
      lcd.setCursor(8, 0);
      lcd.print("[V]");
      lcd.setCursor(8, 1);
      lcd.print(masterVolumeMapped);
      lcd.setCursor(9, 1);
      lcd.print("  ");
    }
    if(masterVolumeMapped >9 && masterVolumeMapped <100){ //Pan potmeter print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters
      lcd.setCursor(8, 0);
      lcd.print("[V]");
      lcd.setCursor(8, 1);
      lcd.print(masterVolumeMapped);
      lcd.setCursor(10, 1);
      lcd.print(" "); 
    }
    else{ 
      lcd.setCursor(8, 0);
      lcd.print("[V]");
      lcd.setCursor(8, 1); //Pan potmeter print value on LCD waarde <10 (scherm print waarde van links naar rechts) => anders lopen cijfers tientallen en honderdtallen door elkaar ivm links uitlijning LCD karakters 
      lcd.print(masterVolumeMapped); 
    }
    oldMasterVolumeValue = masterVolumeValue;
  }
  delay(10); //voorkomt te veel midi signalen

} 
}

void showWelcomeText() { //opstart tekst op lcd-scherm
  lcd.backlight(); // lcd backlight aan
  lcd.setCursor(0, 0);
  lcd.print("THE RSM ERGO");
  lcd.setCursor(0, 1);
  lcd.print("MIDI CONTROLLER");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Serial#: 0");
  lcd.setCursor(0, 1);
  lcd.print("Rev.: MEGA rev_6");
  delay(1000);
  lcd.clear();
}

/*void tftTimer(){ //tekst op LCD bij geen gebruik
  
  //for (int j = 0; j < 20; j++) {   
      //int currentButtonState = digitalRead(buttonPins[j]);
      //currentButtonState == lastButtonState[j];
      if (digitalRead(buttonPins[i]) == HIGH && currentMillis - previousMillis >= interval) {
          //lcd.setCursor(0, 0);
          //lcd.clear();
          //lcd.setCursor(0, 1);
          //lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("sleep modes");
          lcd.setCursor(0, 1);
          lcd.print("MIDI CONTROLLER");
          delay(500);
          lcd.noDisplay();
      }
      
    

  previousMillis = currentMillis;
  lcd.begin(); // LCD-initialisatie
  }
*/