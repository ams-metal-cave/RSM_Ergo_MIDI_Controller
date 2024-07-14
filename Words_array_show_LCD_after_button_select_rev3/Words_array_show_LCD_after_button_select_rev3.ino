#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define appName "RSM MIDI Controller"
#define appVersion "v0.1"

#define toggleSwitchPin 10
#define button1Pin 7
#define button2Pin 8

#define serialBaud 115200

#define lcd_Address 0x27
#define lcd_NrCols 20
#define lcd_NrLines 2
#define lcd_FirstCol 0
#define lcd_LastCol lcd_FirstCol + lcd_NrCols - 1
#define lcd_FirstLine 0
#define lcd_LastLine lcd_FirstLine + lcd_NrLines - 1

#define midi_MinChan 1
#define midi_NrChannels 16
#define midi_MaxChan midi_MinChan + midi_NrChannels -1

#define buttonState_Off 1
#define buttonState_On 0
#define buttonState_Unknown 3 //status knop weet je niet als MCU opstart; iemand kan de knop ingedrukt houden bij opstarten en dat geeft dus een verkeerde waarde als je er standaard vanuit gaat, dat hij normally open is

int prevtoggleSwitchState = buttonState_Unknown;
int toggleSwitchState;

unsigned long prevMillies;
unsigned long curMillies;

uint8_t button1State = 0;
uint8_t button2State = 0;

uint8_t arrayIndex = 0;

String words[5] = {"EEN", "TWEE", "DRIE", "VIER", "VIJF"}; 

LiquidCrystal_I2C lcd(lcd_Address, lcd_NrCols, lcd_NrLines);  

void setup() {
  String versionStr;

  pinMode(toggleSwitchPin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  Serial.begin(serialBaud);
 
  lcd.init();
  lcd.backlight();
  lcd.clear();
  printToLCD(lcd_FirstCol,lcd_FirstLine, appName);
//  printToLCD(lcd_FirstCol,lcd_LastLine,"version " + appVersion);
  versionStr = "version ";
  versionStr += appVersion;
  printToLCD(lcd_FirstCol,lcd_LastLine,versionStr);  
  delay(100);
}

void loop() {

  toggleSwitchState = digitalRead(toggleSwitchPin);
  //delay(10);
  if (toggleSwitchState == buttonState_On) {
    if (toggleSwitchState != prevtoggleSwitchState) {
       initToggleOnMode(); 
    } 
    handleToggleOnMode();
  } else {
    if (toggleSwitchState != prevtoggleSwitchState) {
       initToggleOffMode();
    }     
    handleToggleOffMode();
  }
  prevtoggleSwitchState = toggleSwitchState;
}

void initToggleOnMode() {
  Serial.println("Toggle On mode selected");
  lcd.clear();
  //delay(50);
  printToLCD(lcd_FirstCol, lcd_FirstLine, "Toggle switch is ON ");
}

void handleToggleOnMode() {
  //Serial.println("Toggle On mode running");
}

void initToggleOffMode() {
  Serial.println("Toggle Off mode selected");
  lcd.clear();
  //delay(50);  
  printToLCD(lcd_FirstCol, lcd_FirstLine,"Toggle switch is OFF");
}

void printToLCD(uint8_t col, uint8_t line, String text) {
  lcd.setCursor(col, line);  
  lcd.print(text);  
}

void handleToggleOffMode() {
//   Serial.println("Toggle Off mode"); //wordt later midi controller sketch aansturing
}
/*
void loop2() {

 do { 
  toggleSwitchState = digitalRead(toggleSwitchPin);
  if (toggleSwitchState != prevtoggleSwitchState){
    printToLCD(lcd_FirstCol, lcd_FirstLine, "Toggle switch is OFF");
  }
  prevtoggleSwitchState = toggleSwitchState;
  delay(50); }
  while(toggleSwitchState == HIGH); 
    
toggleSwitchState = digitalRead(toggleSwitchPin);
button1State = digitalRead(button1Pin);
delay(50);
lcd.clear();

while(toggleSwitchState == LOW){
  
  printToLCD(lcd_FirstCol, lcd_FirstLine, "Toggle switch is ON ");
  printToLCD(lcd_FirstCol, lcd_LastLine, words[arrayIndex]);  

  printToLCD(12, lcd_LastLine, arrayIndex);
  delay(1000);
  toggleSwitchState = digitalRead(toggleSwitchPin);
  button1State = digitalRead(button1Pin);
       
   while (button1State == LOW){

    if (arrayIndex >-1 && arrayIndex <= 3) {
      lcd.clear();
      printToLCD(lcd_FirstCol, lcd_FirstLine, "toggleSwitch is ON ");
      printToLCD(lcd_FirstCol, lcd_LastLine, words[arrayIndex++]);
      printToLCD(12,lcd_LastLine, arrayIndex);
      //delay(500);
     }
    toggleSwitchState = digitalRead(toggleSwitchPin);
    button1State = digitalRead(button1Pin);
    delay(50);
    if (arrayIndex >= 4) {
      arrayIndex = 0;
      lcd.clear();
      printToLCD(lcd_FirstCol, lcd_FirstLine, "Toggle switch is ON ");
      printToLCD(lcd_FirstCol, lcd_LastLine, words[arrayIndex]);
      printToLCP(12,lcd_LastLine, arrayIndex);
     }
   
  toggleSwitchState = digitalRead(toggleSwitchPin);
  button1State = digitalRead(button1Pin);
  
  }

  if (toggleSwitchState == LOW && button2State == HIGH && arrayIndex >-1 || arrayIndex < 6) {
      lcd.clear();
      delay(50);
      printToLCD(lcd_FirstCol, lcd_FirstLine, "Toggle switch is ON ");
      printToLCD(lcd_FirstCol, lcd_LastLine, words[arrayIndex--]);
      toggleSwitchState = digitalRead(toggleSwitchPin);
    }

  if (toggleSwitchState == LOW && button2State == HIGH && arrayIndex >= 5) {
      arrayIndex = 0;
      lcd.clear();
      delay(50);
      writeToLCD(lcd_FirstCol, lcd_FirstLine, "Toggle switch is ON ");
      writeToLCD(lcd_FirstCol, lcd_LastLine, words[arrayIndex]);
      toggleSwitchState = digitalRead(toggleSwitchPin);
    }

  if (toggleSwitchState == LOW && button2State == HIGH && arrayIndex < 0) {
      arrayIndex = 5;
      lcd.clear();
      delay(50);
      writeToLCD(lcd_FirstCol, lcd_FirstLine, "toggle switch is ON ");
      writeToLCD(lcd_FirstCol, lcd_LastLine, words[arrayIndex]);
      toggleSwitchState = digitalRead(toggleSwitchPin);
    }
 
  //toggleSwitchState = digitalRead(toggleSwitchPin);

  lcd.clear();
  writeToLCD(lcd_FirstCol, lcd_LastLine, "terug start loop");
  delay(1000);
  lcd.clear();
  */
