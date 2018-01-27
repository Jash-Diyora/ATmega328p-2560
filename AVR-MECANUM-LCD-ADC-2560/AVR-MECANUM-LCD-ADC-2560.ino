/*
 * Author : Jash Diyora
 * jashdiyora7@gmail.com
 */


#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
char str[5]={0,0,0,0,0};
char str1[5]={0,0,0,0,0};
int x=0,y=0;
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
    lcddisplaystring("          ");
    lcdxy(3,9);
    lcddisplaystring(str);
    return 0;
}

void print2()
{
    lcddisplaystring("          ");
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
    lcddisplaystring("POT 2 :         ");}

int main(void)
{
    
    runtime();
    DDRA = 0x00;
    DDRB = 0xff;
    DDRD= 0xff;
    TCCR0A=0b10100011;
    TCCR0B=0b00001001;
    TCCR1A=0b10100011;
    TCCR1B=0b00001001;
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
            x=atoi(str);
            print1();
            ADMUX=0XC1;
            break;
            
        case 0XC1:
            itoa(highbits,str1,10);
            y=atoi(str1);
            print2();
            ADMUX=0XC0;
            break;
    }

    ADCSRA|=1<<ADSC;

   
    
    //OCR0A= (x/4);
    //OCR0B= (x/4);
    //OCR1A =(x/4);
   // OCR1B = (x/4);
    
    if((x>=615 & x<=645) && (y>=615 & y<=645)) // forward (x=1023) && (y>635&y<650)
    {
        OCR0A= 0;
        OCR0B= 0;
        OCR1A =0;
        OCR1B = 0;

    //    PORTB=(1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3);
     //   PORTD=(1<<PIND0)|(1<<PIND1)|(0<<PIND2)|(0<<PIND3);
    }
    
    else if((x>=650 & x<=1023) && (y>=615 & y<=635)) //backward  (x=0) && (y>620&y<630)
    {
        OCR0A= x;
        OCR0B= 0;
        OCR1A =0;
        OCR1B = 0;

      //  PORTB=(1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3);
      // PORTD=(0<<PIND0)|(0<<PIND1)|(1<<PIND2)|(1<<PIND3);
    }
    
    else if((x>=0 & x<=626) && (y>=625 & y<=645))//right   (x>630&x<640) && (y=1023)
    {
        OCR0A= 0;
        OCR0B= 0;
        OCR1A =x;
        OCR1B = 0;

        //PORTB=(1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3);
         //PORTD=(1<<PIND0)|(0<<PIND1)|(1<<PIND2)|(0<<PIND3);
    }
    
    else if((x>=635 & x<=645) && (y>=0 & y<=615)) //left   (x>635&x<645) && (y=0)
    {
        OCR0A= 0;
        OCR0B= 0;
        OCR1A =0;
        OCR1B = y;

        //PORTB=(1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3);
        // PORTD=(0<<PIND0)|(1<<PIND1)|(0<<PIND2)|(1<<PIND3);
    }
    
    else if((x>=0 & x<=645) && (y>=0 & y<=615)) //left   (x>635&x<645) && (y=0)
    {
        OCR0A= 0;
        OCR0B= y;
        OCR1A =0;
        OCR1B = 0;
        
        //PORTB=(1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3);
        // PORTD=(0<<PIND0)|(1<<PIND1)|(0<<PIND2)|(1<<PIND3);
    }

    
    else //stop
    {
        OCR0A= 0;
        OCR0B= 0;
        OCR1A =0;
        OCR1B = 0;

        //PORTB=(0<<PINB0)|(0<<PINB1)|(0<<PINB2)|(0<<PINB3);
        //PORTD=(0<<PIND0)|(0<<PIND1)|(0<<PIND2)|(0<<PIND3);
    }
}

