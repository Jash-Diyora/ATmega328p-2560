// AUTHOR: JASH DIYORA
//   TEAM UVPCE 5.0
//   JR. PROGRAMMER


#define F_CPU 8000000UL
#include <avr/io.h>

uint16_t read(uint8_t ch)
{
  ch = ch & 0b00000111;
  ADMUX &= 0b01100000;
  ADMUX |= ch;
  ADCSRA = 0B11000111 ;
  while ((ADCSRA & (1 << ADIF)==1));
  ADCSRA |= (1 << ADIF);
  return (ADC);
}


int main()
{

  Serial.begin(9600);
  ADMUX=(0<<REFS1)|(1<<REFS0);
  ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
  
  while (1)
  {
    int x = read(0);
    Serial.print(x);
    
    int y = read(1);
    Serial.print("----TEAM UVPCE 5.0----");
    Serial.println(y);

  }
}


