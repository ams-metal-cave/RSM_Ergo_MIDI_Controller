//RSM Menu code for LCD I2C 

/*
Encoder connection: Pins 2 and 3 should work for many processors See README for meaning of constructor arguments.
Use FULL_PULSE for encoders that produce one complete quadrature pulse per detnet, such as: https://www.adafruit.com/product/377
Use HALF_PULSE for endoders that produce one complete quadrature pulse for every two detents, such as: https://www.mouser.com/ProductDetail/alps/ec11e15244g1/?qs=YMSFtX0bdJDiV4LBO61anw==&countrycode=US&currencycode=USD

    uint8_t aPin - Hardware pin connected to the encoder's "A" terminal.
    uint8_t bPin - Hardware pin connected to the encoder's "B" terminal.
    int16_t minValue - Lowest count value to be returned. Further anti-clockwise rotation produces no further change in output.
    int16_t maxValue - Highest count value to be returned. Further clockwise rotation produces no further change in output.
    int16_t initalValue - Initial encoder value. Should be between minValue and maxValue
    uint8_t type Type of encoder - FULL_PULSE (default, one quadrature pulse per detent) or HALF_PULSE (one quadrature pulse for every two detents)

   void configure(uint8_t aPin, uint8_t bPin, int16_t minValue, int16_t maxValue, int16_t initalValue, uint8_t type = FULL_PULSE);
*/


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewEncoder.h>

// Define constants for the LCD display
#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// Create an instance of the LCD display
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// encoder definition
NewEncoder encoder(2, 3, 1, 36, 0, FULL_PULSE);
const int SW_PIN = 6;
int16_t prevEncoderValue;

// startpositie van de encoder + knop
int prevPos = 0;
int encKnopStatus = 0;
int lastEncKnopStatus = 0;

char naamMenu_Nul  [] = {"Menu-Nul:       "};
char naamMenu_Een  [] = {"Menu-Een:       "};
char naamMenu_Twee [] = {"Menu-Twee:      "};

int menu_nul  = 4; // aantal keuzes in hoofdmenu menu_0
int menu_een  = 12; // aantal keuzes per hoofdmenu menu_0 keuze
int menu_twee = 36; // aantal keuzes per menu_1 keuze 

int currentMenuItem_Nul = 0;
int currentMenuItem_Een = 0;
int currentMenuItem_Twee = 0;

// hoofdmenu laag 4#
const char* menu_0 [4]={"k0", "k1", "k2", "Exit menu"}; 

// menu laag 1 3x4
const char* menu_1 [12]={ 
  "k0-1-00", "k0-1-01", "k0-1-02", "back   ",
  "k1-1-00", "k1-1-01", "k1-1-02", "back   ",  
  "k2-1-00", "k2-1-01", "k2-1-02", "back   ",
  };

// menulaag 2 9x4
const char* menu_2 [36]={
  "k0.1.00.2.00", "k0.1.00.2.01", "k0.1.00.2.02", "back        ", 
  "k0.1.01.2.00", "k0.1.01.2.01", "k0.1.01.2.02", "back        ",
  "k0.1.02.2.00", "k0.1.02.2.01", "k0.1.02.2.02", "back        ",
  "k1.1.00.2.00", "k1.1.00.2.01", "k1.1.00.2.02", "back        ",
  "k1.1.01.2.00", "k1.1.01.2.01", "k1.1.01.2.02", "back        ",
  "k1.1.02.2.00", "k1.1.02.2.01", "k1.1.02.2.02", "back        ",
  "k2.1.00.2.00", "k2.1.00.2.01", "k2.1.00.2.02", "back        ",
  "k2.1.01.2.00", "k2.1.01.2.01", "k2.1.01.2.02", "back        ",
  "k2.1.02.2.00", "k2.1.02.2.01", "k2.1.02.2.02", "back        "
  };

// menu startnummer na back functie 
int menu_Level_Nul_Status = 0;  
int menu_Level_Een_Status = 0; 
int menu_Level_Twee_Status = 0;

void setup() {
  
  // Initialize the LCD display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  
  //code naam en revisieaanduiding op scherm
  lcd.setCursor(0, 0);
  lcd.print("MENU LCD I2C ENC");
  lcd.setCursor(0, 1);
  lcd.print("Rev.:3 23-8-2024");
  delay(800);
  lcd.clear();
  
  // Serial monitor
  Serial.begin(115200);
  Serial.println("Starting serial monitor");

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
  //pinMode(SW_PIN, INPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
}

void loop() {
  
  // Read the encoder value
  int16_t currentValue;
  NewEncoder::EncoderState currentEncoderState;

  if (encoder.getState(currentEncoderState)){
      Serial.print("Encoder value: ");
      currentValue = currentEncoderState.currentValue;
            
      if (currentValue != prevEncoderValue && menu_Level_Een_Status == 0 & menu_Level_Twee_Status == 0) { // encoder selectie binnen menu laag nul
          Serial.println(currentValue);
          prevEncoderValue = currentValue;
          currentMenuItem_Nul = currentValue;
          Serial.println(currentMenuItem_Nul);
            
          if(currentMenuItem_Nul > menu_nul){
             currentMenuItem_Nul = 4;
           } 
          if(currentMenuItem_Nul < 0){
             //currentValue = 1;
             currentMenuItem_Nul = 1;        
           }
           // Update tekst op LCD display menu laag nul
           lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print(naamMenu_Nul);
           lcd.setCursor(0, 1);
           lcd.print(menu_0[currentMenuItem_Nul-1]);
       }  
   
      else if (currentValue != prevEncoderValue && menu_Level_Een_Status == 1 && menu_Level_Twee_Status == 0) { // encoder selectie binnen menu laag een
            Serial.println(currentValue);
            prevEncoderValue = currentValue;
            currentMenuItem_Een = currentValue;
            //currentMenuItem_Een = currentMenuItem_Een + 1;
            Serial.println(currentMenuItem_Een);
      
           if (currentMenuItem_Een > menu_een){
                currentMenuItem_Een = menu_een;
              }
            if (currentMenuItem_Een < 0){
                currentMenuItem_Een = 1;
                //currentValue = 1;         
             }    

            // Update tekst op LCD display
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(naamMenu_Een);
            lcd.setCursor(0, 1);
            lcd.print(menu_1 [currentMenuItem_Een]);
            //prevEncoderValue = currentValue;
        
       } 
      lcd.setCursor(11, 0);
      lcd.print(currentValue);
  }
   
  encKnopStatus = digitalRead(SW_PIN); // status push button encoder
  if (encKnopStatus != HIGH && lastEncKnopStatus == LOW){
      lcd.setCursor(15,0);
      lcd.print("*");
      delay(100);

     if (menu_Level_Een_Status == 0 && menu_Level_Twee_Status == 0){ // menu 0 is van toepassing
         menu_Level_Een_Status = 1; //knop ingedrukt van menu 0 naar menu 1
         lcd.setCursor(0,0);
         lcd.print(naamMenu_Een);
         lcd.setCursor(13,1);
         lcd.print(menu_Level_Een_Status);
         lcd.setCursor(14,1);
         lcd.print(" "); 
         lcd.setCursor(15,1);
         lcd.print(menu_Level_Twee_Status);
         lcd.setCursor(15,0);
         lcd.print("*");
         delay(100);

      }
     else if (menu_Level_Een_Status == 1 && menu_Level_Twee_Status == 0){ // menu 1 is van toepassing
              menu_Level_Twee_Status = 1; //knop ingedrukt van menu 1 naar menu 2
              lcd.setCursor(0,0);
              lcd.print(naamMenu_Twee);
              lcd.setCursor(13,1);
              lcd.print(menu_Level_Een_Status);
              lcd.setCursor(14,1);
              lcd.print(" "); 
              lcd.setCursor(15,1);
              lcd.print(menu_Level_Twee_Status);
              lcd.setCursor(15,0);
              lcd.print("*");
              delay(100);
              
      }
  }

  if (encKnopStatus != LOW ){
      //Serial.print("knop uit  ");
      //Serial.print("SW_PIN: ");
      //Serial.print(encKnopStatus);
      //Serial.print("  ");
      //Serial.print("SW_PIN old: ");
      //Serial.println(lastEncKnopStatus);
      lcd.setCursor(15,0);
      lcd.print("-");
   }
 }
   

   
