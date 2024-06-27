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

enum state t2_sta;
int anglehigh = 100, anglelow = 80;
//int proxthreshhold[8] = {700,700,650,600,650,750,700,640};
//int t2_proxthreshhold[8] = {840,840,840,840,840,840,840,840};
int t2_proxthreshhold[8] = {750,750,750,750,750,750,750,750};
int linecolour = 970;
int t2_reversedelay = 150;
int t2_Kl = 26;
int t2_Kr = 44;
float t2_leftspeed = 1;
float t2_rightspeed = 1;

void setup() {
  // put your setup code here, to run once:

DDRA = 0b11111111;
DDRJ |= 0b00001111;
PORTJ |= 0b00001111;
Serial.begin(9600);
t2_sta = forward;
}

void loop()
{
  // put your main code here, to run repeatedly:
  t2_advancestate();
  t2_dostate();

}
void t2_dostate()
{
  switch(t2_sta)
  {
    case forward:
      t2_goforward();
      break;

    case reversethenleft:
    case reversethenright:
    case reversethencorner:
      t2_goreverse();
      break;
    
    case turnleft:
      t2_goturnleft();
      break;

    case turnright:
      t2_goturnright();
      break;

    case corner:
      t2_gocorner();
      break;

    
  }
}
void t2_advancestate(){
    if(t2_sta == reversethenleft)
    {
    t2_sta = turnleft;
    }
    else if(t2_sta == reversethenright)
    {
    t2_sta = turnright;
    }
    else if(t2_sta == reversethencorner)
    {
    t2_sta = corner;
    }
    else{
    sensestate();
  }
}
void sensestate()
{
  unsigned char proximity = t2_readProxSensor();
  bool blockedbehind = proximity & (p_back | p_backleft | p_backright) != 0;
  unsigned char ground = readGroundSensor();

  if(ground == (g_rightback | g_rightfront))
  {
    t2_sta = blockedbehind ? turnleft : reversethenleft;
    anglehigh = 80;
    anglelow = 60;
  }
  else if(ground == (g_leftfront | g_leftback))    
  {
    t2_sta = blockedbehind ? turnright : reversethenright ;
    anglehigh = 80;
    anglelow = 60;
  }
  else if(ground == (g_leftfront | g_rightfront))
  {
    t2_sta = blockedbehind ? corner : reversethencorner;
    anglehigh = 80;
    anglelow = 60;
  }
  else if(ground == (g_leftfront | g_leftback | g_rightfront))    
  {
    t2_sta = blockedbehind ? turnright : reversethenright ;
    anglehigh = 80;
    anglelow = 60;
  }
  else if(ground == (g_leftfront | g_rightback | g_rightfront))    
  {
    t2_sta = blockedbehind ? turnleft : reversethenleft ;
    anglehigh = 80;
    anglelow = 60;
  }
 
  
  else if((proximity & p_front)){
      t2_leftspeed = 1;
      t2_rightspeed = -1.5;
   
  }
  else if((proximity & p_frontleft)){
    t2_leftspeed = 1;
    t2_rightspeed = -1.5;
  }
  else if((proximity & p_frontright)){
    t2_leftspeed = -1.5;
    t2_rightspeed = 1;
  }
  else if(proximity & p_rightside){
    t2_rightspeed = 1.5;
    t2_leftspeed = 1;
  }
  else if(proximity & p_leftside){
    t2_rightspeed = 1;
    t2_leftspeed = 1.5;
  }
  else
  {
    t2_sta = forward;
    t2_leftspeed = 1;
    t2_rightspeed =1;
  }
}
void t2_goforward()
{
    leftforward(t2_Kl * t2_leftspeed);
    rightforward(t2_Kr * t2_rightspeed);   //move forward
}
void t2_goreverse()
{
    rightback(40);
    leftback(30);
    delay(t2_reversedelay);
}
void t2_goturnright()
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
void t2_goturnleft()
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
void t2_gocorner()
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
unsigned char t2_readProxSensor()
{
  
  unsigned char ret = 0;
  unsigned char mask = 1;
  proxLEDon();
  delay(3);
  for( int i = A0; i<A8; i++)
  {
    int reading =analogRead(i);
    if(reading < t2_proxthreshhold[i-A0])
    {
      ret |= mask;
    }
   mask <<= 1;
  }
  proxLEDoff();
  return ret;
}

void leftforward(float duty)
{
  if(duty < 0){
    leftback(-duty);
  }
  else{
  analogWrite(7,0);
  analogWrite(6, duty);
  }
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
  if(duty < 0){
    rightback(-duty);
  }
  else{
  analogWrite(2,0);
  analogWrite(5, duty);
  }
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
