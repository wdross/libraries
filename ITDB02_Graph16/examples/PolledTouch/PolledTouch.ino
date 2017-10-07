#define DCLK     6
#define CS       5  
#define DIN      4 
#define DOUT     3

#include "CFwTimer.h"

unsigned int TP_X,TP_Y;
void spistar()                                     //SPI Start
{
  digitalWrite(CS,HIGH);
  digitalWrite(DCLK,HIGH);
  digitalWrite(DIN,HIGH);
  digitalWrite(DCLK,HIGH);

}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPI Write Data
{
  unsigned char count=0;
  unsigned char temp;
  unsigned nop;
  temp=num;
  digitalWrite(DCLK,LOW);
  for(count=0;count<8;count++)
  {
    if(temp&0x80)
      digitalWrite(DIN,HIGH);
    else
      digitalWrite(DIN,LOW);

    temp=temp<<1; 

    digitalWrite(DCLK,LOW);                
    nop++;
    nop++;
    digitalWrite(DCLK,HIGH);
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
    digitalWrite(DCLK,HIGH);//DCLK=1; _nop_();_nop_();_nop_();                
    nop++;
    digitalWrite(DCLK,LOW);//DCLK=0; _nop_();_nop_();_nop_();
    nop++;
    if(digitalRead(DOUT)) Num++;
  }
  return(Num);
}

void AD7843(void)              
{
  spistar();  
  digitalWrite(CS,LOW);                    
  WriteCharTo7843(0x90); 
  digitalWrite(DCLK,HIGH);
  digitalWrite(DCLK,LOW); 
  TP_Y=ReadFromCharFrom7843();
  WriteCharTo7843(0xD0);   
  digitalWrite(DCLK,HIGH);
  digitalWrite(DCLK,LOW);
  TP_X=ReadFromCharFrom7843();
  digitalWrite(CS,HIGH);
}


void setup()
{
  Serial.begin(115200);
  Serial.println("Toucher");
  for (int p=3; p<7; p++)
    pinMode(p,OUTPUT);
  pinMode(DOUT,INPUT);
}

CFwTimer CheckInterval(20);
CFwTimer TouchTime(INFINITE);
void loop()
{
  unsigned char flag;
  unsigned char ss[6];	
  int lx,ly;
  static int tx, ty;

  if (CheckInterval.IsTimeout())
  {
    AD7843();
    lx=((TP_X-340)*10/144);
    if(lx>237) lx=237;
    ly=320-((TP_Y-320)/11);
    if(ly<0) ly=0;
    if (lx < 237 && ly > 0) {
      // declare 'touch'
      if (TouchTime.GetExpiredBy() == INFINITE) {
        TouchTime.SetTimer(0); // record time
        tx = lx; ty = ly; // lock in touch location
        Serial.print(tx); Serial.print(","); Serial.println(ty);
      }
    }
    else if (TouchTime.GetExpiredBy() != INFINITE)
      TouchTime.SetTimer(INFINITE);
    CheckInterval.IncrementTimer(40);
  }
}
