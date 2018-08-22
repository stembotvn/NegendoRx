#include <NegendoRx.h>
#include <SPI.h>
#include "RF24.h"

NegendoRx Negendo;
RF24 radio(CE_PIN, CSN_PIN);
int Data[8]; //Received data array
bool updated = 0;
int F;
int B;
int L;
int R;
int X;
int Led;
int speaker;
int speeds;
int Speed;

void Move()
{
  if(F==1 && B==0 && R==0 && L==0 && Led==0 && X==0 && speaker==0)
  {
    Negendo.Forward(Speed, 90);
  }
  else if(F==0 && B==1 && R==0 && L==0 && Led==0 && X==0 && speaker==0)
  {
    Negendo.moveBackward(Speed);
  }
  else if(F==0 && B==0 && R==1 && L==0 && Led==0 && X==0 && speaker==0)
  {
    Negendo.moveRight(Speed);
  }
  else if(F==0 && B==0 && R==0 && L==1 && Led==0 && X==0 && speaker==0)
  {
    Negendo.moveLeft(Speed);
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
    F = Data[0];
    B = Data[1];
    R = Data[2];
    L = Data[3];
    Led = Data[4];
    X = Data[5];
    speaker = Data[6];
    speeds = Data[7];
    Speed = speeds/1024*100;
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

