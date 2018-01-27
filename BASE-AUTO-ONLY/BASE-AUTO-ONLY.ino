
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>
#include <stdlib.h>


#define MISO 3   //data
#define MOSI 2   //cmnd 
#define SCK  1   //clock
#define SS   0   //attention

/* define
PC 7 PH4   
PD 6 PH3
DC 11 PB5 
DD 10 PB4

JUNCTION F 21 PD0
JUNCTION B 20 PD1

BUTTONS 
1 PA0
2 PA1
3 PA2
4 PA3
5 PA4

*/
int a;
int b;
int p = 0;
int q = 0;
int flag=2;

int data0, data1, data2, data3, data4, data5,data6,data7,data8,data9;

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
 INIT_SPI_MSTR();
 config_analog();
  
 
  
  DDRH |= (1 << PIN4) | (1 << PIN3);          // PC oc4B(7)  PD oc4A(6)  output
  DDRB |= (1 << PIN5) | (1 << PIN4);          // DC     (11)  DD     (10)       output
  
  DDRD  |= (0 << PIN0)|(0 << PIN1);   // int0(21) int1(20)   INPUT
  PORTD |= (1 << PIN0)|(1 << PIN1);   // int0(21) int1(20)   pullup

///////////   IR sensor pulldowns and outout //////////
  DDRK  |= (0<<0) | (0<<1);//IR output 
  PORTK &=~ (1<<0) | (1<<1); //pull DOWn for IR

/////////////////   Defination for buttons  ////////////////
     
  DDRA  |= (0<<0) | (0<<1)| (0<<2) | (0<<3);    //buttons output 
  PORTA |= (1<<0) | (1<<1)| (1<<2) | (1<<3);    //pull up for buttons

/////////////////     PWM for base motor C-D  ///////////////////
 
  TCCR4A |= (1<<COM4A1)|(1<<COM4B1)|(1<<WGM40);   
  //non inverted pwm generation on OC4A and OC4B 
  TCCR4B |= (1<<WGM42)|(1<<CS40);
  // fast pwn 8 bit mode 5

////////////////  INTERRUPT FOR JUNCTION PULSES ///////////////////
  
  EIMSK |= (1 << INT0)|(1<<INT1);                               //int0 enable
  EICRA |= (1 << ISC00) | (1 << ISC01)|(1 << ISC10) | (1 << ISC11);               //rising edge
  sei();

  while (1)
  {
  
           //polling mode-42
           PORTB&=~(1<<SS);
           hey_slave(0x01);
           hey_slave(0x42);
           hey_slave(0x00);
           
           data0=hey_slave(0x00); 
           data1=hey_slave(0x00);
            
           _delay_us(1); 
           PORTB|=(1<<SS);
           _delay_us(10);

    int a = ADCconvert(0);   //forward
    int b = ADCconvert(1);   //backward
    
    int g = PINK & 0b00000001;
    int h = PINK & 0b00000010; 
    
    Serial.print(g);
    Serial.print("   ");
    Serial.print(h);
    Serial.println("          ");

    if (data1 == 127)
    {
      q = 1;
    }
    if (data1 == 223)
    {
      q = 2;
    }
    if (data1 == 239)
    {
      q = 3;
    }
    if (data1 == 253)
    {
      q = 4;
    }
    if (data1 == 247)
    {
      q = 5;
    }


    if (p < q && p != q)
    {  flag = 0;
      if (a > 400 && a < 500 )
      {
        forward();
      }
      else if (a < 400)
      {
        left();
      }
      else if (a > 500)
      {
        right();
      }
    }

    else if (p > q && p != q)
    {  flag = 1;
      if (b > 400 && b < 500)
      {
        backward();
      }
      else if (b < 400)
      {
        left();
      }
      else if (b > 500)
      {
        right();
      }
    }

    else if (p==q && flag == 0 && g==1 && h==2)
    {
 if (a > 400 && a < 500 )
      {
        calforward();
      }
      else if (a < 400)
      {
        calleft();
      }
      else if (a > 500)
      {
        calright();
      }
    }
  
    else if (p==q && flag == 1 && g==1 && h==2)
    {
  if (b > 400 && b < 500)
      {
        calbackward();
      }
      else if (b < 400)
      {
        calleft();
      }
      else if (b > 500)
      {
        calright();
      }
    }
    
    else  
    {
      stopthebot();
    }


  }
}

void forward()
{
  Serial.println("FORWARD");
  
  OCR4A = 100;  //PC 6
  OCR4B = 100;  //PD 5
  PORTB |= (1 << PIN5);  //DC 11
  PORTB &=~ (1 << PIN4); //DD 10
}

void backward()
{ 
  Serial.println("Backward");
  OCR4A = 100;  //PC 6
  OCR4B = 100;  //PD 5
  PORTB &= ~(1 << PIN5); //DC 11  
  PORTB |= (1 << PIN4);   //DD 10
}

void left()
{
  Serial.println(" left");
  OCR4A = 50;        //PC 6
  OCR4B = 50;       //PD 5
  PORTB &= ~ (1 << PIN5); //DC 11
  PORTB &= ~ (1 << PIN4); //DD 10 
}

void right()
{
  Serial.println(" right");
  OCR4A = 50;        //PC 6
  OCR4B = 50;       //PD 5
  PORTB |= (1 << PIN5); //DC 11
  PORTB |= (1 << PIN4); //DD 10 
}

void calforward()
{
  Serial.println("CALFORWARD");
  
  OCR4A = 35;  //PC 6
  OCR4B = 35;  //PD 5
  PORTB |= (1 << PIN5);  //DC 11
  PORTB &=~ (1 << PIN4); //DD 10
}

void calbackward()
{ 
  Serial.println("CALBackward");
  OCR4A = 35;  //PC 6
  OCR4B = 35;  //PD 5
  PORTB &= ~(1 << PIN5); //DC 11  
  PORTB |= (1 << PIN4);   //DD 10
}

void calleft()
{
  Serial.println(" CALleft");
  OCR4A = 30;        //PC 6
  OCR4B = 30;       //PD 5
  PORTB &= ~ (1 << PIN5); //DC 11
  PORTB &= ~ (1 << PIN4); //DD 10 
}

void calright()
{
  Serial.println(" CALright");
  OCR4A = 30;        //PC 6
  OCR4B = 30;       //PD 5
  PORTB |= (1 << PIN5); //DC 11
  PORTB |= (1 << PIN4); //DD 10 
}

void stopthebot()
{
  Serial.println("I M ON HOLD");
  OCR4A = 00;           //PC 6
  OCR4B = 00;          //PD 5
}

ISR (INT0_vect)
{
  if (p < q && p != q)
  {
    p++;
  }
}

ISR (INT1_vect)
{
  if (p > q && p != q)
  {
    p--;
  }
}



