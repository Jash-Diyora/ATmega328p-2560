#include <SPI.h>  
#include "RF24.h" 
#define m1 2
#define m2 3

RF24 myRadio (7, 8); 
struct package
{
  char  text[100] ="empty";
};

byte addresses[][6] = {"0"}; 

struct package1
{
  char text[1000]="empty";
};

typedef struct package1 Package1;
Package1 data1;

typedef struct package Package;
Package data;

void setup() 
{
  Serial.begin(115200);
  delay(1000);
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}


void loop()  
{

  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
      myRadio.read( &data1, sizeof(data1) );
    }

    Serial.println(data.text);

    char c=Serial.read();
   Serial.println(                                                   c                );
    
  }

  

}
