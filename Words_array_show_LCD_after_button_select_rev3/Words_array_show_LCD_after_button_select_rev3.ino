#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int toggleSwitchPin = 10;
const int button1Pin = 7;
const int button2Pin = 8;
const int buttonState_Off = 1;
const int buttonState_On = 0;
const int buttonState_Unknown = 3; //status knop weet je niet als MCU opstart; iemand kan de knop ingedrukt houden bij opstarten en dat geeft dus een verkeerde waarde als je er standaard vanuit gaat, dat hij normally open is

int prevtoggleSwitchState = buttonState_Unknown;
int toggleSwitchState;

int prevMillies;
int curMillies;

int button1State = 0;
int button2State = 0;

int arrayIndex = 0;

String words[5] = {"EEN", "TWEE", "DRIE", "VIER", "VIJF"};

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  pinMode(toggleSwitchPin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("M coder was here");
  lcd.setCursor(0,2);
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
  lcd.setCursor(0, 0);
  lcd.print("toggleSwitch is ON");
}

void handleToggleOnMode() {
  //Serial.println("Toggle On mode");
}

void initToggleOffMode() {
  lcd.clear();
  Serial.println("Toggle Off mode");
  lcd.setCursor(0, 0);
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
    lcd.setCursor(0, 0);
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
  
  lcd.setCursor(0, 0);
  lcd.print("toggleSwitch is ON");
  lcd.setCursor(0, 1);
  lcd.print(words[arrayIndex]);
  lcd.setCursor(12,1);
  lcd.print(arrayIndex);
  delay(1000);
  toggleSwitchState = digitalRead(toggleSwitchPin);
  button1State = digitalRead(button1Pin);
       
   while (button1State == LOW){

    if (arrayIndex >-1 && arrayIndex <= 3) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("toggleSwitch is ON");
      lcd.setCursor(0, 1);
      lcd.print(words[arrayIndex++]);
      lcd.setCursor(12,1);
      lcd.print(arrayIndex);
      //delay(500);
     }
    toggleSwitchState = digitalRead(toggleSwitchPin);
    button1State = digitalRead(button1Pin);
    delay(50);
    if (arrayIndex >= 4) {
      arrayIndex = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("toggleSwitch is ON");
      lcd.setCursor(0, 1);
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
      lcd.setCursor(0, 0);
      lcd.print("toggleSwitch is ON");
      lcd.setCursor(0, 1);
      lcd.print(words[arrayIndex--]);
      toggleSwitchState = digitalRead(toggleSwitchPin);
    }

  if (toggleSwitchState == LOW && button2State == HIGH && arrayIndex >= 5) {
      arrayIndex = 0;
      lcd.clear();
      delay(50);
      lcd.setCursor(0, 0);
      lcd.print("toggleSwitch is ON");      
      lcd.setCursor(0, 1);
      lcd.print(words[arrayIndex]);
      toggleSwitchState = digitalRead(toggleSwitchPin);
    }

  if (toggleSwitchState == LOW && button2State == HIGH && arrayIndex < 0) {
      arrayIndex = 5;
      lcd.clear();
      delay(50);
      lcd.setCursor(0, 0);
      lcd.print("toggleSwitch is ON");
      lcd.setCursor(0, 1);
      lcd.print(words[arrayIndex]);
      toggleSwitchState = digitalRead(toggleSwitchPin);
    }
 
  //toggleSwitchState = digitalRead(toggleSwitchPin);

  lcd.clear();
  lcd.setCursor(0, 3);
  lcd.print("terug start loop");
  delay(1000);
  lcd.clear();
  */





