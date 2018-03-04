/*
 * Author : Jash Diyora
 * jashdiyora7@gmail.com
 */


#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
char str[5]={0,0,0,0,0};
char str1[5]={0,0,0,0,0};

void lcdclear()
{
    lcdcommand(0x01);
}

void lcdcommand (unsigned char jash)
{
    PORTC = 0xF0 & jash;
    PORTC &=~(1<<PIN2);   // RS0
    PORTC |= (1<<PIN3);   // EN1
    _delay_ms(4);
    PORTC &=~ (1<<PIN3);  // EN0
    jash = jash<<4;
    PORTC = 0xF0 & jash;  // LOWER BITS
    PORTC &=~ (1<<PIN2);   // RS0
    PORTC |= (1<<PIN3);   // EN1
    _delay_ms(4);
    PORTC &=~ (1<<PIN3);  // EN0
}

void lcddata (unsigned char data)
{
    PORTC = 0xF0 & data;
    PORTC |= (1<<PIN2);    //RS
    PORTC |= (1<<PIN3);    //EN1
    _delay_ms(4);
    PORTC &=~ (1<<PIN3);   //EN0
    data = data<<4;
    PORTC = 0xF0 & data;
    PORTC |= (1<<PIN2);   //RS1
    PORTC |= (1<<PIN3);   //EN1
    _delay_ms(4);
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

void lcdxy(unsigned char x , unsigned char y)
{
    unsigned char jashline[] = {0x80,0xC0,0x90,0xD0};
    lcdcommand(jashline[x-1] + y-1);
    _delay_ms(10);
}

void lcddisplaystring(const char *label)
{
    while(*label!='\0')
    {
        lcddata(*label);
        label++;
    }
}

void print1()
{
    lcddisplaystring("               ");
    lcdxy(3,9);
    lcddisplaystring(str);
    return 0;
}

void print2()
{
    lcddisplaystring("                ");
    lcdxy(4,9);
    lcddisplaystring(str1);
    return 0;
}

void runtime()
{
    lcdinstall();
    lcdclear();
    lcdxy(1,1);
    lcddisplaystring(" TEAM UVPCE 5.0 ");
    lcdxy(2,1);
    lcddisplaystring("  ROBOCON 2017  ");
    lcdxy(3,1);
    lcddisplaystring("PROGRAMMED BY - ");
    lcdxy(4,1);
    lcddisplaystring("  JASH  DIYORA  ");
    _delay_ms(1000);
    lcdclear();
    
    lcdxy(1,1);
    lcddisplaystring(" TEAM UVPCE 5.0 ");
    lcdxy(2,1);
    lcddisplaystring("  ROBOCON 2017  ");
    lcdxy(3,1);
    lcddisplaystring("POT 1 :         ");
    lcdxy(4,1);
    lcddisplaystring("POT 2 :         ");
}

int main(void)
{
    runtime();
    DDRA = 0x00;
    ADCSRA|=1<<ADPS2;
    ADMUX|=1<<REFS0 | 1<<REFS1;
    ADCSRA|=1<<ADIE;
    ADCSRA|=1<<ADEN;
    
    sei();
    
    ADCSRA|=1<<ADSC;
    
    
    while(1)
    {
        //interrupt is on , do ntg
    }
}

ISR(ADC_vect)
{
    uint8_t lowbits = ADCL;
    uint16_t highbits = ADCH<<8|lowbits;
    
    switch (ADMUX)
    {
        case 0XC0:
            itoa(highbits,str,10);
            print1();
            ADMUX=0XC1;
            break;
            
        case 0XC1:
            itoa(highbits,str1,10);
            print2();
            ADMUX=0XC0;
            break;
    }
     ADCSRA|=1<<ADSC;     
}
