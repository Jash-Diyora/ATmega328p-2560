#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include<avr/interrupt.h>

int junctionpulse = 0 , forwardlsa = 0, backwardlsa = 0, positions = 0 , l, g , h , i, j , lastdirection = 0 , irsensor = 0;

void INITADC()
{
  ADMUX =  (1 << REFS0) | (0 << REFS1) | (0 << ADLAR);               //as vcc   right justified   (1<<REFS1)
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

  // DDRB |= (1 << PIN6) ;  //PWM
  // DDRE |=  (1 << PIN4);  //PWM
  // DDRL |= (1 << PIN4);   //PWM
  DDRH |= (1 << PIN4) | (1 << PIN3); //PWM

  //TCCR1A |= (1 << COM1B1) | (1 << COM1C1) | (1 << WGM10);     //NON-INVERTED
  //TCCR1B |= (1 << CS10) | (1 << WGM12);                       //FAST PWM
  //TCCR3A |= (1 << COM3A1) | (1 << COM3B1) | (1 << WGM30);     //NON-INVERTED
  //TCCR3B |= (1 << CS30) | (1 << WGM32);                       //FAST PWM
  TCCR4A |= (1 << COM4A1) | (1 << COM4B1) | (1 << WGM40);     //NON-INVERTED
  TCCR4B |= (1 << CS40) | (1 << WGM42);                       //FAST PWM
  //TCCR5A |= (1 << COM5A1) | (1 << COM5B1) | (1 << WGM50);     //NON-INVERTED
  //TCCR5B |= (1 << CS50) | (1 << WGM52);                       //FAST PWM

  DDRD |= (0 << PIN0) | (0 << PIN1);        //INT 0 , INT 1
  PORTD |= (1 << PIN0) | (1 << PIN1);       // PULLUP INT0 , INT1

  EIMSK |= (1 << INT0) | (1 << INT1);                                               //INT0 INT1 ENABLE
  EICRA |= (1 << ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11);               //RISING EDGE

  sei();

  DDRB = 0xff; //DIR OMNI
  DDRK = 0X00; //IR SENSOR

  DDRA = 0b11100000;    //|= (0 << PIN0) | (0 << PIN1) | (0 << PIN2) | (0 << PIN3) | (0 << PIN4); BUTTONS
  PORTA = 0b00011111;   //|= (0 << PIN0) | (1 << PIN1) | (1 << PIN2) | (1 << PIN3) | (1 << PIN4); PULLUP

  while (1)
  {
    irsensor = ((PINK & 0b00000001)  && (PINK & 0b00000010));

    //OCR1B PA 12
    //OCR3B PA 2
    //OCR4B PA 6
    //OCR5B PA 45

    l = PINA & 0b00000001;
    g = PINA & 0b00000010;
    h = PINA & 0b00000100;
    i = PINA & 0b00001000;
    j = PINA & 0b00010000;

    forwardlsa = ADCconvert(0);
    backwardlsa = ADCconvert(1);

    Serial.print("FORWARD LSA:- ");
    Serial.print(forwardlsa);
    Serial.print("    BACKWARD LSA:- ");
    Serial.print(backwardlsa);
    Serial.print("    JUNC PULSE.:- ");
    Serial.print(junctionpulse);
    Serial.print("    POSITION:-  ");
    Serial.print(positions);
    Serial.print("    MOTION:- ");

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


    if ( (junctionpulse < positions) && (junctionpulse != positions))   //FORWARD
    {
      lastdirection = 1;

      if ((forwardlsa > 400 & forwardlsa < 520))
      {
        forward();
      }

      else if (forwardlsa < 400)
      {
        left();
      }

      else if (forwardlsa > 500)
      {
        right();
      }
    }



    else if ( (junctionpulse > positions) && (junctionpulse != positions))  //BACKWARD
    {
      lastdirection = 2;

      if ((forwardlsa > 400 & forwardlsa < 500))
      {
        backward();
      }

      else if (backwardlsa < 400)
      {
        left();
      }

      else if (backwardlsa > 500)
      {
        right();
      }
    }


    else if ( junctionpulse == positions)    //CALIBRATION
    {
      if (lastdirection == 1) // CALIBRATE FORWARD
      {
        calibrateforward();
        if (irsensor == 0)
        {
          stopped();
          lastdirection = 0;
        }
      }


      else if (lastdirection == 2)  // CALIBRATE BACKWARD
      {
        calibratebackward();
        if (irsensor == 0)
        {
          stopped();
          lastdirection = 0;
        }
      }
     
      else
      {
        lastdirection = 0;
        stopped();
      }

    }

    else
    {
     // lastdirection = 0;
      stopped();
    }

    Serial.flush();
  }
}


void forward()
{
  Serial.println("FORWARD");

  OCR4A = 50;  //PA 2
  OCR4B = 50;  //PB 12
  PORTB |= (1 << 5);
  PORTB &= ~ (1 << 4);
}

void calibrateforward()
{
  Serial.println("CALIBRATE FORWARD");

  OCR4A = 30;  //PA 2
  OCR4B = 30;  //PB 12
  PORTB |= (1 << 5);
  PORTB &= ~ (1 << 4);
}

void backward()
{
  Serial.println("BACKWARD");

  OCR4A = 50;  //PA 2
  OCR4B = 50;  //PB 12
  PORTB |= (1 << 4) ;
  PORTB &= ~ (1 << 5);
}

void calibratebackward()
{
  Serial.println("CALIBRATE BACKWARD");

  OCR4A = 30;  //PA 2
  OCR4B = 30;  //PB 12
  PORTB |= (1 << 4) ;
  PORTB &= ~ (1 << 5);
}

void left()
{
  Serial.println("LEFT");
  OCR4A = 50;  //PA 2
  OCR4B = 50;  //PB 12
  PORTB &= ~ (1 << 5);
  PORTB &= ~ (1 << 4);

}

void right()
{
  Serial.println("RIGHT");

  OCR4A = 50;  //PA 2
  OCR4B = 50;  //PB 12
  PORTB |=  (1 << 5);
  PORTB |=  (1 << 4);
}

void stopped()
{
  Serial.println("STOPPED");

  OCR4A = 0;  //PA 2
  OCR4B = 0;  //PB 12
  PORTB = 0X00;
}

ISR (INT0_vect)
{
  if ((junctionpulse < positions) && (junctionpulse != positions))
  {
    junctionpulse++;
  }
}


ISR (INT1_vect)
{
  if ((junctionpulse > positions) && (junctionpulse != positions))
  {
    junctionpulse--;
  }
}
