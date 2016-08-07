
/*****************************************************************************
*	Register address declaration 
******************************************************************************/

#ifndef	IO_H
	#define IO_H
#endif

#define P1DDR (*(volatile unsigned char*)0xfffc0)
#define P2DDR (*(volatile unsigned char*)0xfffc1)
#define P1DR (*(volatile unsigned char*)0xfffc2)
#define P2DR (*(volatile unsigned char*)0xfffc3)
#define P3DDR (*(volatile unsigned char*)0xfffc4)
#define P4DDR (*(volatile unsigned char*)0xfffc5)
#define P3DR (*(volatile unsigned char*)0xfffc6)
#define P4DR (*(volatile unsigned char*)0xfffc7)
#define P5DDR (*(volatile unsigned char*)0xfffc8)
#define P6DDR (*(volatile unsigned char*)0xfffc9)
#define P5DR (*(volatile unsigned char*)0xfffcA)
#define P6DR (*(volatile unsigned char*)0xfffcB)
//#define  (*(volatile unsigned char*)0xfffcC)
#define P8DDR (*(volatile unsigned char*)0xfffcD)
#define P7DR (*(volatile unsigned char*)0xfffcE)
#define P8DR (*(volatile unsigned char*)0xfffcF)
#define P9DDR (*(volatile unsigned char*)0xfffd0)
#define PADDR (*(volatile unsigned char*)0xfffd1)
#define P9DR (*(volatile unsigned char*)0xfffd2)
#define PADR (*(volatile unsigned char*)0xfffd3)
#define PBDDR (*(volatile unsigned char*)0xfffd4)
//#define  (*(volatile unsigned char*)0xfffd5)
#define PBDR (*(volatile unsigned char*)0xfffd6)
//#define  (*(volatile unsigned char*)0xfffd7)
#define P2PCR (*(volatile unsigned char*)0xfffd8)
//#define  (*(volatile unsigned char*)0xfffd9)
#define P4PCR (*(volatile unsigned char*)0xfffdA)
#define P5PCR (*(volatile unsigned char*)0xfffdB)