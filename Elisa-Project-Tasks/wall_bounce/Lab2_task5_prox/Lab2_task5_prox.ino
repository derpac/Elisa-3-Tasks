void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DDRA = 0b11111111;
  int threshold = 150;
}

void loop() {
  int threshold = 150;
  int sta = 1;
  // put your main code here, to run repeatedly:
  proxLEDon();
  delay(4);
  readProxSensor();
  delay(2);
  proxLEDoff();
  delay(2);

}
int RL[8];

void readProxSensor() {
  

  for( int i = A0; i<A8; i++)
  {
    int reading =analogRead(i);
    int reallight = map(reading,0,1023,1000,0);
    RL[i] = reallight;
    //int mm = 11.98*exp(0.004343*reallight);
    //Prox[i-A0] = cm;
    Serial.print(" ");
    Serial.print(reallight);
    Serial.print(" ");
     
  }
  Serial.println("");
  delay(100);

  int prox0 = RL[0];
  int prox1 = RL[1];
  int prox2 = RL[2];
  int prox3 = RL[3];
  int prox4 = RL[4];
  int prox5 = RL[5];
  int prox6 = RL[6];
  int prox7 = RL[7];
 
  int threshold = 150;
  int sta = 1;
  if(prox1 > threshold && prox2 > threshold){
    sta = 2;
}
  else if(prox6 > threshold && prox7 > threshold){
    sta = 3;
}
  //else if(ground1 > colour && ground2 > colour){
  //  sta = 4;

  else{
    sta = 1;
}

switch(sta)
{
  case 1:
    leftforward(30);
    rightforward(51);//move forward
    break;
  case 2:
    leftforward(30);
    rightforward(61);//move forward
    //delay(2);
          //move left
    break;
  case 3:
    leftforward(37);
    rightforward(51);//move forward
    //delay(2);
       //move right
    break;
  case 4:
    leftstop();
    rightstop();      //stop
    break;
  
}
}
void proxLEDon()
{
  PORTA |= 0b11111111;
}
void proxLEDoff()
{
  //PORTA &= 0b00000000;
}
void leftforward(float duty){
  analogWrite(7,0);
  analogWrite(6, duty);
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
  analogWrite(2,0);
  analogWrite(5, duty);
}
void rightstop(){
  analogWrite(2,0);
  analogWrite(5,0);
  
}
void rightback(float duty){
  analogWrite(5,0);
  analogWrite(2, duty);
  
}
