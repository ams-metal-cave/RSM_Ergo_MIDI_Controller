//By RSM: generic Menu code for LCD I2C 2x16 with rotary encoder with knob function 

/*
Explanation NewEncoder.h:
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

// revisie van sketch
const char* rev [] = {"Rev.:6 23-8-2024"}; 

// Create an instance of the LCD display
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// encoder definition
NewEncoder encoder(2, 3, -1, 36, -1, FULL_PULSE);
const int SW_PIN = 6;
int16_t prevEncoderValue;

// startpositie van de encoder + knop
int prevPos = 0;
int encKnopStatus = 0;
int lastEncKnopStatus = 0;

// menu lagen
char naamMenu_Nul  [] = {"Menu-Nul:       "};
char naamMenu_Een  [] = {"Menu-Een:       "};
char naamMenu_Twee [] = {"Menu-Twee:      "};

int menu_nul  = 4;                                                        // aantal keuzes in hoofdmenu menu_0 array
int menu_een  = 12;                                                       // aantal subkeuzes per hoofdmenu item => is menu laag een; zie menu_1 array
int menu_twee = 36;                                                       // aantal subkeuzes per menu_1 item => is menu laag twee; zie menu_2 array 

// menu status na encoder knop indrukking 
int menu_Level_Nul_Status = 0;                                            // huidige status menu nul => nog niet gebruikt  
int menu_Level_Een_Status = 0;                                            // huidige status menu een (0 = niet geselecteerd; 1 = actief (in combi met menu twee = 0))
int menu_Level_Twee_Status = 0;                                           // huidige status menu twee (0 = niet geselecteerd; 1 = actief (in combi met menu een = 1))

int currentMenuItem_Nul = 0;                                              // encoder status nummer wordt gekopieerd naar deze variabele om de positie in de menu_0 array te bewaren 
int currentMenuItem_Een = 0;                                              // encoder status nummer wordt gekopieerd naar deze variabele om de positie in de menu_1 0array te bewaren
int currentMenuItem_Twee = 0;                                             // encoder status nummer wordt gekopieerd naar deze variabele om de positie in de menu_2 array te bewaren

// hoofdmenu laag keuzes 1x4 
const char* menu_0 [4]={"k0", "k1", "k2", "Verlaat menu"};                // encoder min waarde op -1 en startwaarde op -1 => eerste waarde encoder is dan 0 en dit is de eerste menu_0 array test waarde

// menu laag een met keuze 3x4
const char* menu_1 [12]={ 
  "k0-1-00", "k0-1-01", "k0-1-02", "Terug   ",
  "k1-1-00", "k1-1-01", "k1-1-02", "Terug   ",  
  "k2-1-00", "k2-1-01", "k2-1-02", "Terug   ",
  };

// menulaag twee aantal keuze 9x4
const char* menu_2 [36]={
  "k0.1.00.2.00", "k0.1.00.2.01", "k0.1.00.2.02", "Terug        ", 
  "k0.1.01.2.00", "k0.1.01.2.01", "k0.1.01.2.02", "Terug        ",
  "k0.1.02.2.00", "k0.1.02.2.01", "k0.1.02.2.02", "Terug        ",
  "k1.1.00.2.00", "k1.1.00.2.01", "k1.1.00.2.02", "Terug        ",
  "k1.1.01.2.00", "k1.1.01.2.01", "k1.1.01.2.02", "Terug        ",
  "k1.1.02.2.00", "k1.1.02.2.01", "k1.1.02.2.02", "Terug        ",
  "k2.1.00.2.00", "k2.1.00.2.01", "k2.1.00.2.02", "Terug        ",
  "k2.1.01.2.00", "k2.1.01.2.01", "k2.1.01.2.02", "Terug        ",
  "k2.1.02.2.00", "k2.1.02.2.01", "k2.1.02.2.02", "Terug        "
  };

void setup() {
  
  // Initialize the LCD display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  
  //code naam en revisieaanduiding op scherm
  lcd.setCursor(0, 0);
  lcd.print("MENU LCD I2C ENC");
  lcd.setCursor(0, 1);
  lcd.print(rev[0]);
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
      
// encoder selectie binnen menu laag nul            
      if (currentValue != prevEncoderValue && menu_Level_Een_Status == 0 & menu_Level_Twee_Status == 0) { 
          prevEncoderValue = currentValue;
          currentMenuItem_Nul = currentValue;
  
          if(currentMenuItem_Nul >= menu_nul){
             currentMenuItem_Nul = menu_nul -1;
           } 
          if(currentMenuItem_Nul <= -1){
             currentMenuItem_Nul = 0;        
           }
// Update tekst op LCD display menu laag nul
           lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print(naamMenu_Nul);
           lcd.setCursor(0, 1);
           lcd.print(menu_0[currentMenuItem_Nul]);
       }
// encoder selectie binnen menu laag
      if (currentValue != prevEncoderValue && menu_Level_Een_Status == 1 && menu_Level_Twee_Status == 0 ) { 
          prevEncoderValue = currentValue;
          currentMenuItem_Een = currentValue;
// encoder scrollen binnen menu laag roterend binnen grenzen)
         if (currentMenuItem_Een > -1 | currentMenuItem_Een <=3){
             if(currentMenuItem_Een >= 4){
               currentMenuItem_Een = 4 - 1;
             } 
             if (currentMenuItem_Een <= -1){
               currentMenuItem_Een = 0;        
             }
         }
// encoder scrollen binnen menu laag roterend binnen grenzen)
         if (currentMenuItem_Een > 4 | currentMenuItem_Een <=7){
             if(currentMenuItem_Een >= 7){
               currentMenuItem_Een = 7 - 1;
             } 
             if (currentMenuItem_Een <= -1){
               currentMenuItem_Een = 0;        
             }
         }
// Update tekst op LCD display
          lcd.setCursor(0, 0);
          lcd.print(naamMenu_Een);
          lcd.setCursor(0, 1);
          lcd.print(menu_1 [currentMenuItem_Een]);               
      } 
      lcd.setCursor(11, 0);                                                        // encoder test waarde   
      lcd.print("  ");                                                             // encoder test waarde     
      lcd.setCursor(11, 0);                                                        // encoder test waarde   
      lcd.print(currentValue);                                                     // encoder test waarde     
  }
   
  encKnopStatus = digitalRead(SW_PIN); // status push button encoder
  if (encKnopStatus != HIGH && lastEncKnopStatus == LOW){
      delay(50);
      lcd.setCursor(15,0);                                                      // test data werking encoderknop print op LCD: knop ingedrukt => * te zien
      lcd.print("*");                                                           // test data werking encoderknop print op LCD: knop ingedrukt => * te zien
      delay(100);
      lcd.clear();                                                              // test data werking encoderknop print op LCD: knop ingedrukt => * te zien

// encoder knop ingedrukt: van menu nul naar menu een 
     if (menu_Level_Een_Status == 0 && menu_Level_Twee_Status == 0){            // menu nul is huidig menu
         menu_Level_Een_Status = 1;                                             // menu nul is nu menu een geworden

// start menu laag een op de eerste keuze positie binnen de gekozen keuze optie van menu laag nul (dus: k0 wordt k0-1-00; k1 wordt k1-1-00; etc)          
         if (currentMenuItem_Nul == 0){
             currentMenuItem_Een = currentMenuItem_Nul;
         }  
         if (currentMenuItem_Nul == 1){
            currentMenuItem_Een = currentMenuItem_Nul + 4;
         }        
         if (currentMenuItem_Nul == 2){
                  currentMenuItem_Een = currentMenuItem_Een + 7;
         }
         if (currentMenuItem_Nul == 3){
                  currentMenuItem_Een = currentMenuItem_Een + 9;
         }

         lcd.setCursor(0, 1);
         lcd.print("            ");                                              // lcd wissen voorgaande data                                                    
         lcd.setCursor(0, 0);
         lcd.print(naamMenu_Een);
         lcd.setCursor(0, 1);
         lcd.print(menu_1 [currentMenuItem_Een]);
         lcd.setCursor(13,1);                                                    // test data menu een status: 0 => menu nul actief; 1 => menu een actief NB: menu twee status moet dan 0 zijn
         lcd.print(menu_Level_Een_Status);                                      
         lcd.setCursor(14,1);
         lcd.print(" "); 
         lcd.setCursor(15,1);
         lcd.print(menu_Level_Twee_Status);                                     // test data menu een status: 0 => menu nul of een actief; 1 => menu twee actief NB: menu een status moet dan 1 zijn
         lcd.setCursor(11, 0);                                                  // encoder test waarde   
         lcd.print(currentValue);                                               // encoder test waarde     
         delay(50);                                                             // debounce encoder knop                                 
      }
// encoder knop indrukken: van menu een naar menu twee 
     else if (menu_Level_Een_Status == 1 && menu_Level_Twee_Status == 0){       // menu een is huidig menu
              menu_Level_Een_Status = 1;              
              menu_Level_Twee_Status = 1; 
              //lcd.setCursor(0,0);
              //lcd.print(naamMenu_Twee);
              lcd.setCursor(13,1);
              lcd.print(menu_Level_Een_Status);
              lcd.setCursor(14,1);
              lcd.print(" "); 
              lcd.setCursor(15,1);
              lcd.print(menu_Level_Twee_Status);  
              delay(50);                                                        // debounce encoder knop            
      }
  }
  if (encKnopStatus != LOW ){
      lcd.setCursor(15,0);
      lcd.print("-");
 
   }
 }
   

   
