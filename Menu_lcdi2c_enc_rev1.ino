#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewEncoder.h>

// Define constants for the LCD display
#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// Create an instance of the LCD display
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// PinsCLK, DT en SW of the encoder
//#define CLK_PIN 2 
//#define DT_PIN 3
#define SW_PIN 6
NewEncoder encoder(2, 3, -20, 20, 0, FULL_PULSE);
int16_t prevEncoderValue;
long previousTimeoutMillis;

// startpositie van de encoder + knop
int prevPos = 0;
int buttonState = 0;
int lastButtonstate = 0;

// Pins 2 and 3 should work for many processors, including Uno. See README for meaning of constructor arguments.
// Use FULL_PULSE for encoders that produce one complete quadrature pulse per detnet, such as: https://www.adafruit.com/product/377
// Use HALF_PULSE for endoders that produce one complete quadrature pulse for every two detents, such as: https://www.mouser.com/ProductDetail/alps/ec11e15244g1/?qs=YMSFtX0bdJDiV4LBO61anw==&countrycode=US&currencycode=USD
/*
    uint8_t aPin - Hardware pin connected to the encoder's "A" terminal.
    uint8_t bPin - Hardware pin connected to the encoder's "B" terminal.
    int16_t minValue - Lowest count value to be returned. Further anti-clockwise rotation produces no further change in output.
    int16_t maxValue - Highest count value to be returned. Further clockwise rotation produces no further change in output.
    int16_t initalValue - Initial encoder value. Should be between minValue and maxValue
    uint8_t type Type of encoder - FULL_PULSE (default, one quadrature pulse per detent) or HALF_PULSE (one quadrature pulse for every two detents)
*/
//void configure(uint8_t aPin, uint8_t bPin, int16_t minValue, int16_t maxValue, int16_t initalValue, uint8_t type = FULL_PULSE);

// Define the menu structure
const char* menuItems[] = {
  "Start_menu       ",
  "  > level1    ",
  "  > level2      ",
  "  > Exit        "
};

const char* settingsMenuItems[] = {
  "level1.X    ",
  "  > level1.1  ",
  "  > level1.2    ",
  "  > Back        "
};

const char* aboutMenuItems[] = {
  "level2.X       ",
  "  > level2.1     ",
  "  > level2.2    ",
  "  > Back        "
};

// Define the current menu and item indices
int currentMenu = 0;
int currentItem = 0;

void setup() {
  
  // Initialize the LCD display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  // Serial monitor
  Serial.begin(115200);
  Serial.println("Starting");

  // Initialize the encoder
  NewEncoder::EncoderState state;

 if (!encoder.begin()) {
    Serial.println("Encoder Failed to Start. Check pin assignments and available interrupts. Aborting.");
    while (1) {
      yield();
    }
  } else {
    encoder.getState(state);
    Serial.print("Encoder Successfully Started at value = ");
    prevEncoderValue = state.currentValue;
    Serial.println(prevEncoderValue);
  }
  pinMode(SW_PIN, INPUT_PULLUP);

}

void loop() {
  // Read the encoder value
  int16_t currentValue;
  NewEncoder::EncoderState currentEncoderState;

if (encoder.getState(currentEncoderState)) {
    Serial.print("Encoder: ");
    currentValue = currentEncoderState.currentValue;
    // Update the current item index
    if (currentValue != 0) {
      
      currentItem += currentValue;
      Serial.println(currentValue);
      prevEncoderValue = currentValue;
      currentItem = currentValue;

    if (currentItem < 0) {
      currentItem = 0;
    } else if (currentItem >= sizeof(menuItems) / sizeof(menuItems[0])) {
      currentItem = sizeof(menuItems) / sizeof(menuItems[0]) - 1;
    }

    }  
    Serial.print("Status menu + level");
    Serial.print(menuItems[currentItem]); // Print the last selected menu-item
    Serial.println(currentItem); 
  }

    // Update the LCD display
    lcd.setCursor(0, 0);
    lcd.print(menuItems[currentItem]);
  
  buttonState = digitalRead(SW_PIN); // status switch knob encoder
  if (buttonState == LOW && lastButtonstate == HIGH){
     // Handle the button press
     handleButtonPress();
     Serial.print("buttonState:  ");
     Serial.println(buttonState);
     Serial.print("lastButtonstate:  ");
     Serial.println(lastButtonstate);
    }
  lastButtonstate = buttonState;  
   
}
void handleButtonPress() {
  // Check the current menu and item indices
  switch (currentMenu) {
    case 0: // Main Menu
      switch (currentItem) {
        case 1: // level1
          currentMenu = 1;
          currentItem = currentItem + 1;
          break;
        case 2: // level2
          currentMenu = 2;
          currentItem = currentItem + 1 ;
          break;
        case 3: // Exit
          currentMenu = currentMenu -1;
          currentItem = 0;
          // Exit the menu
          break;
      }
      break;
    case 1: // level1
      switch (currentItem) {
        case 1: // Brightness
          // Handle brightness setting
          currentMenu = 1;
          currentItem = currentItem +1;
          break;
        case 2: // level2
          // Handle contrast setting
          break;
        case 3: // Back
          currentMenu = 0;
          currentItem = 0;
          // Exit the menu
          break;
      }
      break;
    case 2: // About Menu
      switch (currentItem) {
        case 1: // Version
          // Handle version display
          break;
        case 2: // Credits
          // Handle credits display
          break;
        case 3: // Back
          currentMenu = 0;
          currentItem = 0;
          // Exit the menu
          break;
      }
      break;
  }

  // Update the LCD display
  lcd.setCursor(0, 0);
  lcd.clear();
  switch (currentMenu) {
    case 0:
  
      lcd.print(menuItems[currentItem]);
      break;
    case 1:
    
      lcd.print(settingsMenuItems[currentItem]);
      lcd.setCursor(1,5);
      lcd.print("yes");
      break;
    case 2:
    
      lcd.print(aboutMenuItems[currentItem]);
      lcd.setCursor(1,5);
      lcd.print("level 2.x deep");
      delay(1000);
      lcd.clear();
      break;
  }
}
