enum groundname{
  g_leftback =   0b0001,
  g_leftfront =  0b0010,
  g_rightfront = 0b0100,
  g_rightback =  0b1000
};
enum junctions{
  start,
  offstart,
  junc_A,
  junc_B,
  junc_C,
  junc_D,
  junc_E,
  junc_F,
  junc_G,
  junc_H,
  junc_I, 
  junc_J,
  junc_K,
  junc_L,
  junc_M,
  junc_N,
  junc_O,
  junc_P,
  junc_BFLH,
  end
};
String junctionnames[] = {
  "start",
  "offstart",
  "junc_A",
  "junc_B",
  "junc_C",
  "junc_D",
  "junc_E",
  "junc_F",
  "junc_G",
  "junc_H",
  "junc_I", 
  "junc_J",
  "junc_K",
  "junc_L",
  "junc_M",
  "junc_N",
  "junc_O",
  "junc_P",
  "junc_BFLH",
  "end"
};


enum actions{
  forward,
  reverse,
  leftturn,
  rightturn,
  stop
};
String actionnames[] = {
  "forward",
  "reverse",
  "leftturn",
  "rightturn",
  "stop"
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

enum actions action = forward;
enum junctions junction = start;
float speed_l = 27;
float speed_r = 35;

float leftsteer_l = -26;
float leftsteer_r = 42;
float rightsteer_l = 24;
float rightsteer_r = -38;

float highleftsteer_l = -34;
float highleftsteer_r = 44;
float highrightsteer_l = 30;
float highrightsteer_r = -38;

float lowleftsteer_l = -20;
float lowleftsteer_r = 30;
float lowrightsteer_l = 20;
float lowrightsteer_r = -30;

float wf_leftsteer_l = -22;
float wf_leftsteer_r = 33;
float wf_rightsteer_l = 20;
float wf_rightsteer_r = -30;

int linecolour = 970;
int proxthreshholdmed[8] = {750,750,860,750,750,750,860,700};
int proxthreshholdclose[8] = {300,300,300,300,300,300,300,300};
int proxwallthreshclose[8] = {500,500,500,500,500,500,500,500};
int proxwallthresh[8] = {830,830,800,800,800,800,800,830};
bool usewallthresh = false;
bool turnwhitedetect = false;
bool linefollow = false;
bool wallfollow = false;
int countsinceleft = 0;
int countsinceright = 0;
int countsincethresh = 13;
bool bothdetect = false;
bool countingleft = false;
bool countingright = false;
int wallfollowcount = 20;
int wallfollowcountdown = 0;
int wallturndelay = 5;
int wallmovedelay = 5;
bool motorson = true;


void setup() {
  // put your setup code here, to run once:
  DDRA = 0b11111111;
  DDRJ |= 0b00001111;
  PORTJ |= 0b00001111;
  DDRL |= 0b11110111;
  DDRG |= 0b00001000;
  Serial.begin(9600);
  GreenLEDoff();
}

void t3_loop() {
  // put your main code here, to run repeatedly:
unsigned char ground = readGroundSensor();
unsigned char medproximity, closeproximity;
int *proxvalues = readProxSensor(medproximity, closeproximity);
doaction(ground, medproximity, closeproximity, proxvalues);
advancestate(ground, medproximity, closeproximity);
}

void changestate(enum junctions junc){
  junction = junc;
  countingleft = false;
  countingright = false;
}
void doaction(unsigned char ground, unsigned char proximity, unsigned char closeproximity, int *proxvalues){
  switch(action)
  {
    case forward:
  
      goforward(ground, proximity, closeproximity, proxvalues);
      break;
    
    case leftturn:
      goturnleft(ground, proximity);
      break;
    case rightturn:
      goturnright(ground, proximity);
      break;
    case reverse:
      goreverse();
      break;
    case stop:
      gostop();
      break;
      
  }
}

void advancestate(unsigned char ground,unsigned char proximity,unsigned char closeproximity){

 // if(junction != end && (ground == (g_leftback | g_rightback | g_rightfront | g_leftfront))){/
   // junction = start;
   // action = forward;
   // linefollow = false;
  //  wallfollowleft = false;
   // wallfollowright = false;
    //usewallthresh = false;
   // leftsteer_l = highleftsteer_l;
   // leftsteer_r = highleftsteer_r;
   // rightsteer_l = highrightsteer_l;
   // rightsteer_r = highrightsteer_r;
    //Serial.println("start");
 // }
  if(junction != end && (ground == (g_leftback | g_rightback | g_rightfront | g_leftfront))){
    junction = junc_D;
    action = forward;
    wallfollowcountdown = wallfollowcount;
    linefollow = true;
    wallfollow = false;
    usewallthresh = false;
    leftsteer_l = lowleftsteer_l;
    leftsteer_r = lowleftsteer_r;
    rightsteer_l = lowrightsteer_l;
    rightsteer_r = lowrightsteer_r;
    //Serial.println("start");
  }
  else{
    bool bothdetected = false;
    bool leftdetect = false;
    bool rightdetect = false;
  
    if(action == forward || action == reverse){
      if(ground & g_leftback){
        if(!countingleft){
          if(countingright && countsinceright < countsincethresh){
            bothdetect = true;
          }
          else{
            countingleft = true;
            countsinceleft = 0;
          }
        }
        else{
          if(++countsinceleft > countsincethresh){
            countingleft = false;
            leftdetect = true;
          }
        }
      }
      if(ground & g_rightback){
        if(!countingright){
          if(countingleft && countsinceleft < countsincethresh){
            bothdetect = true;
          }
          else{
            countingright= true;
            countsinceright = 0;
          }
        }
        else{
          if(++countsinceright > countsincethresh){
            countingright = false;
            rightdetect = true;
          }

        }
      }
      if(bothdetect && (((ground & g_rightback) == 0) || ((ground & g_leftback) == 0))){
        bothdetected = true;
        bothdetect = false;
        countingleft = countingright = false;
      }
      else{
        bothdetected = false;
      }
    }


    if(!motorson){
      Serial.print("ground=");
      Serial.print(ground,BIN);
      Serial.print(" proximity=");
      Serial.print(proximity,BIN);
      Serial.print(" junction=");
      Serial.print(junctionnames[junction]);
      Serial.print(" action=");
      Serial.print(actionnames[action]);
      Serial.print(" linefollow=");
      Serial.print(linefollow);
      Serial.print(" bothdetect=");
      Serial.print(bothdetect);
      Serial.print(" bothdetected=");
      Serial.print(bothdetected);     Serial.println("");
    }
    switch(junction)
    {
      case start:
      GreenLEDoff();
      if(ground == 0){
        changestate(offstart);
        linefollow = true;
      }
      break;
      case offstart:
      linefollow = true;
      GreenLEDon(5);

      if(bothdetected){

          changestate(junc_BFLH);
          leftsteer_l = lowleftsteer_l;
          leftsteer_r = lowleftsteer_r;
          rightsteer_l = lowrightsteer_l;
          rightsteer_r = lowrightsteer_r;
      }

      if(proximity & (p_frontright | p_front)){
        startturnleft();           //junction M
      }
      else if(proximity & p_frontleft){
        startturnright();         //junction N
      }
      //else if(action == turningleft){
       // junction = junc_G;
        //startturnleft();
//}
      //else if(action == turningright){
       // junction = junc_A;
        //startturnright();
      //}
      //else if(leftdetect){
       // junction = junc_G;
        //startturnleft();
      //}
      //else if(rightdetect){
        //junction = junc_A;
        //startturnright();
      //}
     
      
      
      break;

      case junc_A:
      GreenLEDon(0);
      if(action == forward){
        linefollow = true;
      }
      
      
      break;

      case junc_B:
      if(bothdetected){
        changestate(junc_C);
        startturnleft();
      }
      break;

      case junc_C:
      GreenLEDon(7);
      if(bothdetected){
        changestate(junc_D);
        startturnright();
        wallfollowcountdown = wallfollowcount;

      }
      break;

      case junc_D:
      GreenLEDon(linefollow ? 1 : 7);
      if(wallfollowcount > 0){
        if(--wallfollowcount == 0){
          usewallthresh = true;
        }
      }
      if(linefollow && wallfollowcount == 0){
        if(proximity & (p_leftside | p_rightside | p_frontleft | p_frontright)){
          linefollow = false;
          wallfollow = true;
        }
        if(wallfollow){
          if(proximity & p_front){
            startturnright();
          }
          if(ground == (g_leftback | g_rightback | g_rightfront | g_leftfront)){
            changestate(end);
            action = stop;
          }
        }
      }
      break;

      case junc_E:
      if(bothdetected){
        if(action == reverse){
          action = forward;
        }
        else{
          changestate(junc_F);
          startturnleft();
        }
      }
      break;

      case junc_F:
      GreenLEDon(3);
      if(bothdetected){
        changestate(junc_B);
        startturnright();
      }
      break;

      case junc_G:
      GreenLEDon(1);
      if(action == forward){
        linefollow = true;
      }
      
    
      break;

      case junc_H:
      if(bothdetected){
        changestate(junc_I);
        startturnright();
      }
      break;

      case junc_I:
      GreenLEDon(6);
      if(bothdetected){
        changestate(junc_J);
        startturnleft();
        wallfollowcountdown = wallfollowcount;
      }
      break;

      case junc_J:
      if(wallfollowcount > 0){
        if(--wallfollowcount == 0){
          usewallthresh = true;
        }
      }
      if(linefollow && wallfollowcount == 0){
        if(proximity & (p_rightside | p_leftside | p_frontleft | p_frontright)){
          linefollow = false;
          wallfollow = true;
        }
        if(wallfollow){
          if(proximity & p_front){
            startturnleft();
          }
          if(ground == (g_leftback | g_rightback | g_rightfront | g_leftfront)){
            changestate(end);
            action = stop;
          }
        }
      }
      break;

      case junc_K:
      if(bothdetected){
        if(action == reverse){
          action = forward;
        }
        else{
          changestate(junc_L);
          startturnright();
        }
      }
      break;

      case junc_L:
      GreenLEDon(2);
      if(bothdetected){
        changestate(junc_H);
        startturnleft();
      }
      break;

      case junc_BFLH:
      GreenLEDon(4);
      if(bothdetected && (proximity & p_rightside)){
        changestate(junc_C);
        startturnleft();
      }
      else if(bothdetected && (proximity & p_leftside)){
        changestate(junc_I);
        startturnright();
      }
      else if((proximity & (p_frontleft | p_front) == (p_frontleft | p_front)) || (closeproximity & p_frontleft)) {
        changestate(junc_E);
        action = reverse;
      }
      else if((proximity & (p_frontright | p_front) == (p_frontright | p_front)) || (closeproximity & p_frontright)){
        changestate(junc_K);
        action = reverse;
      }

      break;

      case end:
      if(ground == 0){
          changestate(start);
          action = forward;
        }
      break;
    }
  }
}





void goforward(unsigned char ground, unsigned char proximity, unsigned char closeproximity, int *proxvalues)
{
    bool steerleft = false;
    bool steerright = false;

    if(linefollow){
      steerleft = ((ground & g_leftfront) && !(ground & g_rightfront));
      steerright = ((ground & g_rightfront) && !(ground & g_leftfront));
      if(steerleft){
      rightforward(leftsteer_r);                   // Turning left
      leftforward(leftsteer_l);
      }
      else if(steerright){
        leftforward(rightsteer_l);                 // Turning right
        rightforward(rightsteer_r);
      }
      else{
        leftforward(speed_l);
        rightforward(speed_r);
      }

    }

    else if(wallfollow){

      int leftvalue = max(proxvalues[6], proxvalues[7]);
      int rightvalue = max(proxvalues[1], proxvalues[2]);

      if(leftvalue  < rightvalue)
      {
        steerright = true;
      }
      else
      {
        steerleft = true;
      }

      //steerleft = (proximity & (p_rightside | p_frontright)); //|| (closeproximity & (p_rightside | p_frontright));
      //steerright = (proximity & (p_leftside | p_frontleft));
    
      if(steerleft){
        rightforward(wf_leftsteer_r);                   // Turning left
        leftforward(wf_leftsteer_l);
        delay(wallturndelay);
      }
      else if(steerright){
        leftforward(wf_rightsteer_l);                 // Turning right
        rightforward(wf_rightsteer_r);
        delay(wallturndelay);
      }
      //else{
      //  leftforward(speed_l);
       // rightforward(speed_r);
       // delay(wallmovedelay);
      //}
      leftforward(speed_l);
      rightforward(speed_r);
      delay(wallmovedelay);
    }
}

    


void goreverse()
{
    rightback(speed_r);
    leftback(speed_l);
}
void startturnleft(){
  action = leftturn;
  turnwhitedetect = false;
}
void goturnleft(unsigned char ground, unsigned char proximity)
{
  bool stopturning = false;
  if(wallfollow){
    stopturning = !(proximity & p_front);
  }
  if(linefollow){
    
    if(turnwhitedetect == false && (ground & g_leftfront) == 0){
      turnwhitedetect = true;
    }
    else if(turnwhitedetect && (ground & g_leftfront)){
      stopturning = true;
    }
  }
  if(stopturning){
    action = forward;
  }
  else{
    rightforward(speed_r);
    leftforward(-speed_l);
  }

}
void startturnright(){
  action = rightturn;
  turnwhitedetect = false;
}
void goturnright(unsigned char ground, unsigned char proximity)
{
  bool stopturning = false;
  if(wallfollow){
    stopturning = !(proximity & p_front);
  }
  if(linefollow){
    if(turnwhitedetect == false && (ground & g_rightfront) == 0){
      turnwhitedetect = true;
    }
    else if(turnwhitedetect && (ground & g_rightfront)){
      stopturning = true;
    }
  }
  if(stopturning){
    action = forward;
  }
  else{
    leftforward(speed_l);
    rightforward(-speed_r);
  }
}
void gostop(){

  leftstop();
  rightstop();
}

void leftforward(float duty)
{
  if(duty < 0){
    leftback(-duty);
  }
  else if(motorson){
    analogWrite(7,0);
    analogWrite(6, duty);
  }
}
void leftback(float duty)
{
  if(motorson){
    analogWrite(6,0);
    analogWrite(7, duty);
  }
}
void leftstop(){
  if(motorson){
    analogWrite(6,0);
    analogWrite(7,0);
  }
}
void rightforward(float duty)
{
  if(duty < 0){
    rightback(-duty);
  }
  else if(motorson){
    analogWrite(2,0);
    analogWrite(5, duty);
  }
}
void rightstop()
{
  if(motorson){
    analogWrite(2,0);
    analogWrite(5,0);
  }
  
}
void rightback(float duty)
{
  if(motorson){
    analogWrite(5,0);
    analogWrite(2, duty);
  }
  
  
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
void GreenLEDson(unsigned char mask){
  PORTL |= ~(mask & 0b11110111);
  PORTG &= ~(mask & 0b00001000);
}
void GreenLEDon(int Ledno){
  if(Ledno == 3){
    PORTL |= 0b11110111;
    PORTG &= 0b11110111;
  }
  else{
    PORTL = ~(1<<Ledno);
    PORTG |= 0b00001000;
  }
}
void GreenLEDoff(){
  PORTG |= 0b00001000;
  PORTL |= 0b11110111;
}

int* readProxSensor(unsigned char &med, unsigned char &close)
{
  med = close = 0;
  unsigned char mask = 1;
  int values[8];
  proxLEDon();
  delay(3);
  for( int i = A0; i<A8; i++)
  {
    int reading =analogRead(i);
    values[i]=reading;
    if(reading < (usewallthresh ? proxwallthresh[i-A0] : proxthreshholdmed[i-A0]))
    {
      med |= mask;
    }
    if(reading < (usewallthresh ? proxwallthreshclose[i-A0] : proxthreshholdclose[i-A0]))
    {
      close |= mask;
    }
   mask <<= 1;
  }
  proxLEDoff();
  return values;
}
void proxLEDon()
{
  PORTA |= 0b11111111;
}

void proxLEDoff()
{
  PORTA &= 0b00000000;
}
