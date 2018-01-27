
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#define SS 2
#define MOSI 3
#define MISO 4
#define SCK 5

char strj[5]={0,0,0,0,0};

void lcdcommand (unsigned char jash);
void lcddata (unsigned char data);
void lcdinstall ();
void lcdxy(unsigned char x , unsigned char y);
void lcdprint( const char *str );


int main(void)
{
  Serial.begin(9600);
  DDRD = 0xFF;

  //INPUT - SS SCK MOSI //OUTPUT - MISO.
  DDRB =  (1 << MISO);

  //SPI ENBLE SLAVE MODE
  SPCR = (1 << SPE);

  lcdinstall();
  lcdxy(1, 1);
  lcdprint(" TEAM UVPCE 5.0 ");
  lcdxy(2, 1);
  lcdprint("  ROBOCON 2017  ");
  lcdxy(3, 1);
  lcdprint("DIYORA  JASH .R.");
  lcdxy(4, 1);
  lcdprint(" Jr. PROGRAMMER ");
  _delay_ms(10);
  lcdcommand(0x01);

  while (1)
  {
    SPDR = 7;
    while (!(SPSR & (1 << SPIF)));

    itoa(SPDR, strj, 10);
    lcdxy(1, 1);
    lcdprint(strj);
    Serial.println(SPDR);
  }
}


void lcdcommand (unsigned char jash)
{
  PORTD = 0xF0 & jash;
  PORTD &= ~(1 << PIN2); // RS0
  PORTD |= (1 << PIN3); // EN1
  _delay_ms(5);
  PORTD &= ~ (1 << PIN3); // EN0
  jash = jash << 4;
  PORTD = 0xF0 & jash;  // LOWER BITS
  PORTD &= ~ (1 << PIN2); // RS0
  PORTD |= (1 << PIN3); // EN1
  _delay_ms(5);
  PORTD &= ~ (1 << PIN3); // EN0
}

void lcddata (unsigned char data)
{
  PORTD = 0xF0 & data;
  PORTD |= (1 << PIN2);  //RS
  PORTD |= (1 << PIN3);  //EN1
  _delay_ms(5);
  PORTD &= ~ (1 << PIN3); //EN0
  data = data << 4;
  PORTD = 0xF0 & data;
  PORTD |= (1 << PIN2); //RS1
  PORTD |= (1 << PIN3); //EN1
  _delay_ms(5);
  PORTD &= ~ (1 << PIN3); //EN0

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
  unsigned char jashline[] = {0x80, 0xC0, 0x90, 0xD0};
  lcdcommand(jashline[x - 1] + y - 1);
  _delay_ms(10);
}

void lcdprint( const char *str )
{
  while (*str != '\0' )
  {
    lcddata(*str);
    str++;
  }
}
