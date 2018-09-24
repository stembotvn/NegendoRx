#include <NegendoRx.h>

NegendoRx Negendo;
int Data[9];
int BF,BB,BL,BR,BZ,BLED,BX,BY;
int Speed;
int updated;

void Move()
{ 
  if(BF==1)
    Negendo.moveForward(Speed);
  else if(BB==1)
    Negendo.moveBackward(Speed);
  else if(BL==1)
    Negendo.setServo1(Left_angle);
  else if(BR==1)
    Negendo.setServo1(Right_angle);
  else if(BF==1 && BL==1)
    Negendo.moveLeft(100);
  else if(BF==1 && BR==1)
    Negendo.moveRight(Speed);
  else if(BB==1 && BL==1)
    Negendo.moveBackLeft(Speed);
  else if(BB==1 && BR==1)
    Negendo.moveBackRight(Speed);
  else if(BZ==1)
    Negendo.tones(500,200);
  else
  {
    Negendo.stop();
    Negendo.setServo1(Center_angle);
  }
}

void readRadio()
{
  if (Negendo.radio.available())
  {
    while(Negendo.radio.available())
    {
      Negendo.radio.read( Data, sizeof(Data));
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
    BZ = Data[4];
    BLED = Data[5];
    BX = Data[6];
    BY = Data[7];
    Speed = Data[8];
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

