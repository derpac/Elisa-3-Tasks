enum groundname{                      // Enumeration to assign names to each ground sensor
  g_leftback =   0b0001,
  g_leftfront =  0b0010,
  g_rightfront = 0b0100,
  g_rightback =  0b1000
};
enum actions{
  forward,                          // Enum some states
  stop,
  turnleft,
  turnright,
  turnrightmore,
  turnleftmore
};
void setup() {
DDRJ |= 0b00000110;                 // Setting up the ground sensors
PORTJ |= 0b00000110;                 
}
int t1_sta = 1;
int linecolour = 970;               // setting the black line threshhold
int t1_Kr = 47;
int t1_Kl = 28;
int t1_SKr = 55;
int t1_SKl = 34;
int t1_TKr = 37;
int t1_TKl = 22;
void loop() {
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
unsigned char readGroundSensor()
{
  
  unsigned char ret = 0;
  unsigned char mask = 1;                            // reads the ground sensors and assigns it to a 4bit mask
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
void groundledson()                                               // turns the LEDs on
{
  PORTJ &= 0b11110000;
} 
void groundledsoff()                                             // turns the LEDs off
{
  PORTJ |= 0b00001111;
}

void leftforward(float duty){
  if(duty < 0){
    leftback(-duty);
  }
  else{
  analogWrite(7,0);
  analogWrite(6, duty);
  }
}
void leftback(float duty){
  analogWrite(6,0);
  analogWrite(7, duty);
}
void leftstop(){
  analogWrite(6,0);
  analogWrite(7,0);
}
void rightforward(float duty){
  if(duty < 0){
    rightback(-duty);
  }
  else{
  analogWrite(2,0);
  analogWrite(5, duty);
  }
}
void rightstop(){
  analogWrite(2,0);
  analogWrite(5,0);
  
}
void rightback(float duty){
  analogWrite(5,0);
  analogWrite(2, duty);
  
}
