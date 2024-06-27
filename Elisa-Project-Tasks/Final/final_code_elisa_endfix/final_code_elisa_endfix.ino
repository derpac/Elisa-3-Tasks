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

enum groundname{
  g_leftback =   0b0001,
  g_leftfront =  0b0010,
  g_rightfront = 0b0100,
  g_rightback =  0b1000
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

void setup() {
  // put your setup code here, to run once:
  DDRC &= 0b11110000;
  DDRA = 0b11111111;
  DDRJ |= 0b00001111;
  PORTJ |= 0b00001111;
  DDRL |= 0b11110111;
  DDRG |= 0b00001000;

}
int curselectorpos = -1;

void loop() {
  // put your main code here, to run repeatedly:

if(SelectorPos() != curselectorpos){
  curselectorpos = SelectorPos();
  GreenLEDon(SelectorPos() & 0b00000111);
  switch(curselectorpos){
    case 1:
    t1_setup();
    break;
    case 2:
    t2_setup();
    break;
    case 3:
    t3_setup();
    break;
  }
}
switch(curselectorpos){
  case 1:
  t1_loop();
  break;

  case 2:
  t2_loop();
  break;

  case 3:
  t3_loop();
  break;
}
}

unsigned char SelectorPos(){
  return PINC & 0b00001111;
}
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------

enum actions{
  forward,                          // Enum some states
  stop,
  turnleft,
  turnright,
  turnrightmore,
  turnleftmore,
  reverse,
  leftturn,
  rightturn,
  corner,
  reversethenleft,
  reversethenright,
  reversethencorner
};
void t1_setup() {
               
}

int t1_sta = 1;
int linecolour = 970;               // setting the black line threshhold
int t1_Kr = 47;
int t1_Kl = 28;
int t1_SKr = 55;
int t1_SKl = 34;
int t1_TKr = 37;
int t1_TKl = 22;
void t1_loop() {
  t1_dostate();                       // checks the conditions with dostate
  t1_advancestate();                  // executes the advancement of states with cosideration to the conditions

}
void t1_advancestate(){
  switch(t1_sta)                  // The switch case for states
{
  case forward:
    leftforward(t1_Kl);
    rightforward(t1_Kr);       //move forward
    break;
  case turnleft:
    rightforward(t1_Kr);
    leftforward(-t1_Kl);
    break;
  case turnright:
    leftforward(t1_Kl);
    rightforward(-t1_Kr);
    break;
  case turnrightmore:
    leftforward(t1_Kl);
    rightforward(-t1_Kr);
    break;
  case turnleftmore:
    rightforward(t1_Kr);
    leftforward(-t1_SKl);
    break;
  case stop:
    leftstop();
    rightstop();      //stop
    break;

  
}
}
void t1_dostate(){
  unsigned char ground = readGroundSensor();             // first assigns ground
  if(ground == g_leftfront){
    t1_sta = turnleft;
  }
  else if(ground == g_rightfront){                        // assigns state based on ground conditions
    t1_sta = turnright;
  }
  else if(ground == (g_rightback)){
    t1_sta = turnrightmore;
  }
  else if(ground == (g_leftback)){
    t1_sta = turnleftmore;
  }
  else if(ground == (g_rightfront | g_leftfront | g_leftback | g_rightback)){
    t1_sta = stop; 
  }
  else{
    t1_sta = forward;
  }
}



//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------



enum actions t2_sta;
int anglehigh = 100, anglelow = 80;
//int t2_proxthreshhold[8] = {700,700,650,600,650,750,700,640};
//int t2_proxthreshhold[8] = {840,840,840,840,840,840,840,840};
int t2_proxthreshhold[8] = {700,700,700,700,700,700,700,700};
int t2_reversedelay = 180;
int t2_Kl = 29;
int t2_Kr = 45;
float t2_leftspeed = 1;
float t2_rightspeed = 1;

void t2_setup() {
  // put your setup code here, to run once:;
t2_sta = forward;
}

void t2_loop()
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
    anglehigh = 100;
    anglelow = 50;
  }
  else if(ground == (g_leftfront | g_leftback))    
  {
    t2_sta = blockedbehind ? turnright : reversethenright ;
    anglehigh = 100;
    anglelow = 30;
  }
  else if(ground == (g_leftfront | g_rightfront))
  {
    t2_sta = blockedbehind ? corner : reversethencorner;
    anglehigh = 100;
    anglelow = 45;
  }
  else if(ground == (g_leftfront | g_leftback | g_rightfront))    
  {
    t2_sta = blockedbehind ? turnright : reversethenright ;
    anglehigh = 100;
    anglelow = 50;
  }
  else if(ground == (g_leftfront | g_rightback | g_rightfront))    
  {
    t2_sta = blockedbehind ? turnleft : reversethenleft ;
    anglehigh = 100;
    anglelow = 50;
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




// ----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------



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






enum actions action = forward;
enum junctions junction = start;
float speed_l = 26;
float speed_r = 41;

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
float lowrightsteer_l = 25;
float lowrightsteer_r = -30;

float wf_leftsteer_l = -22;
float wf_leftsteer_r = 33;
float wf_rightsteer_l = 20;
float wf_rightsteer_r = -30;

int proxthreshholdmed[8] = {600,600,860,600,600,600,860,600};
int proxthreshholdclose[8] = {300,300,300,300,300,300,300,300};
int proxwallthreshclose[8] = {500,500,500,500,500,500,500,500};
int proxwallthresh[8] = {700,700,700,700,700,700,700,700};
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
int wallfollowcount = 250;
int wallfollowcountdown = 0;
int wallturndelay = 5;
int wallmovedelay = 5;
bool motorson = true;


void t3_setup() {
  // put your setup code here, to run once:
  
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

  if(junction != end && (ground == (g_leftback | g_rightback | g_rightfront | g_leftfront))){
    junction = start;
    action = forward;
    linefollow = false;
    wallfollow = false;
    usewallthresh = false;
    leftsteer_l = highleftsteer_l;
    leftsteer_r = highleftsteer_r;
    rightsteer_l = highrightsteer_l;
    rightsteer_r = highrightsteer_r;
    //Serial.println("start");
  }
  //if(junction != end && (ground == (g_leftback | g_rightback | g_rightfront | g_leftfront))){
  //  junction = junc_D;
 //   action = forward;
  //  wallfollowcountdown = wallfollowcount;
 //   linefollow = true;
 //   wallfollow = false;
  //  usewallthresh = false;
 //   leftsteer_l = lowleftsteer_l;
  //  leftsteer_r = lowleftsteer_r;
 //   rightsteer_l = lowrightsteer_l;
 //   rightsteer_r = lowrightsteer_r;
    //Serial.println("start");
//}
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
     // Serial.print(actionnames[action]);
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
	  }
      if(wallfollow){
        if(proximity & p_front){
          //startturnright();
        }
        if(ground == (g_leftback | g_rightback | g_rightfront | g_leftfront)){
          changestate(end);
          action = stop;
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
	  }
		if(wallfollow){
		  if(proximity & p_front){
			//startturnleft();
		  }
		  if(ground == (g_leftback | g_rightback | g_rightfront | g_leftfront)){
			changestate(end);
			action = stop;
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

      int leftvalue = min(proxvalues[6], proxvalues[7]) + 120;
      int rightvalue = min(proxvalues[1], proxvalues[2]);

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
