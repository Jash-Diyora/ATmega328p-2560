#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

int lsavalue,lsavalue2;
int junctionpulse = 0;
int positions = 0;



void INITADC()
{
  ADMUX =  (1 << REFS0) | (0 << REFS1) | (0 << ADLAR);          //as vcc   right justified   (1<<REFS1) |
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //interrupt enable  prescaler 128
}

int ADCconvert(int z)
{
  z = z & 0b00000111;
  ADMUX &= 0b11100000;
  ADMUX |= z;
  ADCSRA |= (1 << ADSC);

  while ((ADCSRA & (1 << ADIF)) == 0);
  return ADC;
}

int main()
{



  Serial.begin(9600);
  cli();
  INITADC();

  DDRB |= (1 << PIN6) ;
  DDRE |=  (1 << PIN4);
  DDRL |= (1 << PIN4);
  DDRH |= (1 << PIN4);

  TCCR1A |= (1 << COM1B1) | (1 << COM1C1) | (1 << WGM10);     //NON-INVERTED
  TCCR1B |= (1 << CS10) | (1 << WGM12);                       //FAST PWM
  TCCR3A |= (1 << COM3A1) | (1 << COM3B1) | (1 << WGM30);     //NON-INVERTED
  TCCR3B |= (1 << CS30) | (1 << WGM32);                       //FAST PWM
  TCCR4A |= (1 << COM4A1) | (1 << COM4B1) | (1 << WGM40);     //NON-INVERTED
  TCCR4B |= (1 << CS40) | (1 << WGM42);                       //FAST PWM
  TCCR5A |= (1 << COM5A1) | (1 << COM5B1) | (1 << WGM50);     //NON-INVERTED
  TCCR5B |= (1 << CS50) | (1 << WGM52);                       //FAST PWM

  DDRD |= (0 << PIN0) | (0 << PIN1);        //INT 0 ,1 output
  PORTD |= (1 << PIN0) | (1 << PIN1);       // PULLUP INT0 , INT1;
  EIMSK |= (1 << INT0) | (1 << INT1);                                               //INT0 INT1 ENABLE
  EICRA |= (1 << ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11);               //RISING EDGE
               //rising edge
  sei();

  DDRB = 0xff;                                                                       //DC 13  DD 12

  DDRK = 0b11100000;    //|= (0 << PIN0) | (0 << PIN1) | (0 << PIN2) | (0 << PIN3) | (0 << PIN4); BUTTONS
  PORTK = 0b00011111;   //|= (0 << PIN0) | (1 << PIN1) | (1 << PIN2) | (1 << PIN3) | (1 << PIN4); PULLUP


  while (1)
  {
OCR3B = 150;  //PA 2
  OCR1B = 154;  //PB 12
  PORTB = (1 << 0) | (0 << 1);
    //OCR1B = 150;  //PA 12
    //OCR3B = 150;  //PA 2
    //OCR4B = 150;  //PA 6
    //OCR5B = 150;  //PA 45

 


    lsavalue = ADCconvert(0);
    lsavalue2 = ADCconvert(1);
    Serial.print("lsa:- ");
    Serial.print(lsavalue);

    Serial.print("lsa2:- ");
    Serial.print(lsavalue2);

    
    Serial.print("    junc.:- ");
    Serial.print(junctionpulse);
    Serial.print("    pos:-  ");
    Serial.print(positions);
    Serial.print("    motion:- ");


    int l = PINK & 0b00000001;
    int g = PINK & 0b00000010;
    int h = PINK & 0b00000100;
    int i = PINK & 0b00001000;
    int j = PINK & 0b00010000;


    if (l == 0)
    {
      positions = 1;
    }

    if (g == 0)
    {
      positions = 2;
    }
    if (h == 0)
    {
      positions = 3;
    }
    if (i == 0)
    {
      positions = 4;
    }
    if (j == 0)
    {
      positions = 5;
    }



    if ( (junctionpulse < positions) && (junctionpulse != positions) && (positions != 0) )
    {
      forward();
    }

    else if ( (junctionpulse > positions) && (junctionpulse != positions) && (positions != 0))
    {
      backward();


    }

  /*  
    else if ( (lsavalue >= 0 && lsavalue <= 150) && (junctionpulse < positions) && (junctionpulse != positions) && (positions != 0) )
    {
      forwardleft(); //230
    }

    else if ( (lsavalue >= 650 && lsavalue <= 970) && (junctionpulse < positions) && (junctionpulse != positions) && (positions != 0) )
    {
      forwardright(); //650
    }

    else if ( junctionpulse < positions && junctionpulse != positions && positions != 0)
    {
      backwardleft();
    }

    else if (junctionpulse < positions && junctionpulse != positions && positions != 0 )
    {
      backwardright();
    }
*/
    else
    {
      stopped();
    }
  }
}


void forward()
{
  Serial.println("FORWARD");

  OCR3B = 150;  //PA 2
  OCR1B = 154;  //PB 12
  PORTB = (1 << 0) | (0 << 1);
}



void backward()
{

  Serial.println("BACKWARD");

  OCR3B = 150;  //PA 2
  OCR1B = 154;  //PB 12
  PORTB = (0 << 0) | (1 << 1) ;
  
}

void stopped()
{
  Serial.println("STOPPED");

  OCR3B = 0;  //PA 2
  OCR1B = 0;  //PB 12
  PORTB = (0 << 0) | (0 << 1);
  PORTB = (0 << 2) | (0 << 3);
}


void forwardleft()
{
  Serial.println("FORWARD LEFT");
  OCR4B = 85;  //PA 6
  OCR5B = 85;  //PA 45
  PORTB = (1<< 2) | (1 << 3);

}

void forwardright()
{
  Serial.println("FORWARD RIGHT");

  OCR4B = 85;  //PA 6
  OCR5B = 85;  //PA 45
  PORTB = (0 << 2) | (0 << 3);

}

void backwardleft()
{
  Serial.println("BACKWARD LEFT");

  OCR4B = 85;  //PA 6
  OCR5B = 85;  //PA 45
  PORTB = (0 << 2) | (1 << 3);

}

void backwardright()
{
  Serial.println("BACK RIGHT");

  OCR4B = 85;  //PA 6
  OCR5B = 85;  //PA 45
  PORTB = (1 << 2) | (0 << 3);
}




ISR (INT0_vect)
{

  if (junctionpulse < positions)
  {
    junctionpulse++;
  }
}


ISR (INT1_vect)
{
  if (junctionpulse > positions)
  {
    junctionpulse--;
  }

}


