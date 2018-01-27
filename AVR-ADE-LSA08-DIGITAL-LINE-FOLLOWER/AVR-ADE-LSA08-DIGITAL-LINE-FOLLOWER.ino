#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define S1 PINA
#define S2 PINA>>1
#define S3 PINA>>2
#define S4 PINA>>3
#define S5 PINA>>4
#define S6 PINA>>5
#define S7 PINA>>6
#define S8 PINA>>7

int main(void)

{
    DDRK=0xFF;
    DDRF=0X00;
    PORTF=0X00;
    
    while (1)
    {
       
       // PORTK=0b01000001; //FRONT
       
       // PORTK=0b11000011; //BACK
       
       // PORTK=0b01000011; //LEFT

       // PORTK=0b11000001; //RIGHT
        
        
       if (PINF & ((1<<PINF3)|(1<<PINF4))) 
        {
           PORTK=0b01000001; //FRONT
        }
        
        
        else if (PINF & ((1<<PINF0)|(1<<PINF1)|(1<<PINF2)) ) 
        {
            PORTK=0b11000001; //RIGHT
        }
        
       
        else if (PINF & ((1<<PINF7)|(1<<PINF6)|(1<<PINF5)) ) 
        { 
            PORTK=0b01000011; //LEFT
        }

        
        else
        {
            PORTK=0b00000000;
        }


    }
   
}

