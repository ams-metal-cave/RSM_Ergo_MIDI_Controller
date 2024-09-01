/*
MVSLCD.h - Library for LCD functions
Created by Marcel van Strien, August 24, 2024.
*/
#ifndef MVSLCD_
#define MVSLCD_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <inttypes.h>

#define lcd_FirstCol 1
#define lcd_FirstLine 1
#define lcd_Top 1
#define lcd_Bottom -1
#define lcd_Left 1
#define lcd_Right -1
#define lcd_Center 0
#define lcd_FullLine 0
#define lcd_ClearWhitespace true
#define lcd_LeaveWhitespace false
#define lcd_AlignRight true
#define lcd_AlignLeft false

class MVSLCD : public LiquidCrystal_I2C
{
	public:
	    MVSLCD(int lcd_Address, int lcd_NrCols, int lcd_NrLines);
		void initLCD();
		void printString(int8_t col, int8_t line, String inputText, int fieldLen, bool clearWhitespace);
		void printLine(int8_t line, String lcdText);
		void printValue(int8_t col, int8_t line, int value, int fieldLen);
		void printValue2(int8_t col, int8_t line, int value, int minValue, int maxValue, bool alignRight);
		void printStringAndValue(int8_t line, String prefixStr, int value, int fieldLen);
		void fillLine(byte line, char fillChar);
		void fillScreen(char fillChar);
		
	    int lcd_LastCol;
	    int lcd_LastLine;		

	private:
		int _lcd_Address;
		int _lcd_NrCols;
		int _lcd_NrLines;
};

#endif
