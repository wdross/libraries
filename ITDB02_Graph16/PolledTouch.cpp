#include "PolledTouch.h"

#include <Arduino.h> // HIGH, LOW

unsigned int TP_X,TP_Y;
void spistar()                                     //SPI Start
{
  digitalWrite(T_CS,HIGH);
  digitalWrite(T_DCLK,HIGH);
  digitalWrite(T_DIN,HIGH);
  digitalWrite(T_DCLK,HIGH);
}

//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPI Write Data
{
  unsigned char count=0;
  unsigned char temp;
  unsigned nop;
  temp=num;
  digitalWrite(T_DCLK,LOW);
  for(count=0;count<8;count++)
  {
    if(temp&0x80)
      digitalWrite(T_DIN,HIGH);
    else
      digitalWrite(T_DIN,LOW);

    temp=temp<<1; 

    digitalWrite(T_DCLK,LOW);                
    nop++;
    nop++;
    digitalWrite(T_DCLK,HIGH);
    nop++;
    nop++;
  }
}

//**********************************************************
unsigned int ReadFromCharFrom7843()             //SPI Read Data
{ 
  unsigned nop;
  unsigned char count=0;
  unsigned int Num=0;
  for(count=0;count<12;count++)
  {
    Num<<=1;
    digitalWrite(T_DCLK,HIGH);//DCLK=1; _nop_();_nop_();_nop_();                
    nop++;
    digitalWrite(T_DCLK,LOW);//DCLK=0; _nop_();_nop_();_nop_();
    nop++;
    if(digitalRead(T_DOUT)) Num++;
  }
  return(Num);
}

void AD7843(void)              
{
  spistar();  
  digitalWrite(T_CS,LOW);                    
  WriteCharTo7843(0x90); 
  digitalWrite(T_DCLK,HIGH);
  digitalWrite(T_DCLK,LOW); 
  TP_Y=ReadFromCharFrom7843();
  WriteCharTo7843(0xD0);   
  digitalWrite(T_DCLK,HIGH);
  digitalWrite(T_DCLK,LOW);
  TP_X=ReadFromCharFrom7843();
  digitalWrite(T_CS,HIGH);
}


void ToucherSetup()
{
  pinMode(T_DOUT,OUTPUT);
  pinMode(T_DIN,OUTPUT);
  pinMode(T_CS,OUTPUT);
  pinMode(T_DCLK,OUTPUT);
  pinMode(T_DOUT,INPUT);
}

#define TOUCHER_INCREMENT 40
bool ToucherLoop(int &tx, int &ty, int TouchTime)
{
  static CFwTimer CheckInterval(TOUCHER_INCREMENT);
  static CFwTimer TouchTimer(INFINITE);
  static bool LastRet = false;
  int lx,ly;
  bool ret = false;

  if (CheckInterval.IsTimeout())
  {
    AD7843();
    lx=((TP_X-340)*10/144);
    if(lx>237) lx=237;
    ly=320-((TP_Y-320)/11);
    if(ly<0) ly=0;
    if (lx < 237 && ly > 0) {
      // declare 'touch'
      if (TouchTimer.GetExpiredBy() == INFINITE)
        TouchTimer.SetTimer(TouchTime); // record time
      if (TouchTimer.IsTimeout() && !LastRet) {
#ifdef POLLEDTOUCH_LANDSCAPE
        tx = 320 - ly; // y is measured from the other end
        ty = lx; // x becomes y
#else
        tx = lx; ty = ly; // post touch location
#endif
        ret = true; // 1 time only per touch
        LastRet = true;
      }
    }
    else if (TouchTimer.GetExpiredBy() != INFINITE) {
      // let go, not touching
      TouchTimer.SetTimer(INFINITE);
      LastRet = false;
    }
    CheckInterval.IncrementTimerUnlessWayBehind(TOUCHER_INCREMENT);
  }
  return(ret); // one time per touch will be true
}
