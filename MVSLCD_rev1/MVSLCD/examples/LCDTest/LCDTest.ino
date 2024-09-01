/*
  Tests/examples of LCD routines added to the LiquidCrystal_I2C library by the MVSLCD library
  By Marcel van Strien, 30-8-2024
*/

#include <MVSLCD.h>

#define lcd_Address 0x27     // The I2C address of the LCD display
#define lcd_NrCols 20        // Number of LCD screen columns, usually 16 or 20
#define lcd_NrLines 4        // Number of LCD screen lines, usually 2 or 4

#define serialBaud 115200    // Serial baudrate

#define minValue 0           // The minimum value used in the value print tests
#define maxValue 127         // The maximum value used in the value print tests

#define valueFieldLen 3      // The field length used in the value print tests
#define stringFieldLen 8     // The field length used in the string print tests
#define valueDelay 50        // The delay between each increase in value in the value print tests
#define directionDelay 1000  // The delay after switching direction (up or down) in showing the values
#define fieldLenDelay 200    // The delay between successive lenghts of output in field tests 
#define testDelay 2000       // The delay between each LCD test

MVSLCD lcd(lcd_Address, lcd_NrCols, lcd_NrLines);

void setup() {
  Serial.begin(serialBaud);
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {
  lcdAlignmentTest1();
  delay(testDelay);	  

  lcdAlignmentTest2();
  delay(testDelay);	

  lcdAlignmentTest3();
  delay(testDelay);

  lcdAlignmentTest4();
  delay(testDelay);
  
  lcdFieldTest();
  delay(testDelay);  

  lcdValueTest1();
  delay(testDelay);

  lcdValueTest2();
  delay(testDelay);

  lcdStringAndValueTest();
  delay(testDelay);

  lcdOverlengthTest();
  delay(testDelay);
}

void lcdAlignmentTest1() {
// Basic test of all lines, left aligned clearing the unused space on the line

 byte line; 
 String valueStr;
 
 for (line = lcd_FirstLine; line <= lcd.lcd_LastLine; line++) {
  valueStr = F("Full line ");
  valueStr += line;
  lcd.printLine(line, valueStr);
 };
}

void lcdAlignmentTest2() {
 // Test of X and Y alignments: Left/right, Top/Bottom

 lcd.clear();
 lcd.printString(lcd_Left, lcd_Top, F("Top left"), lcd_FullLine, lcd_ClearWhitespace);
 lcd.printString(lcd_Right, lcd_Bottom, F("Bottom right"), lcd_FullLine, lcd_ClearWhitespace);
}

void lcdAlignmentTest3() {
 // Test of X alignments: Center 

 lcd.clear();  
 lcd.printString(lcd_Center, lcd_Top, F("Top center"), lcd_FullLine, lcd_ClearWhitespace);
 lcd.printString(lcd_Center, lcd_Bottom, F("Bottom center"), lcd_FullLine, lcd_ClearWhitespace);
 }

void lcdAlignmentTest4() {	
 // Test of X and Y alignments: Center both to the middele of the LCD

 lcd.clear();
 lcd.printString(lcd_Center, lcd_Center, F("Full center"), lcd_FullLine, lcd_ClearWhitespace);
}

void lcdFieldTest() {
 // LCD field test. Write a text to a field behind a prefix string, specifying a field length of 8 characters.
 // We will align it to the left of the field on the first line, and to the right of it on the second line
 
 int value;
 String prefixStr;
 String outputStr;
 byte lcdCol1;
 byte lcdCol2;
 byte outputLen;

 // Fill the screen with equal signs so we can see which LCD content gets overwritten 
 lcd.fillScreen('=');
 
 // Write some prefix strings, and determine where the fields should start (i.e. where the output will be written) from those 
 prefixStr="Left :";
 lcdCol1 = lcd_FirstCol + prefixStr.length();
 lcd.printString(lcd_Left, lcd_Top, prefixStr, lcd_FullLine, lcd_LeaveWhitespace);
 prefixStr="Right:";
 lcdCol2 = lcd_FirstCol + prefixStr.length();
 lcd.printString(lcd_Left, lcd_Bottom, prefixStr, lcd_FullLine, lcd_LeaveWhitespace);

 // Output a text of increasing length to a field, starting from 1 characters up to 8 characters
 outputStr = F("");
 for (outputLen=1; outputLen < stringFieldLen; outputLen++) {
  outputStr += outputLen;
  lcd.printString(lcdCol1, lcd_Top, outputStr, stringFieldLen, lcd_LeaveWhitespace);   // At the top line, the text is aligned left by entering a positive field length
  lcd.printString(lcdCol2, lcd_Bottom, outputStr, -stringFieldLen, lcd_LeaveWhitespace);  // At the bottom line, the text is aligned right by entering a negative field length
  delay(fieldLenDelay);
 };
 
 delay(directionDelay);
 
 // Now do the same, counting back down
 for (outputLen=stringFieldLen; outputLen >= 1; outputLen--) { 
  outputStr = F("");     // Create a string of n characters
  for (byte i = 1; i <= outputLen; i++) {
	  outputStr += i;
  };
  lcd.printString(lcdCol1, lcd_Top, outputStr, stringFieldLen, lcd_LeaveWhitespace);
  lcd.printString(lcdCol2, lcd_Bottom, outputStr, -stringFieldLen, lcd_LeaveWhitespace);
  delay(fieldLenDelay);
 };	
}	
	
void lcdValueTest1() {
 // LCD value test 1. Write a value to a field behind a prefix string describing the value.
 // This time we are using method #1, by specifying the field length of 3 characters ourselves.
 // We will align the value to the left of the field on the first line, and to the right of it on the second line
 
 int value;
 String prefixStr;
 byte lcdCol1;
 byte lcdCol2; 

 // Fill the screen with equal signs so we can see which LCD content gets overwritten 
 lcd.fillScreen('=');
 
 // Write some prefix strings, and determine where the fields should start (i.e. where the values will be written) from those 
 prefixStr="Left  V1:";
 lcdCol1 = lcd_FirstCol + prefixStr.length();
 lcd.printString(lcd_Left, lcd_Top, prefixStr, lcd_FullLine, lcd_LeaveWhitespace);
 prefixStr="Right V1:";
 lcdCol2 = lcd_FirstCol + prefixStr.length();
 lcd.printString(lcd_Left, lcd_Bottom, prefixStr, lcd_FullLine, lcd_LeaveWhitespace);

 // Demo of outputting int values, counting up from 0 to 127, method 1 by pre-specifying field length
 for (value=minValue; value<=maxValue; value++) {
  lcd.printValue(lcdCol1, lcd_Top, value, valueFieldLen);   // At the top line, the value is aligned left by entering a positive field length
  lcd.printValue(lcdCol2, lcd_Bottom, value, -valueFieldLen);  // At the bottom line, the value is aligned right by entering a negative field length
  delay(valueDelay);
 };
 delay(directionDelay);
 for (value=maxValue; value>=minValue; value--) { // Same, counting back down
  lcd.printValue(lcdCol1, lcd_Top, value, valueFieldLen);
  lcd.printValue(lcdCol2, lcd_Bottom, value, -valueFieldLen);
  delay(valueDelay);
 };	
}

void lcdValueTest2() {
 // LCD value test 2. Write a value to a field of length 3.
 // This one is using method #2: by specifying the minimum and maximimum expected values the function calculates that 3 characters are neede to fit all values.
 // We will align the value to the left of the field on the first line, and to the right of it on the second line
 
 int value;
 String prefixStr;
 byte lcdCol1;
 byte lcdCol2; 

 // Fill the screen with equal signs so we can see which LCD content gets overwritten
 lcd.fillScreen('=');

 // Write some prefix strings, and determine where the field should start from those 
 prefixStr=F("Left  V2:");
 lcdCol1 = lcd_FirstCol + prefixStr.length();
 lcd.printString(lcd_Left, lcd_Top, prefixStr, lcd_FullLine, lcd_LeaveWhitespace);
 prefixStr=F("Right V2:");
 lcdCol2 = lcd_FirstCol + prefixStr.length(); 
 lcd.printString(lcd_Left, lcd_Bottom, prefixStr, lcd_FullLine, lcd_LeaveWhitespace);

 // Output int values, counting up from 0 to 127, method 2 by specifying minimum and maximum expected values
 for (value=minValue; value<=maxValue; value++) {
  lcd.printValue2(lcdCol1, lcd_Top, value, minValue, maxValue, lcd_AlignLeft);     // At the top line, the value is aligned left
  lcd.printValue2(lcdCol2, lcd_Bottom, value,  minValue, maxValue, lcd_AlignRight); // At the bottom line, the value is aligned right
  delay(valueDelay);
 };

 delay(directionDelay);

 for (value=maxValue; value>=minValue; value--) {  // Same as above, counting down from 127 to 0
  lcd.printValue2(lcdCol1, lcd_Top, value, minValue, maxValue, lcd_AlignLeft);  
  lcd.printValue2(lcdCol2, lcd_Bottom, value, minValue, maxValue, lcd_AlignRight);  
  delay(valueDelay);  
 };
}

void lcdStringAndValueTest() {
 // Demo of outputting a string followed by an int value counting up from 1 to 127, by pre-specifying field length
 // The function will automatically calculate where the value should be printed behind the prefix string. 
 
 int value;
 
 // Fill the screen with equal signs so we see  which parts of the LCD are overswitten
 lcd.fillScreen('=');
 
 // Display a string followed by a value increasing from 0 to 127
 for (value=minValue; value<=maxValue; value++) {
  lcd.printStringAndValue(lcd_Top, F("Left  S:"), value, valueFieldLen);     // At the top line, the value is aligned left by entering a positive field length
  lcd.printStringAndValue(lcd_Bottom, F("Right S:"), value, -valueFieldLen);  // At the bottom line, the value is aligned right by entering a negative field length
  delay(valueDelay);
 };

 delay(directionDelay); 

 // Same, counting back down
 for (value=maxValue; value>=minValue; value--) {
  lcd.printStringAndValue(lcd_Top, F("Left  S:"), value, valueFieldLen);
  lcd.printStringAndValue(lcd_Bottom, F("Right S:"), value, -valueFieldLen);
  delay(valueDelay);
 }  
};

void lcdOverlengthTest() {
  String testStr;

  // Fill the screen with equal signs so we can see which LCD content gets overwritten
  lcd.fillScreen('=');

  testStr = "1234567890123456789012345";
  lcd.printString(lcd_Center, lcd_Top, "Overlength test", lcd_FullLine, lcd_LeaveWhitespace);  
  lcd.printString(3, lcd_Bottom, testStr, lcd_FullLine, lcd_LeaveWhitespace);
}
