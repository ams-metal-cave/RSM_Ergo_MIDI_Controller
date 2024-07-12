#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int toggleSwitchPin = 10;

const int button1Pin = 7;
const int button2Pin = 8;

const serialBaud = 9600;

const int buttonState_Off = 1;
const int buttonState_On = 0;
const int buttonState_Unknown = 3; //status knop weet je niet als MCU opstart; iemand kan de knop ingedrukt houden bij opstarten en dat geeft dus een verkeerde waarde als je er standaard vanuit gaat, dat hij normally open is

const int lcd_NrCols = 20;
const int lcd_NrLines = 4;
const int lcd_Address = 0x27;
const int lcd_FirstCol = 0;
const int lcd_LastCol = lcd_FirstCol + lcd_NrCols - 1;
const int lcd_FirstRow = 0;
const int lcd_LastRow = lcd_FirstRow + lcd_NrRows - 1;

const midi_MinChan = 1;
const midi_MaxChan = 16;
const midi_NrChannels = 16;

int prevtoggleSwitchState = buttonState_Unknown;
int toggleSwitchState;

int prevMillies;
int curMillies;

int button1State = 0;
int button2State = 0;

int arrayIndex = 0;

String words[5] = {"EEN", "TWEE", "DRIE", "VIER", "VIJF"};

void setup() {
  pinMode(toggleSwitchPin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  Serial.begin(serialBaud);
  LiquidCrystal_I2C lcd(lcd_Address, lcd_NrCols, lcd_NrLines);  
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(lcd_FirstCol,lcd_FirstLine);
  lcd.print("M coder was here");
  lcd.setCursor(lcd_FirstCol,lcd_FirstLine + 2);
  lcd.print("R was looking");
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
  lcd.clear();
  Serial.println("Toggle On mode");
  //delay(50);
  lcd.setCursor(lcd_FirstCol, lcd_FirstLine);
  lcd.print("toggleSwitch is ON");
}

void handleToggleOnMode() {
  //Serial.println("Toggle On mode");
}

void initToggleOffMode() {
  lcd.clear();
  Serial.println("Toggle Off mode");
  lcd.setCursor(lcd_FirstCol, lcd_FirstLine);  
  lcd.print("toggleSwitch is OFF");  
}

void handleToggleOffMode() {
//   Serial.println("Toggle Off mode"); //wordt later midi controller sketch aansturing
}
/*
void loop2() {

 do { 
  toggleSwitchState = digitalRead(toggleSwitchPin);
  if (toggleSwitchState != prevtoggleSwitchState){
    lcd.setCursor(lcd_FirstCol, lcd_FirstLine);
    lcd.print("toggleSwitch is OFF");

  }
  prevtoggleSwitchState = toggleSwitchState;
  delay(50); }
  while(toggleSwitchState == HIGH); 
    
toggleSwitchState = digitalRead(toggleSwitchPin);
button1State = digitalRead(button1Pin);
delay(50);
lcd.clear();

while(toggleSwitchState == LOW){
  
  lcd.setCursor(lcd_FirstCol, lcd_FirstLine);
  lcd.print("toggleSwitch is ON");
  lcd.setCursor(lcd_FirstCol, lcd_FirstLine + 1);
  lcd.print(words[arrayIndex]);  
  lcd.setCursor(12,lcd_LastLine);
  lcd.print(arrayIndex);
  delay(1000);
  toggleSwitchState = digitalRead(toggleSwitchPin);
  button1State = digitalRead(button1Pin);
       
   while (button1State == LOW){

    if (arrayIndex >-1 && arrayIndex <= 3) {
      lcd.clear();
      lcd.setCursor(lcd_FirstCol, lcd_FirstLine);
      lcd.print("toggleSwitch is ON");
      lcd.setCursor(lcd_FirstCol, lcd_LastLine);
      lcd.print(words[arrayIndex++]);
      lcd.setCursor(12,lcd_FirstLine + 1);
      lcd.print(arrayIndex);
      //delay(500);
     }
    toggleSwitchState = digitalRead(toggleSwitchPin);
    button1State = digitalRead(button1Pin);
    delay(50);
    if (arrayIndex >= 4) {
      arrayIndex = 0;
      lcd.clear();
      lcd.setCursor(lcd_FirstCol, lcd_FirstLine);
      lcd.print("toggleSwitch is ON");
      lcd.setCursor(lcd_FirstCol, lcd_LastLine);
      lcd.print(words[arrayIndex]);
      lcd.setCursor(12,1);
      lcd.print(arrayIndex);
     }
   
  toggleSwitchState = digitalRead(toggleSwitchPin);
  button1State = digitalRead(button1Pin);
  
  }

  if (toggleSwitchState == LOW && button2State == HIGH && arrayIndex >-1 || arrayIndex < 6) {
      lcd.clear();
      delay(50);
      lcd.setCursor(lcd_FirstCol, lcd_FirstLine);
      lcd.print("toggleSwitch is ON");
      lcd.setCursor(lcd_FirstCol, lcd_LastLine);
      lcd.print(words[arrayIndex--]);
      toggleSwitchState = digitalRead(toggleSwitchPin);
    }

  if (toggleSwitchState == LOW && button2State == HIGH && arrayIndex >= 5) {
      arrayIndex = 0;
      lcd.clear();
      delay(50);
      lcd.setCursor(lcd_FirstCol, lcd_FirstLine);
      lcd.print("toggleSwitch is ON");      
      lcd.setCursor(lcd_FirstCol, lcd_LastLine);
      lcd.print(words[arrayIndex]);
      toggleSwitchState = digitalRead(toggleSwitchPin);
    }

  if (toggleSwitchState == LOW && button2State == HIGH && arrayIndex < 0) {
      arrayIndex = 5;
      lcd.clear();
      delay(50);
      lcd.setCursor(lcd_FirstCol, lcd_FirstLine);
      lcd.print("toggleSwitch is ON");
      lcd.setCursor(lcd_FirstCol, lcd_LastLine);
      lcd.print(words[arrayIndex]);
      toggleSwitchState = digitalRead(toggleSwitchPin);
    }
 
  //toggleSwitchState = digitalRead(toggleSwitchPin);

  lcd.clear();
  lcd.setCursor(lcd_FirstCol, lcd_LastLine);
  lcd.print("terug start loop");
  delay(1000);
  lcd.clear();
  */





