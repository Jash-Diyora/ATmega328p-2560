#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};
#define sp 2
char sw;


struct package
{
  char  text[100] = "HIGH";
};


typedef struct package Package;
Package data;

struct package1
{
  char  text[100] = "LOW";
};


typedef struct package1 Package1;
Package1 data1;
void setup()
{
  pinMode(sp,INPUT);
  Serial.begin(115200);
  delay(1000);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);
}

void loop()
{
  sw=digitalRead(sp);
  if(sw==HIGH){
  
  myRadio.write(&data, sizeof(data)); 

  
  Serial.println(data.text);
 }
else
{

myRadio.write(&data1, sizeof(data1));

}delay(100);
}
