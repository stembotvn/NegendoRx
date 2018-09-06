#include <NegendoRx.h>
#include <SPI.h>
#include "RF24.h"

NegendoRx Negendo;
RF24 radio(CE_PIN, CSN_PIN);
int Data[8];
int BF,BB,BL,BR,BSP,BLED,BX;
int Speed;
int updated;

void Move()
{
  if(BSP == 0)
    Negendo.tone(500,200);
  else if(BF == 0)
    Negendo.moveForward(100);
  else if(BB == 0)
    Negendo.moveBackward(100);
  else if(BL == 0)
    Negendo.setServo(1,Left_angle);
  else if(BR == 0)
    Negendo.setServo(1,Right_angle);
  else if(BF==0 && BL==0)
    Negendo.moveLeft(100);
  else if(BF==0 && BR==0)
    Negendo.moveRight(100);
  else if(BB==0 && BL==0)
    Negendo.moveBackLeft(100);
  else if(BB==0 && BR==0)
    Negendo.moveBackRight(100);
  else
  {
    Negendo.stop();
    Negendo.setServo(1,Center_angle);
  }
}

void readRadio()
{
  if ( radio.available())
  {
    while(radio.available())
    {
      radio.read( Data, sizeof(Data));
      updated = 1;
    }
  }
  ////////////////////////////////
  if(updated)
  {
    BF = Data[0];
    BB = Data[1];
    BL = Data[2];
    BR = Data[3];
    BSP = Data[4];
    BLED = Data[5];
    BX = Data[6];
    Speed = Data[7];
    updated = 0;
  }
  Move();
}

void setup() 
{
  Negendo.init();
}
void loop()
{
  Negendo.setAddress();
  readRadio();
}

