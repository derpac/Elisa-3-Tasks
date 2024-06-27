enum proxname{
  p_front =      0b00000001,
  p_frontright = 0b00000010,
  p_rightside =  0b00000100,
  p_backright =  0b00001000,
  p_back =       0b00010000,
  p_backleft =   0b00100000,
  p_leftside =   0b01000000,
  p_frontleft =  0b10000000
};

int proxthreshhold[8] = {820,820,820,820,820,820,820,820};
int leftspeed =0;
int rightspeed =0;

int Kr = 30;
int Kl = 20;
int Rr = 30;

void setup() {
  // put your setup code here, to run once:
DDRA = 0b11111111;


}

 void loop() {

  dostate();
  // put your main code here, to run repeatedly:
 
}

void dostate(){
  sensestate();
  leftforward(Kl * leftspeed);
  rightforward(Kr * rightspeed);
  }
  

void sensestate(){
  unsigned char proximity = readProxSensor();

  if (proximity & p_front){
    leftspeed = 2;
    rightspeed = 0;
  }
  else if(proximity & p_frontleft){
    leftspeed = 2;
    rightspeed = 0.77;
  }
  else if(proximity & p_frontright){
    leftspeed = 0.77;
    rightspeed = 2;
  }
  else if(proximity & p_back){
    leftspeed = 1;
    rightspeed = 1;
  }
  else if(proximity & p_backleft){
    leftspeed = 1;
    rightspeed = 1;
  }
  else if(proximity & p_backright){
    leftspeed = 1;
    rightspeed = 1;
  }
 else{
  leftspeed = 1;
  rightspeed = 1;

 }
}


unsigned char readProxSensor()
{
  
  unsigned char ret = 0;
  unsigned char mask = 1;
  proxLEDon();
  delay(3);
  for( int i = A0; i<A8; i++)
  {
    int reading =analogRead(i);
    if(reading < proxthreshhold[i-A0])
    {
      ret |= mask;
    }
   mask <<= 1;
  }
  proxLEDoff();
  return ret;
}

void proxLEDon()
{
  PORTA |= 0b11111111;
}

void proxLEDoff()
{
  PORTA &= 0b00000000;
}

void leftforward(float duty)
{
  analogWrite(7,0);
  analogWrite(6, duty);
}
void leftback(float duty)
{
  analogWrite(6,0);
  analogWrite(7, duty);
}
void leftstop(){
  analogWrite(6,0);
  analogWrite(7,0);
}
void rightforward(float duty)
{
  analogWrite(2,0);
  analogWrite(5, duty);
}
void rightstop()
{
  analogWrite(2,0);
  analogWrite(5,0);
  
}
void rightback(float duty)
{
  analogWrite(5,0);
  analogWrite(2, duty);
  
}
