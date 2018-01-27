
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>


  void lcdcommand (unsigned char jash)
  {
   PORTD = 0xF0 & jash;      
   PORTD &=~(1<<PIN2);   // RS0
   PORTD |= (1<<PIN3);   // EN1
   _delay_ms(5);
   PORTD &=~ (1<<PIN3);  // EN0
   jash = jash<<4;
   PORTD = 0xF0 & jash;  // LOWER BITS
   PORTD &=~ (1<<PIN2);   // RS0
   PORTD |= (1<<PIN3);   // EN1
   _delay_ms(5);
   PORTD &=~ (1<<PIN3);  // EN0
  }
  
  void lcddata (unsigned char data)
  {
   PORTD = 0xF0 & data;         
   PORTD |= (1<<PIN2);    //RS
   PORTD |= (1<<PIN3);    //EN1
   _delay_ms(5);
   PORTD &=~ (1<<PIN3);   //EN0
   data = data<<4;
   PORTD = 0xF0 & data;         
   PORTD |= (1<<PIN2);   //RS1
   PORTD |= (1<<PIN3);   //EN1
   _delay_ms(5);
   PORTD &=~ (1<<PIN3);  //EN0
    
  }
  
  void lcdinstall ()
  {
    DDRD |= 0xFF;
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
    unsigned char jashline[] = {0x80,0xC0,0x90,0xD0};
    lcdcommand(jashline[x-1] + y-1);
    _delay_ms(10);
  }
  
  void lcdprint( const char *str )
  {
    while (*str!='\0' )
    {
      lcddata(*str);
      str++;
    }
  }
  


int main(void)
{
  DDRD=0xFF;
  
    while (1) 
    {   
    lcdinstall();
    lcdxy(1,1);
    lcdprint(" TEAM UVPCE 5.0 ");
    lcdxy(2,1);
    lcdprint("  ROBOCON 2017  ");
    lcdxy(3,1);
    lcdprint("DIYORA  JASH .R.");
    lcdxy(4,1);
    lcdprint("AMIT BHAI       ");
    while(1);
    return 0;
    }   
}


