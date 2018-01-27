/*

BIT BANG PS2  for atmega640 -connection according to SPI


-------------LOOKING AT THE PLUG-------------------
        -------------------------------
 PIN 1->| o  o  o | o  o  o | o  o  o |
        \_____________________________/
    
PIN # USAGE
 1    DATA
 2    COMMAND
 3    N/C (9 Volts unused)
 4    GND
 5    VCC
 6    ATT
 7    CLOCK
 8    N/C
 9    ACK


----NOTES--
*> 0x5A(by controller) to say "here comes the data".
*> bite 1. header. (should possibly put test on this byte to detect unplugging of controller.)
 ie if (temp==0x73)
*/

#ifndef F_CPU
#define F_CPU 14745600UL // or whatever may be your frequency
#endif

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>


#define PSdata             3               // PB3
#define PScommand          2               // PB5
#define PSclock            1               // PB2
#define PSattention        0               // PB6

#define sbi(x,y)  x|=(1<<y)  
#define cbi(x,y)  x&=~(1<<y)
//#define _BV(y)  (1<<y)

// PSx controller communication function.
// send a byte on the command line and receive one on the data line.
// needs Attention pin to have gone low before called to activate controller.
int gameByte(short int command)
{
        short int i ; 
        _delay_us(1);
        short int data = 0x00;                             // clear data variable to save setting low bits later.
        
		for(i=0;i<8;i++)
        {
		 if(command & _BV(i))
		   { 
			 sbi(PORTB, PScommand);       // bit bang "command" out on PScommand wire.
		   } 
		 else 
		   {
			 cbi(PORTB, PScommand);
		   }
            
			
		 cbi(PORTB, PSclock);                             // CLOCK LOW
		 _delay_us(1);                                    // wait for output to stabilise
            
			
		 if((PINB & _BV(PSdata))) 
		  {
			 sbi(data, i);                               // read PSdata pin and store
		  }
		 else
		  { 
		     cbi(data, i);
		  }
		 	
		sbi(PORTB, PSclock);                             // CLOCK HIGH
        }
		
        sbi(PORTB, PScommand);        
        _delay_us(20);                                                   // wait for ACK to pass.
        return(data);
}






void  int_PS2inanalougemode()
{

      // this loop continues to put PSx controller into analouge mode untill the 
	   // controller responds with 0x73 in the 2nd byte.  
	   // (PS2 controller responds with 0x73 when in analouge mode.)
	   // the status LEDs will continue to count upwards untill a controller is found.
	   // if everything is working correctly this should happen on the first pass of 
	   // this loop but occasionally errors occur and a 2nd or 3rd itteration happen.
	   
	   unsigned char chk_ana = 0, cnt = 0;
	   
	   while(chk_ana != 0x73)
	   {
		   // put controller in config mode
		   sbi(PORTB, PScommand);
		   sbi(PORTB, PSclock);
		   cbi(PORTB, PSattention);
           
		   gameByte(0x01);
		   gameByte(0x43);
		   gameByte(0x00);
		   gameByte(0x01);
		   gameByte(0x00);
           
		   sbi(PORTB, PScommand);
		   _delay_ms(1);
		   sbi(PORTB, PSattention);
           
		   _delay_ms(10);
          
		   // put controller in analouge mode
		   sbi(PORTB, PScommand);
		   sbi(PORTB, PSclock);
		   cbi(PORTB, PSattention);
         
		   gameByte(0x01);
		   gameByte(0x44);
		   gameByte(0x00);
		   gameByte(0x01);
		   gameByte(0x03);
		   gameByte(0x00);
		   gameByte(0x00);
		   gameByte(0x00);
		   gameByte(0x00);
          
		   sbi(PORTB, PScommand);
		   _delay_ms(1);
		   sbi(PORTB, PSattention);
          
		   _delay_ms(10);
         
		   // exit config mode
		   sbi(PORTB, PScommand);
		   sbi(PORTB, PSclock);
		   cbi(PORTB, PSattention);
          
		   gameByte(0x01);
		   gameByte(0x43);
		   gameByte(0x00);
		   gameByte(0x00);
		   gameByte(0x5A);
		   gameByte(0x5A);
		   gameByte(0x5A);
		   gameByte(0x5A);
		   gameByte(0x5A);
            
		   sbi(PORTB, PScommand);
		   _delay_ms(1);
		   sbi(PORTB, PSattention);
           
		   _delay_ms(10);
         
		   // poll controller and check in analouge mode.
		   sbi(PORTB, PScommand);
		   sbi(PORTB, PSclock);
		   cbi(PORTB, PSattention);
          
		   gameByte(0x01);
		   chk_ana = gameByte(0x42);            // the 2nd byte to be returned from the controller should = 0x73 for "red" analouge controller.
		   gameByte(0x00);
		   gameByte(0x00);
		   gameByte(0x00);
		   gameByte(0x00);
		   gameByte(0x00);
		   gameByte(0x00);
		   gameByte(0x00);

		   sbi(PORTB, PScommand);
		   _delay_ms(1);
		   sbi(PORTB, PSattention);
		   _delay_ms(10);

	  // keep increasing counter to be dispalyed untill PSx controller confirms it's in analouge mode.
		   PORTC=cnt++;                     
		   if (cnt > 254){ cnt=0;}
   }


}




int main(void)
{

     DDRC=0XFF; 
     // PSx controller I/O pin setup:
	   sbi(DDRB, PB1);                 // clock. output. (blue)
        
	   cbi(DDRB, PB3);                 // data. input. (brown)
	   sbi(PORTB, PB3);               //    enable pullup resistor
    
        
	   sbi(DDRB, PB2);                 // command. output. (orange)
           
	   sbi(DDRB, PB0);                 // attention. output. (yellow)
	   
	   
	   
	   
	   int_PS2inanalougemode();
	   
	   
	   
	   short int temp, data0, data1, data2, data3, data4, data5, i ,debounceSelect;
	   
	   
	   while(1)
	   {
			sbi(PORTB, PScommand);                          // start communication with PSx controller
			sbi(PORTB, PSclock);
			cbi(PORTB, PSattention);
		 
			gameByte(0x01);                                       // bite 0. header.
			temp = gameByte(0x42);                          // bite 1. header. (should possibly put test on this byte to detect unplugging of controller.)
			gameByte(0x00);                                       // bite 2. header.
		  
			data0 = gameByte(0x00);                         // bite 3. first data bite.
			data1 = gameByte(0x00);                         // bite 4.
			data2 = gameByte(0x00);                         // bite 5.
			data3 = gameByte(0x00);                         // bite 6.
			data4 = gameByte(0x00);                         // bite 7.
			data5 = gameByte(0x00);                         // bite 8.
		   
			_delay_us(1);
			sbi(PORTB, PScommand);                      // close communication with PSx controller
			_delay_us(1);
			sbi(PORTB, PSattention);                        // all done.
			
		   PORTC=data5;
	   }

}
