/*
 * ROBOCON LCD ADC.c
 *
 * Created: 9/30/2016 3:24:31 PM
 * Author : Jash Diyora
 */ 


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
char adcflag[5]={0,0,0,0,0};



void lcdcommand (unsigned char jash)
{
  PORTC = 0xF0 & jash;
  PORTC &=~(1<<PIN2);   // RS0
  PORTC |= (1<<PIN3);   // EN1
  _delay_ms(5);
  PORTC &=~ (1<<PIN3);  // EN0
  jash = jash<<4;
  PORTC = 0xF0 & jash;  // LOWER BITS
  PORTC &=~ (1<<PIN2);   // RS0
  PORTC |= (1<<PIN3);   // EN1
  _delay_ms(5);
  PORTC &=~ (1<<PIN3);  // EN0
}

void lcddata (unsigned char data)
{
  PORTC = 0xF0 & data;
  PORTC |= (1<<PIN2);    //RS
  PORTC |= (1<<PIN3);    //EN1
  _delay_ms(5);
  PORTC &=~ (1<<PIN3);   //EN0
  data = data<<4;
  PORTC = 0xF0 & data;
  PORTC |= (1<<PIN2);   //RS1
  PORTC |= (1<<PIN3);   //EN1
  _delay_ms(5);
  PORTC &=~ (1<<PIN3);  //EN0
  
}

void lcdinstall ()
{
  DDRC |= 0xFF;
  lcdcommand(0x02);
  lcdcommand(0x28);
  lcdcommand(0X0C);
  lcdcommand(0x06);
  lcdcommand(0x01);
  lcdcommand(0x80);
  _delay_us(200);
}

void lcdclear()
{
  lcdcommand(0x01);
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

void adcinstall()
{
  ADMUX=(1<<REFS1)|(1<<REFS0)|(0<<ADLAR);
  ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

/*void adcchannel()
{
  DDRA= 0x00;
  ADCSRA |= (1<<ADSC);
  while ((ADCSRA & (1 << ADIF)==1));
  return 0;
}
*/

void ADCconvert(int z)
{
  ADMUX = (ADMUX|z);
  ADCSRA |= (1 << ADSC);
  
  while ((ADCSRA & (1 << ADIF)) == 0);
  
  return 0;
}
void displayadc()
{
  itoa(ADC,adcflag,10);
  lcdprint("        "); 
}    
  

int main(void)
{
  lcdinstall();
  lcdxy(1,1);
  lcdprint(" TEAM UVPCE 5.0 ");
  lcdxy(2,1);
  lcdprint("HEY! NIKHIL BHAI");
  lcdxy(3,1);
  lcdprint("HERE IS MY TASK");
  lcdxy(4,1);
  lcdprint("INSTALLING ADC..");
  _delay_ms(2000);

  lcdclear();

  adcinstall();
  
  while(1)
  {
    
    lcdxy(1,1);
    lcdprint("POTENTIOMETER #1");
    lcdxy(2,1);
    lcdprint("  ADC VALUES:-  ");
    
    ADCconvert(0);
    displayadc();
    lcdxy(4,6);
    lcdprint(adcflag);

    return 0;
  }

}

