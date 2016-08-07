
/*****************************************************************************
* Library for LCD 
******************************************************************************/


#include "io.h"
#include <string.h>

#define  E_SIG  0x20       //E signal
#define  RS_SIG 0x10       //RS signal

void wait(void)            //Wait function
{
    int  t= 800;          //Wait 0.9 ms
    while(t--);
}


/*****************************************************************************
*   intlen
*
*   Get length of interger
*
*   Input: int number
*
*   Output: int length_of_number
*
******************************************************************************/


int intlen(int x) { 
    if(x>=100000) {
        if(x>=10000000) {
            if(x>=1000000000) return 10;
            if(x>=100000000) return 9;
            return 8;
        }
        if(x>=1000000) return 7;
        return 6;
    } else {
        if(x>=1000) {
            if(x>=10000) return 5;
            return 4;
        } else {
            if(x>=100) return 3;
            if(x>=10) return 2;
            return 1;
        }
    }
}


/*****************************************************************************
*   int2char
*
*   Convert interger to string
*
*   Intput: char* string 
*               Destination to copy string to
*           int len
*               Length of string to create
*           int val
*               Interger to convert
* 
*   Output: none
*
******************************************************************************/


void int2char (char* str, int len, int val)
{
    char i;
    for(i=1; i<=len; i++)
    {
        str[len-i] = (char) ((val % 10UL) + '0');
        val/=10;
    }

    str[i-1] = '\0';
}


/*****************************************************************************
*   LCD Processing Functions
*
*   Private Functions
*   void lcdo8(unsigned char d)
*   void lcdini(void)
*   void lcdclr(void)
*   void lcdxy(unsigned char y, unsigned char x)
*   void lcd04(char d)
*
*   Public Functions
*   void lcd_char1(unsigned char x,char *input)
*   void lcd_char2(unsigned char x,char *input)
*   void lcd_string1(unsigned char x,char *input)
*   void lcd_string2(unsigned char x,char *input)
*
******************************************************************************/


//Send 8 bit to LCD
void lcdo8(unsigned char d)
{
    d = d | E_SIG;          
    P3DR = d;               
    d = d & 0xdf;           
    P3DR = d;               
    wait();                 
}

//Initialize LCD device
void lcdini(void)
{
    lcdo8(3);               
    lcdo8(3);               
    lcdo8(3);      
    lcdo8(2);               
    lcdo8(2);               
    lcdo8(8);               
    lcdo8(0);      
    lcdo8(0x0c);            
    lcdo8(0);      
    lcdo8(6);               
}

//Clear LCD screen
void lcdclr(void)
{
    P3DR = 0;
    lcdo8(0);
    lcdo8(1);               
    wait();
}

//Jump to position x y
void lcdxy(unsigned char y,unsigned char x)	
{
    P3DR = 0;
    lcdo8(y);               //Send y axis
    lcdo8(x);               //Send x axis
    wait();
}

//Display character at current position
void lcdo4(char d)	
{
    unsigned dd;
    dd = d;                 
    d = d >> 4;             
    d = d & 0x0f;           
    d = d | RS_SIG;         
    lcdo8(d);               
    wait();                 
    dd = dd & 0x0f;         
    dd = dd | RS_SIG;      
    lcdo8(dd);             
    wait();
}

//Display char input on position x of first row
void lcd_char1(unsigned char x,char *input)
{
  lcdxy(8,x);
  lcdo4(*input);
}

//Display char input on position x of second row
void lcd_char2(unsigned char x,char *input)
{
  lcdxy(0x0c,x);
  lcdo4(*input);
}

//Display string input on first row
void lcd_string1(unsigned char x,char *input)
{ 
    int i;
    for (i=0;i<strlen(input);i++){
      lcdxy(8,x+i);
      lcdo4(input[i]);
    }
}

//Display string input on second row starting at position x
void lcd_string2(unsigned char x,char *input)
{
    int i;
    for (i=0;i<strlen(input);i++){
      lcdxy(0xc,x+i);
      lcdo4(input[i]);
    }
}


/*****************************************************************************
*   lcd_time
*   Display clock time as xx:yy:zz
*
*   Input: int input1
*               First number to display (xx)
*          int input2
*               Second number to display (yy)
*          int input3
*               Third number to display (zz)
*
*   Output: none
*
******************************************************************************/


void lcd_time(int input1, int input2, int input3)
{
    char  buf1[intlen(input1)],
          buf2[intlen(input2)],
          buf3[intlen(input3)];
          
    int2char(buf1,2,input1);
    int2char(buf2,2,input2);
    int2char(buf3,2,input3);
    
    lcd_string2(0,buf1);
    lcd_string2(2,":");
    lcd_string2(3,buf2);
    lcd_string2(5,":");
    lcd_string2(6,buf3);
    lcd_string2(8,"  ");
}


/*****************************************************************************
*   lcd_date
*   Display calendar time as xxxx/yy/zz
*
*   Input:  int input1
*               First number to display as year
*           int input2
*               Second number to display as month
*           int input3
*               Third number to display as day
*
*   Output: none 
******************************************************************************/


void lcd_date(int input1, int input2, int input3)
{
    char  buf1[intlen(input1)],
          buf2[intlen(input2)],
          buf3[intlen(input3)];
          
    int2char(buf1,4,input1);
    int2char(buf2,2,input2);
    int2char(buf3,2,input3);
    
    lcd_string2(0,buf1);
    lcd_string2(4,"/");
    lcd_string2(5,buf2);
    lcd_string2(7,"/");
    lcd_string2(8,buf3);
}
