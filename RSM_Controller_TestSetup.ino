#include <Wire.h>
#include <Bounce2.h>
#include <LiquidCrystal_I2C.h>

#define appName "RSM MIDI Controller"
#define appVersion "v0.1"

#define mode_Running 0
#define mode_Edit_Start_Waiting 1
#define mode_Edit 2
#define mode_Edit_Exit_Waiting 3
#define editModeWaitTime 2000

#define encoderSwitchPin 10
#define button1Pin 7
#define button2Pin 8
#define numButtons 3
#define buttonBounceInterval 5

#define serialBaud 115200

#define lcd_Address 0x27
#define lcd_NrCols 20
#define lcd_NrLines 2
#define lcd_FirstCol 1
#define lcd_LastCol lcd_FirstCol + lcd_NrCols - 1
#define lcd_FirstLine 1
#define lcd_LastLine lcd_FirstLine + lcd_NrLines - 1

#define midi_NrChannels 16
#define midi_MinChan 1
#define midi_MaxChan midi_MinChan + midi_NrChannels -1

#define buttonState_On LOW
#define buttonState_Off HIGH
#define encoderButtonState_On LOW
#define encoderButtonState_Off HIGH
#define buttonState_Unknown 3 //status knop weet je niet als MCU opstart; iemand kan de knop ingedrukt houden bij opstarten en dat geeft dus een verkeerde waarde als je er standaard vanuit gaat, dat hij normally open is

const uint8_t buttonPins[numButtons] = {encoderSwitchPin, button1Pin, button2Pin};

uint8_t prev_encoderSwitchState = buttonState_Unknown;
uint8_t currentMode = mode_Running;
uint8_t encoderSwitchState;

unsigned long prevMillies;
unsigned long curMillies;
unsigned long encoderStateDuration;
unsigned long encoderPrevDuration;

uint8_t button1State = LOW;
uint8_t button2State = LOW;

Bounce * buttons = new Bounce[numButtons];
LiquidCrystal_I2C lcd(lcd_Address, lcd_NrCols, lcd_NrLines);  

void setup() {
  initSerial();
  initLCD();
  initButtons();
  initEncoders();
  initPots();
  initMidi();
  welcomeMessage();
  startRunningMode();
}

void initSerial() {
  Serial.begin(serialBaud);
  delay(1900);
}

void initMidi() {
  // Nog te vullen
}

void initLCD() {
  Serial.println("Initializing LCD");
  lcd.init();
//  delay(100);
  lcd.backlight();
  lcd.clear();
//  delay(100)
}

void initButtons() {
  Serial.println("Initializing buttons");
  for (int i = 0; i < numButtons; i++) {
   buttons[i].attach(buttonPins[i] , INPUT_PULLUP);       //setup the bounce instance for the current button
//   buttons[i].setPressedState(LOW);
   buttons[i].interval(buttonBounceInterval);              // interval in ms
  }
}

void initEncoders() {
  Serial.println("Initializing encoders");  
}

void initPots() {
  Serial.println("Initializing potmeters");  
}

void welcomeMessage() {
  String versionStr;

  Serial.println("Showing welcome message");
  printToLCD(lcd_FirstCol,lcd_FirstLine, appName);
//  printToLCD(lcd_FirstCol,lcd_LastLine,"version " + appVersion);
  versionStr = "version ";
  versionStr = versionStr + appVersion;
  printToLCD(lcd_FirstCol,lcd_LastLine,versionStr);  
  delay(2000);
//  lcd.clear();  
}

void startRunningMode () {
  lcd.clear();
  printToLCD(lcd_FirstCol,lcd_FirstLine, "Controller status");
  currentMode = mode_Running;
  printCurrentMode();  
}

void loop() {
  handleMidi();
  handleButtons();
  handleEncoders();
  handlePots();
}

void handleMidi() {
  // Nog te vullen
}

void handleButtons() {
  unsigned long currentDuration;
  String serialLine;

  for (int i = 0; i < numButtons; i++)  {
    buttons[i].update();    // Update the Bounce instance
    if ( buttons[i].rose() ) {          // LOW --> HIGH
      handleButtonPress(i);
    } else if (buttons[i].fell()) {     // HIGH --> LOW
      handleButtonRelease(i);
    } else if (i == 0 && (currentMode == mode_Edit_Start_Waiting) || (currentMode == mode_Edit_Exit_Waiting)) {  // Encoder button still pressed
      int debouncedState = buttons[i].read();
      currentDuration = buttons[i].currentDuration();
//      if (debouncedState == HIGH && buttons[i].currentDuration() > editModeWaitTime) {
      if (debouncedState == HIGH && currentDuration > editModeWaitTime) {
        serialLine = "Long press, duration ";
        serialLine = serialLine + currentDuration;
        Serial.println(serialLine); 
        handleEncoderSwitchLongPress();
      }
    }
  }
}

void handleButtonPress(int buttonNr) {
  String serialLine;

  if (buttonNr == 0) {
    handleEncoderSwitchPress();
  } else {
   serialLine = "Button ";
   serialLine = serialLine + buttonNr;
   serialLine = serialLine + " was pressed";
   Serial.println(serialLine);
  }
}

void handleButtonRelease(int buttonNr) {
  String serialLine;

  if (buttonNr == 0) {
    handleEncoderSwitchRelease();
  } else {
   serialLine = "Button ";
   serialLine = serialLine + buttonNr;
   serialLine = serialLine + " was pressed";
   Serial.println(serialLine);
  }
}

void handleEncoderSwitchPress() {
  Serial.println("Encoder switch is ON");
  lcd.clear();
  //delay(50);
  printToLCD(lcd_FirstCol, lcd_FirstLine, "Encoder sw ON ");
  if (currentMode == mode_Running) {
    currentMode = mode_Edit_Start_Waiting;
  } else if (currentMode = mode_Edit) {
    currentMode = mode_Edit_Exit_Waiting;
  }
  printCurrentMode();  
}

void handleEncoderSwitchLongPress() {
  String serialLine;
  String lcdLine;

  serialLine = "Encoder switch long press from mode ";
  serialLine = serialLine + currentMode;
  Serial.println(serialLine);
  lcd.clear();
  //delay(50);
  printToLCD(lcd_FirstCol, lcd_FirstLine, "Encoder - LONG");
  lcdLine = "Current mode: ";
  lcdLine = lcdLine + currentMode;  
  printToLCD(lcd_FirstCol, lcd_LastLine, lcdLine);  
  if (currentMode == mode_Edit_Start_Waiting) {
    EnterEditMode();
  } else if (currentMode == mode_Edit_Exit_Waiting) {
    ExitEditMode();
  }
  printCurrentMode();  
}

void handleEncoderSwitchRelease() {
  Serial.println("Encoder switch is OFF");
  lcd.clear();
  //delay(50);
  printToLCD(lcd_FirstCol, lcd_FirstLine, "Encoder sw OFF");
  if (currentMode == mode_Edit_Start_Waiting) {  // Cancel start edit mode
    currentMode = mode_Running;
  } else if (currentMode == mode_Edit_Exit_Waiting) {  // Cancel exit edit mode
    currentMode = mode_Edit;
  }   
  printCurrentMode();  
}

void handlePots() {
 // Nog te vullen
}

void handleEncoders() {
 // Nog te vullen
}

void EnterEditMode() {
  Serial.println("Entering edit mode");
  lcd.clear();
  printToLCD(lcd_FirstCol, lcd_FirstLine, "Edit mode");  
  currentMode = mode_Edit;
  printCurrentMode();
}

void ExitEditMode() {
  Serial.println("Exiting edit mode");
  startRunningMode();
}

void printCurrentMode() {
   String strLine;

   strLine = "Mode: ";
   switch (currentMode) {
      case 0: 
         strLine = strLine + "Running";
         break;
      case 1: 
         strLine = strLine + "Edit start W";
         break;      
      case 2: 
         strLine = strLine + "Edit";
         break;      
      case 3: 
         strLine = strLine + "Edit exit w";
         break;                        
   }
   Serial.println(strLine); 
   printToLCD(lcd_FirstCol, lcd_LastLine, strLine);
}   

void printToLCD(int8_t col, int8_t line, String lcdText) {  
  String serialLine;
  
  serialLine = "Writing text to LCD (";
  serialLine = serialLine + col;
  serialLine = serialLine + ",";
  serialLine = serialLine + line;
  serialLine = serialLine + "): '";
  serialLine = serialLine + lcdText;
  serialLine = serialLine + "'";
  Serial.println(serialLine);  
  lcd.setCursor(col-1, line-1);  
  lcd.print(lcdText); 
}
