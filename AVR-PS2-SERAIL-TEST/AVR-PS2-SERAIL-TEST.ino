#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define MISO 3   //data
#define MOSI 2   //cmnd 
#define SCK  1   //clock
#define SS   0   //attention

void INIT_SPI_MSTR()
{
  DDRB |= (1<<SS)|(1<<SCK)|(1<<MOSI);
  DDRB &=~(1<<MISO);
  SPCR |= (1<<SPE)|(1<<DORD)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0);
  PORTB|=(1<<SS);
}

int hey_slave(int cmnd)
{
    int data; 
    SPDR = cmnd;
    while(!(SPSR & (1<<SPIF)));
    _delay_us(170);
    data = SPDR;

    return(data);
}

void config_analog()
{
  int doneconfig=0;
  
  while(doneconfig !=0x73)
  {
           //put controller in config mode-43
          PORTB&=~(1<<SS);
           hey_slave(0x01);
           hey_slave(0x43);
           hey_slave(0x00);
           
           hey_slave(0x01); //enter confi
           hey_slave(0x00);
           _delay_us(1); 
           PORTB|=(1<<SS);
           _delay_us(10);

           //Turn on analog mode-44
          PORTB&=~(1<<SS);
           hey_slave(0x01);
           hey_slave(0x44);
           hey_slave(0x00);
           
           hey_slave(0x01); 
           hey_slave(0x03);
           hey_slave(0x00);
           hey_slave(0x00);
           hey_slave(0x00);
           hey_slave(0x00);
           _delay_us(1); 
           PORTB|=(1<<SS);
           _delay_us(10);

           //Exit config mode-43
          PORTB&=~(1<<SS);
           hey_slave(0x01);
           hey_slave(0x43);
           hey_slave(0x00);
           
           hey_slave(0x00); 
           hey_slave(0x5A);
           hey_slave(0x5A);
           hey_slave(0x5A);
           hey_slave(0x5A);
           hey_slave(0x5A);
           _delay_us(1); 
           PORTB|=(1<<SS);
           _delay_us(10);

           
           //polling mode-42
          PORTB&=~(1<<SS);
           hey_slave(0x01);
           doneconfig = hey_slave(0x42);
           hey_slave(0x00);
           
           hey_slave(0x00); 
           hey_slave(0x00);
           hey_slave(0x00);
           hey_slave(0x00);
           hey_slave(0x00);
            _delay_us(1); 
           PORTB|=(1<<SS);
           _delay_us(10);
   }
}

int main(void)
{
  Serial.begin(9600);
   INIT_SPI_MSTR();
   config_analog();
  
 int data0, data1, data2, data3, data4, data5,data6,data7,data8,data9;

 while(1)
  {
           //polling mode-42
          PORTB&=~(1<<SS);
           hey_slave(0x01);
           hey_slave(0x42);
           hey_slave(0x00);
           
           data0=hey_slave(0x00); 
           data1=hey_slave(0x00);
           data4=hey_slave(0x00);
           data5=hey_slave(0x00);
           data2=hey_slave(0x00);
           data3=hey_slave(0x00);
            
           
           _delay_us(1); 
           PORTB|=(1<<SS);
           _delay_us(10);

     Serial.print(data0);
     Serial.print(" ");
     Serial.print(data1);
     Serial.print(" ");
     Serial.print(data2);
     Serial.print(" ");
     Serial.print(data3);
      Serial.print(" ");
     Serial.print(data4);
     Serial.print(" ");
     Serial.println(data5);
 
  }

}

