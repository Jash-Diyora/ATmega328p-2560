//---#0--**UNCOMMENT THE CODE YOU WANT TO UPLOAD**--**CONTAINS 7 CODES FOR LED**--


/*
  #include<avr/io.h>
  #define F_CPU 16000000UL
  #include<util/delay.h>
  int main(void)
  {
  DDRF = 0xFF;
  while (1)
  {
    PORTF = 0b11111111;
    _delay_ms(500);
    PORTF = 0b00000000;
    _delay_ms(500);
    PORTF = 0b10000000;
    _delay_ms(500);
    PORTF = 0b11000000;
    _delay_ms(500);
    PORTF = 0b11100000;
    _delay_ms(500);
    PORTF = 0b11110000;
    _delay_ms(500);
    PORTF = 0b11111000;
    _delay_ms(500);
    PORTF = 0b11111100;
    _delay_ms(500);
    PORTF = 0b11111110;
    _delay_ms(500);
    PORTF = 0b11111111;
    _delay_ms(500);
    PORTF = 0b00000001;
    _delay_ms(500);
    PORTF = 0b00000011;
    _delay_ms(500);
    PORTF = 0b00000111;
    _delay_ms(500);
    PORTF = 0b00001111;
    _delay_ms(500);
    PORTF = 0b00011111;
    _delay_ms(500);
    PORTF = 0b00111111;
    _delay_ms(500);
    PORTF = 0b01111111;
    _delay_ms(500);
    PORTF = 0b11111111;
    _delay_ms(500);
  }
  }
*/

//---#1--------------------------------------------
/*
  #include <util/delay.h>
  #include <avr/io.h>


  int main(void)

  {
   while(1)
   {

    DDRF = 0xff;
    PORTF = 0b10101010;
    _delay_ms(300);
    PORTF = 0b01010101;
    _delay_ms(300);

   }
   return 0;
  }

*/
// --- #2-----------------------------------------------

/*
  #include<avr/io.h>
  #include<util/delay.h>

  int main(void)
  {
  DDRF = 0xff;

  while(1)
  {

    PORTF = 0b00000001;
    _delay_ms(100);
  PORTF = 0b00000010;
    _delay_ms(100);
  PORTF = 0b00000100;
    _delay_ms(100);
  PORTF = 0b00001000;
    _delay_ms(100);
  PORTF = 0b00010000;
    _delay_ms(100);
  PORTF = 0b00100000;
    _delay_ms(100);
  PORTF = 0b11000000;
    _delay_ms(100);

  }

  }

*/

// ---#3-----------------------------------------------

/*
  #include<avr/io.h>
  #include<util/delay.h>

  int main(void)
  {
  DDRF = 0xff;
  while(1)
  {
   int i=0;
  int p=1;
  for(i=0 ; i<6 ; i++)
   {
    p = p*2;
    PORTF = p;
    _delay_ms(1000);
    }

  }

  }
*/
// --- #4-----------------------------------------------

/*
  #include<avr/io.h>
  #include<util/delay.h>

  int main(void)
  {
  DDRF = 0xff;
  while(1)
  {
   int i;

  for(i=0 ; i<7 ; i++)
   {
    PORTF = pow(i,i);
    _delay_ms(1);
    Serial.begin(250000);
   Serial.println("\t benchod led sharu bandh thai che");
    }

  }

  }
*/
// ---#5-----------------------------------------------

/*
  #include<avr/io.h>
  #include<util/delay.h>

  int main(void)
  {
  DDRF = 0xff;
  int my_string[]={2,4,8,16,32,64};

  while(1)
  {
   int i=0;
   for(i=0 ; i<=8 ; i++)
   {
    PORTF = my_string[i];
    _delay_ms(50);
    }

  }

  }
*/
//---- #6----------------------------------------


/*
  #include <avr/io.h>
  #include <util/delay.h>

  int main(void)

  {

    int  z;
    DDRF=0xff;
    PORTF=0xff;
    for(z=0 ; z<8 ; z++)
  {
    PORTF=z;
   _delay_ms(100);
  }



  }

*/

