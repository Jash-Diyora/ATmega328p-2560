//----#1-----------------------------

#include<avr/io.h>
#define F_CPU 8000000UL
#include<util/delay.h>

int main(void)
{
  DDRB = 0xFF;
  while (1);
  {
    DDRB = 0b11111111;
    TCNT0 = 0;
    TCCR0B = 0b01001010;   //normal , prescale 1/8
    while (TIFR0 & TOV0 == 0);
    TCCR0B = 0;
    TIFR0 = 0b00000001;

    //timer clock 1 mhz , timer period 1Us , timer value 1000


  }
}

//---#2----------------------------


/*
 * #include<avr/io.h>
#define F_CPU 8000000UL
#include<util/delay.h>

int main(void)
{
  DDRB = 0xFF;
  while (1);
  {
    DDRB = 0b11111111;
    OCR0=3;
    TCNT0 = 0;
    TCCR0 = 0b01111101;   //TCCR0A = 0b11110011; FOR 328P
    while (1);
    return 0;
  }
}
*/

 //----#3------------
/*
 #include<avr/io.h>
#define F_CPU 8000000UL
#include<util/delay.h>

int main(void)
{
  DDRB = 0xFF;
  TCCR0 = 0b01101101;
  while (1)
  {
    DDRB |= (1 << 3);
    OCR0 = 25;
    jashdelay();
    OCR0 = 50;
    jashdelay();
    OCR0 = 75;
    jashdelay();
    OCR0 = 100;
    jashdelay();
    OCR0 = 125;
    jashdelay();
    OCR0 = 150;
    jashdelay();
    OCR0 = 175;
    jashdelay();
    OCR0 = 200;
    jashdelay();
    OCR0 = 225;
    jashdelay();
    OCR0 = 255;
  }

}

void jashdelay()
{
  for (int z = 0 ; z < 123; z++)
  {
    DDRB |= (1 << 3);
    OCR2 = 254;
    TCCR2 = 0b00000101;   // normal , prescale 1/1024
    while ((TIFR & (1 << OCF2)) = 0);
    TCCR2 = 0;
   // TIFR = 0b10000000;
  }
}



 
*/
