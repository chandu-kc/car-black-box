/*
 * File:   isr.c
 * Author: LENOVO
 *
 * Created on December 15, 2023, 10:16 PM
 */


#include <xc.h>
  int count=120;
void __interrupt() isr(void)
{
   
	static unsigned short count1;
     
	if (TMR0IF)
	{
		TMR0 = TMR0 + 8;

		if (count1++ == 20000) //for 1 seconds
		{
            count1 = 0;
            count--;
		}
		TMR0IF = 0;
	}
}
