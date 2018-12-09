#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

#define ENABLE1 8
#define STEP1 2
#define DIR1 5

#define ENABLE2 8
#define STEP2 3
#define DIR2 6

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
unsigned int onflag = 1, bonflag = 1, conflag=1, a = 0, b = 0, c = 0, d = 0, event = 0, done = 0;
void setup()
{
  // Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  pinMode(ENABLE1, OUTPUT);
  pinMode(STEP1, OUTPUT);
  pinMode(DIR1, OUTPUT);

  pinMode(ENABLE2, OUTPUT);
  pinMode(STEP2, OUTPUT);
  pinMode(DIR2, OUTPUT);

  pinMode(7, INPUT);
  pinMode(4, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);

}

/*******************************************************/

void stepNow1(int totalSteps1)
{
  int i;
  for (i = 0; i < totalSteps1; ++i) {
    digitalWrite(STEP1, HIGH);
    delayMicroseconds(900);
    digitalWrite(STEP1, LOW);
    delayMicroseconds(900);
  }
}


void stepNow2(int totalSteps2)
{
  int j;
  for (j = 0; j < totalSteps2; ++j) {
    digitalWrite(STEP2, HIGH);
    delayMicroseconds(900);
    digitalWrite(STEP2, LOW);
    delayMicroseconds(900);
  }
}

void walkLeftDirections1(int STEPS_PER_ROTATION1) {
  digitalWrite(DIR1, LOW);
  stepNow1(STEPS_PER_ROTATION1);
}

void walkRightDirections1(int STEPS_PER_ROTATION1) {
  digitalWrite(DIR1, HIGH);
  stepNow1(STEPS_PER_ROTATION1);
}

void walkUpDirections2(int STEPS_PER_ROTATION2) {
  digitalWrite(DIR2, LOW);
  stepNow2(STEPS_PER_ROTATION2);
}

void walkDownDirections2(int STEPS_PER_ROTATION2) {
  digitalWrite(DIR2, HIGH);
  stepNow2(STEPS_PER_ROTATION2);
}


/**********************************************/
void loop()
{
  a = digitalRead(7);
  b = digitalRead(4);
  c = digitalRead(A4);
  d = digitalRead(A5);

  digitalWrite(7, HIGH); digitalWrite(4, HIGH); digitalWrite(A4, HIGH); digitalWrite(A5, HIGH);


  if (c == 1 && conflag == 1)
  {
    digitalWrite(0, HIGH);
    digitalWrite(1, LOW);
    delay(5000);
  }

  else if(c==0 && conflag==1)
  {
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    conflag=0;
  }

  else if (a == 0 &&  bonflag == 1)
  {
    digitalWrite(ENABLE2, LOW);
    walkDownDirections2(50);
  }

  else if (a == 1 && bonflag == 1)
  {
    digitalWrite(ENABLE2, LOW);
    walkUpDirections2(80);
    digitalWrite(ENABLE2, HIGH);
    bonflag = 0;
  }

  else {

    if (b == 1 &&  onflag == 1)
    {
      digitalWrite(ENABLE1, LOW);
      walkRightDirections1(50);
    }

    if (b == 0 && onflag == 1)
    {
      digitalWrite(ENABLE1, LOW);
      walkLeftDirections1(60);
      digitalWrite(ENABLE1, HIGH);
      onflag = 0;
    }
  }



  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "34 EC 23 A3") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();

    digitalWrite(ENABLE1, LOW);
    walkLeftDirections1(4000);
    digitalWrite(ENABLE2, LOW);
    walkUpDirections2(5000);
    Serial.println("main done");
    done = 1;
  }

  while (done == 1)
  {
    Serial.println("sub done");

    a = digitalRead(7);
    b = digitalRead(4);
    c = digitalRead(A4);
    d = digitalRead(A5);

    digitalWrite(7, HIGH); digitalWrite(4, HIGH); digitalWrite(A4, HIGH); digitalWrite(A5, HIGH);

    if (d == 1 && event == 0)
    {
      digitalWrite(1, HIGH);
      digitalWrite(0, LOW);
      delay(8000);
      event = 1;
      Serial.println(" step 1");
    }

    else if (d == 0 && event == 1)
    {
      digitalWrite(0, LOW);
      digitalWrite(1, LOW);
      event = 2;
      Serial.println(" step 2");
    }

    else if (d == 0 && event == 2)
    {
      digitalWrite(ENABLE2, LOW);
      walkDownDirections2(800);
      event = 3;
      Serial.println(" step 3");
    }

    else if (c == 1 && event == 3)
    {
      digitalWrite(0, HIGH);
      digitalWrite(1, LOW);
      delay(8000);
      event = 4;
      Serial.println(" step 4");
    }

    else if (c == 0 && event == 4)
    {
      digitalWrite(0, LOW);
      digitalWrite(1, LOW);
      event = 5;
      bonflag = 1; onflag = 1; 
      Serial.println(" step 5");
    }

    else if (event == 5)
    {
      if (a == 0 &&  bonflag == 1)
      {
        digitalWrite(ENABLE2, LOW);
        walkDownDirections2(50);
      }

      else if (a == 1 && bonflag == 1)
      {
        digitalWrite(ENABLE2, LOW);
        walkUpDirections2(80);
        digitalWrite(ENABLE2, HIGH);
        bonflag = 0;
      }

      else {

        if (b == 1 &&  onflag == 1)
        {
          digitalWrite(ENABLE1, LOW);
          walkRightDirections1(50);
        }

        if (b == 0 && onflag == 1)
        {
          digitalWrite(ENABLE1, LOW);
          walkLeftDirections1(60);
          digitalWrite(ENABLE1, HIGH);
          onflag = 0;
        }
      }
    }

    //    digitalWrite(ENABLE2, LOW);
    //    walkDownDirections2(800);
    //    digitalWrite(ENABLE1, LOW);
    //    walkRightDirections1(400);
  }
}

void home()
{

}

//
//digitalWrite(0, HIGH);
//  digitalWrite(1, LOW); back motor
