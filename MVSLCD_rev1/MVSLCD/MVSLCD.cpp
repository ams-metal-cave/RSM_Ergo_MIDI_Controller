/*
MVSLCD.h - Library adding LCD functions
Created by Marcel van Strien, August 24, 2024.
*/

// #define debugLCD

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <MVSLCD.h>

// Constructor
MVSLCD::MVSLCD(int lcd_Address, int lcd_NrCols, int lcd_NrLines)
  : LiquidCrystal_I2C(lcd_Address, lcd_NrCols, lcd_NrLines) {	  
	  
	lcd_LastCol = lcd_FirstCol + lcd_NrCols - 1;
	lcd_LastLine = lcd_FirstLine + lcd_NrLines - 1;	 
	_lcd_Address = lcd_Address;
	_lcd_NrCols  = lcd_NrCols;
	_lcd_NrLines = lcd_NrLines;	
  }

void MVSLCD::fillScreen(char fillChar) {
  for (byte line = lcd_FirstLine; line <= lcd_LastLine; line++) {
    fillLine(line, fillChar);
  } 
}

void MVSLCD::fillLine(byte line, char fillChar) {
  String lcdText;
/*
 setCursor(0, line-1);
  for (byte i=1; i <= _lcd_NrCols; i++) {
    print(fillChar);
  } 
*/  
  lcdText = F("");
  for (byte i=lcd_FirstCol; i <= lcd_LastCol; i++) {
    lcdText += fillChar;
  }
  printString(lcd_Left, line, lcdText, lcd_FullLine, lcd_LeaveWhitespace);
}

void MVSLCD::printLine(int8_t line, String lcdText) {
  printString(lcd_Left, line, lcdText, lcd_FullLine, lcd_ClearWhitespace);
}

// Print a prefix string followed by an integer value to the LCD by pre-specifying the field length
// Use negative field length value to use right alignment
void MVSLCD::printStringAndValue(int8_t line, String prefixStr, int value, int fieldLen) {  
  String valueStr;
  int8_t prefixStrLen;

  valueStr = value; 
  printString(lcd_FirstCol, line, prefixStr, lcd_FullLine, lcd_LeaveWhitespace);
  prefixStrLen = prefixStr.length();
  printValue(lcd_FirstCol + prefixStrLen, line, value, fieldLen);
}

// Print an integer value to LCD. Variant 1: by pre-specifying the field length
// Use negative field length value to use right alignment
void MVSLCD::printValue(int8_t col, int8_t line, int value, int fieldLen) {  
  String valueStr;

  valueStr = value;
  printString(col, line, valueStr, fieldLen, lcd_LeaveWhitespace);
}

// Print an integer value to LCD. Variant 2: by specifying the minimum and maximum value to be expected.
// The function calculates the field length from those values
void MVSLCD::printValue2(int8_t col, int8_t line, int value, int minValue, int maxValue, bool alignRight) {  
  String valueStr;
  uint8_t valueStrLen; 
  int fieldLen;

  valueStr = minValue;
  valueStrLen = valueStr.length();
  fieldLen = valueStrLen;
  valueStr = maxValue;
  valueStrLen = valueStr.length();
  if (valueStrLen > fieldLen) {
    fieldLen = valueStrLen;
  };
  if (alignRight) {
	  fieldLen = -fieldLen;
  };   
  valueStr = value;
  printString(col, line, valueStr, fieldLen, lcd_LeaveWhitespace);
}  

void MVSLCD::printString(int8_t col, int8_t line, String inputText, int fieldLen, bool clearWhitespace) {  
  String outputText;
  String lcdText;
  byte x;
  byte y;
  unsigned int inputTextLen;
  unsigned int outputTextLen;  
  int whiteSpaceLeft = 0;
  int whiteSpaceRight = 0;
  byte i;

#ifdef debugLCD
  Serial.print(F("LCD print input: col = "));
  Serial.print(col);
  Serial.print(F(", line = "));
  Serial.print(line);
  Serial.print(F(", field length = "));
  Serial.print(fieldLen);  
  Serial.print(F(", Clear whitespace = "));
  Serial.print(clearWhitespace);    
  Serial.print(F(": '"));
  Serial.print(inputText);
  Serial.println(F("'"));
#endif

  inputTextLen = inputText.length();
  
  if (fieldLen == lcd_FullLine) {// No field specified, use full LCD line	
#ifdef debugLCD  
   Serial.println("LCD field not specified, using full line");
#endif	
   outputText = inputText;
   outputTextLen = inputTextLen;
  } else {  	  // Field length specified, value is positive for left alignment or negative for right alignment within field
#ifdef debugLCD  
    Serial.println(F("LCD field specified"));
#endif	
    if (inputTextLen > abs(fieldLen)) {
#ifdef debugLCD		
     Serial.println("ERROR: String is longer that the field length!");
#endif	 
     // Restrict text to field length?
    };
    if (fieldLen > 0) {  // Positive value so left alignmment, add spaces to end of string    
      outputText = inputText;      
      for (i = 0; i< fieldLen - inputTextLen; i++) {
           outputText += F(" ");
      };
    } else {  // Negative value so right alignmment, add spaces before the string      
        outputText = "";               
        for (i = 0; i < -fieldLen - inputTextLen; i++) {
           outputText += F(" "); 
        }
        outputText += inputText;
    }
    outputTextLen = fieldLen; 
  }; 

  // Calculate effective LCD column
  if (col == lcd_Center) {  // Center in X direction
     // Serial.println("Center X align");
     whiteSpaceLeft = (_lcd_NrCols - outputTextLen) / 2;
     whiteSpaceRight = _lcd_NrCols - outputTextLen - whiteSpaceLeft;
   } else if (col < 0) {   // Align to right of display
     // Serial.println("Right X align");
     whiteSpaceRight = abs(col) - lcd_FirstCol;
     whiteSpaceLeft = _lcd_NrCols - inputTextLen - whiteSpaceRight;
    } else {   // col > 0, Align to left of display  
      // Serial.println("Left X align");
      whiteSpaceLeft = col - lcd_FirstCol;
      whiteSpaceRight = _lcd_NrCols - outputTextLen - whiteSpaceLeft;
     x = lcd_FirstCol;   
    };

  // Calculate effective LCD row
  if (line == lcd_Center) {  
    // Center in Y direction
#ifdef debugLCD	
    Serial.println(F("Center Y align"));
#endif	
     y = lcd_FirstLine + (_lcd_NrLines -1) / 2;  
  } else if (line < 0) {
    // Align to bottom of display
#ifdef debugLCD		
    Serial.println(F("Bottom Y align"));
#endif	
    y = lcd_FirstLine + _lcd_NrLines + line;
  } else {   
    // Align to top of display
#ifdef debugLCD		
    Serial.println(F("Top Y align"));
#endif	
    y = line;
  }

  lcdText = "";

  if (whiteSpaceLeft > 0) { 
    if (clearWhitespace) {      
    // Fill up empty space on the left
#ifdef debugLCD	
    Serial.println(F("Adding whitespace to the left"));
#endif	
    for (i=0; i< whiteSpaceLeft; i++) {
       lcdText += F(" ");
       x = lcd_FirstCol;
    };  
    } else {      
    // Skip left whitespace
      x = lcd_FirstCol + whiteSpaceLeft;
    }
  };

  lcdText += outputText;

  if ((whiteSpaceRight > 0) && clearWhitespace) { 
    // Fill up empty space on the right
#ifdef debugLCD	
    Serial.println(F("Adding whitespace to the right"));
#endif	
    for (i = 0; i < whiteSpaceRight; i++) {
       lcdText += " ";
    };  
  };

  setCursor(x-1, y-1);
  // Check if the output fits on the LCD
  if (lcdText.length() > _lcd_NrCols - x  + 1) {       // String to long to fit on LCD
#ifdef debugLCD	  
     Serial.print(F("Line too long to fit on the LCD: '"));
     Serial.print(lcdText);
     Serial.println(F("'"));
     Serial.print(F("Truncated output: '"));
     Serial.print(lcdText);
     Serial.println(F("'"));	 
#endif	    
	 outputText = lcdText.substring(0, _lcd_NrCols - x  + 1); // Truncate it to maximum length
	 lcdText = outputText;
#ifdef debugLCD
     Serial.print(F("Truncated output: '"));
     Serial.print(lcdText);
     Serial.println(F("'"));	 
#endif	    	 
  }	  
#ifdef debugLCD	  
   Serial.print(F("Writing text to LCD ("));
   Serial.print(x);
   Serial.print(F(","));
   Serial.print(y);
   Serial.print(F("): '"));
   Serial.print(lcdText);
   Serial.println(F("'"));
#endif	
   print(lcdText);
}
