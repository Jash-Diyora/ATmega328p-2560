#include<avr/io.h>

#define SS 2
#define MOSI 3
#define MISO 4
#define SCK 5

int main(void)
{
  Serial.begin(9600);

  //OUTPUT - SS SCK MOSI //INPUT - MISO.
  DDRB =  (1 << MOSI) | (1 << SCK) | (1 << SS);

  //INTERRUPT DISABLE //SPI ENABLED //MASTER MODE.
  SPCR =  (1 << SPE) | (1 << MSTR) | (1 << SPR0);

  while (1)
  {
    SPDR =2;
    while (!(SPSR & (1 << SPIF))); //SPI STATUS //SPIF-INTERRUPT FLAG
    Serial.println(SPDR);
    _delay_ms(200);

//    SPDR = 2;
//    while (!(SPSR & (1 << SPIF))); //SPI STATUS //SPIF-INTERRUPT FLAG
//    Serial.println(SPDR);
//    _delay_ms(200);
//
//    SPDR = 3;
//    while (!(SPSR & (1 << SPIF))); //SPI STATUS //SPIF-INTERRUPT FLAG
//    Serial.println(SPDR);
//    _delay_ms(200);
  }

}
