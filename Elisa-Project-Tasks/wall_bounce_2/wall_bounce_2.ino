enum state{
  turnleft,
  turnright,
  forward,
  corner,
  reversethenleft,
  reversethenright,
  reversethencorner
};

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

enum groundname{
  g_leftback =   0b1000,
  g_leftfront =  0b0100,
  g_rightfront = 0b0010,
  g_rightback =  0b0001
};

enum state sta;
int anglehigh = 100, anglelow = 80;
//int proxthreshhold[8] = {700,700,650,600,650,750,700,640};
//int proxthreshhold[8] = {800,800,800,800,800,800,800,800};
int proxthreshhold[8] = {750,750,750,750,750,750,750,750};
int linecolour = 970;
int reversedelay = 100;
int Kl = 23;
int Kr = 30;
int leftspeed = 1;
int rightspeed = 1;

void setup() {
  // put your setup code here, to run once:

DDRA = 0b11111111;
DDRJ |= 0b00001111;
PORTJ |= 0b00001111;
Serial.begin(9600);
sta = forward;
}

void loop()
{
  // put your main code here, to run repeatedly:
  advancestate();
  dostate();

}
void dostate()
{
  switch(sta)
  {
    case forward:
      goforward();
      break;

    case reversethenleft:
    case reversethenright:
    case reversethencorner:
      goreverse();
      break;
    
    case turnleft:
      goturnleft();
      break;

    case turnright:
      goturnright();
      break;

    case corner:
      gocorner();
      break;

    
  }
}
void advancestate(){
    if(sta == reversethenleft)
    {
    sta = turnleft;
    }
    else if(sta == reversethenright)
    {
    sta = turnright;
    }
    else if(sta == reversethencorner)
    {
    sta = corner;
    }
    else{
    sensestate();
  }
}
void sensestate()
{
  unsigned char proximity = readProxSensor();
  bool blockedbehind = proximity & (p_back | p_backleft | p_backright) != 0;
  unsigned char ground = readGroundSensor();

  if(ground == (g_rightback | g_rightfront))
  {
    sta = blockedbehind ? turnleft : reversethenleft;
    anglehigh = 100;
    anglelow = 80;
  }
  else if(ground == (g_leftfront | g_leftback))    
  {
    sta = blockedbehind ? turnright : reversethenright ;
    anglehigh = 100;
    anglelow = 80;
  }
  else if(ground == (g_leftfront | g_rightfront))
  {
    sta = blockedbehind ? corner : reversethencorner;
    anglehigh = 100;
    anglelow = 90;
  }
  else if(ground == (g_leftfront | g_leftback | g_rightfront))    
  {
    sta = blockedbehind ? turnright : reversethenright ;
    anglehigh = 100;
    anglelow = 80;
  }
  else if(ground == (g_leftfront | g_rightback | g_rightfront))    
  {
    sta = blockedbehind ? turnleft : reversethenleft ;
    anglehigh = 100;
    anglelow = 80;
  }
  else if((proximity & p_front)){
    //sta = blockedbehind ? corner : reversethencorner;
    //anglehigh = 90;
    //anglelow = 80;
    leftspeed =2;
    rightspeed = 0;
  }
  else if((proximity & p_frontleft)){
    //sta = reversethenright;
    //anglehigh = 90;
    //anglelow = 80;
    leftspeed = 2;
    rightspeed = 0.77;
  }
  else if((proximity & p_frontright)){
    //sta = reversethenleft;
    //anglehigh = 90;
    //anglelow = 80;
    leftspeed = 0.77;
    rightspeed = 1.8;
  }
 // else if((proximity & p_leftside) != 0){
  //  sta = reversethenright;
   // anglehigh = 40;
   // anglelow = 30;
 // }
 // else if((proximity & p_rightside) != 0){
   // sta = reversethenleft;
   // anglehigh = 40;
  //  anglelow = 30;
 // }
  //else if(ground == g_leftback)
  //{
    //sta = turnright;
  //}
  //else if(ground == g_rightback)
 // {
   // sta = turnright;
  //}
  else
  {
    sta = forward;
    leftspeed = 1;
    rightspeed =1;
  }
}
void goforward()
{
    leftforward(Kl * leftspeed);
    rightforward(Kr + rightspeed);//move forward
}
void goreverse()
{
    rightback(40);
    leftback(30);
    delay(reversedelay);
}
void goturnright()
{
    rightstop();
    leftstop();
    //delay(500);
    int angle = random(anglelow,anglehigh);
    int turn = map(angle,0,360,0,1050);
    leftback(25);
    rightforward(34);
    delay(turn);
    rightstop();
    leftstop();
       //move right
}
void goturnleft()
{
    rightstop();
    leftstop();
    //delay(500);
    int angle = random(anglelow,anglehigh);
    int turn = map(angle,0,360,0,1050);
    leftforward(25);
    rightback(34);
    delay(turn);
    rightstop();
    leftstop();
          //move left
}
void gocorner()
{
    rightstop();
    leftstop();
    //delay(500);
    int angle = random(anglelow,anglehigh);
    int turn = map(angle,0,360,0,1050);
    leftback(25);
    rightforward(34);
    delay(turn);
    rightstop();
    leftstop();
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

unsigned char readGroundSensor()
{
  
  unsigned char ret = 0;
  unsigned char mask = 1;
  groundledson();
  delay(3);
  for( int i = A8; i<A12; i++)
  { 
    int reading =analogRead(i);
    if(reading > linecolour)
    {
      ret |= mask;
    }
   mask <<= 1;
  }
  groundledsoff();
  return ret;
}

void groundledon(unsigned char ledno)
{
  PORTJ &= ~(1<<ledno);
}
void groundledoff(unsigned char ledno)
{
  PORTJ |= (1<<ledno);
  
}
void groundledson()
{
  PORTJ &= 0b11110000;
}
void groundledsoff()
{
  PORTJ |= 0b00001111;
}
