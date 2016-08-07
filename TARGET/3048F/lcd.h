
/*****************************************************************************
* Header file for LCD 
******************************************************************************/

#ifndef LCD_H
	#define	LCD_H
#endif

#include "lcd.c"

void lcd_char1(unsigned char x,char *input);
void lcd_char2(unsigned char x,char *input);

void lcd_string1(unsigned char x,char *input);
void lcd_string2(unsigned char x,char *input);

void lcd_time(int input1, int input2, int input3);
void lcd_date(int input1, int input2, int input3);

