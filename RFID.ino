/*
  PINOUT:
  RC522 MODULE    Uno/Nano     MEGA
  SDA             D10          D9
  SCK             D13          D52
  MOSI            D11          D51
  MISO            D12          D50
  IRQ             N/A          N/A
  GND             GND          GND
  RST             D9           D8
  3.3V            3.3V         3.3V
*/

#include <SPI.h>
#include <RFID.h>
#define SDA_DIO 9
#define RESET_DIO 8
double jash;
RFID RC522(SDA_DIO, RESET_DIO);

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  RC522.init();
  pinMode(13, OUTPUT);
}

void loop()
{
  jash = Serial.read();
  if (RC522.isCard())
  {
    RC522.readCardSerial();
    Serial.print("Card detected: ");
    for (int i = 0; i < 5; i++)
    {
      Serial.print(RC522.serNum[i], DEC);
    }
   Serial.println(jash);
  }


  delay(1000);
}
