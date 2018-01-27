/*
   jash lcd.c

   Created: 9/22/2016 4:41:23 PM
   Author : Jash Diyora
*/


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

void lcdcommand (unsigned char jash)
{
  PORTF = 0xF0 & jash;
  PORTF &= ~(1 << PIN2); // RS0
  PORTF |= (1 << PIN3); // EN1
  _delay_ms(10);
  PORTF &= ~ (1 << PIN3); // EN0
  jash = jash << 4;
  PORTF = 0xF0 & jash;  // LOWER BITS
  PORTF &= ~ (1 << PIN2); // RS0
  PORTF |= (1 << PIN3); // EN1
  _delay_ms(10);
  PORTF &= ~ (1 << PIN3); // EN0
}

void lcddata (unsigned char data)
{
  PORTF = 0xF0 & data;
  PORTF |= (1 << PIN2);  //RS
  PORTF |= (1 << PIN3);  //EN1
  _delay_ms(10);
  PORTF &= ~ (1 << PIN3); //EN0
  data= data << 4;
  PORTF = 0xF0 & data;
  PORTF |= (1 << PIN2); //RS1
  PORTF |= (1 << PIN3); //EN1
  _delay_ms(10);
  PORTF &= ~ (1 << PIN3); //EN0

}

void lcdinstall ()
{
  DDRF |= 0xFF;
  lcdcommand(0x02);
  lcdcommand(0x28);
  lcdcommand(0X0C);
  lcdcommand(0x06);
  lcdcommand(0x01);
  lcdcommand(0x80);
  _delay_us(200);
}

void lcdxy(unsigned char x , unsigned char y)
{
  unsigned char jashline[] = {0x80, 0xC0, 0x90, 0xD0};
  lcdcommand(jashline[x - 1] + y - 1);
  _delay_ms(10);
}

void lcdprint( const char *str )
{
  while (*str != '\0' )
  {
    lcddata(*str);
    str++;
  }
}



int main(void)
{
  DDRF = 0xFF;

  while (1)
  {
    lcdinstall();
    lcdxy(1, 1);
    lcdprint(" TEAM UVPCE 5.0 ");
    lcdxy(2, 1);
    lcdprint("  ROBOCON 2017  ");
    lcdxy(3, 1);
    lcdprint("JASHKUMAR DIYORA");
    lcdxy(4, 1);
    lcdprint("   PROGRAMMER   ");
    return 0;
  }
}


