void setup() {
  // put your setup code here, to run once:
DDRJ |= 0b00001111;
PORTJ |= 0b00001111;
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int angle = 0;
int turn = 0;
int sta = 1;
int colour = 970;
groundledon(1);
delay(3);
int ground1 = analogRead(A9);
//groundledoff(1);
//Serial.print(ground1);
//delay(3);
//Serial.print(" ");
groundledon(2);
delay(3);
int ground2 = analogRead(A10);
//groundledoff(2);
//Serial.println(ground2);
groundledon(0);
delay(3);
int ground0 = analogRead(A8);
//
groundledon(3);
delay(3);
int ground3 = analogRead(A11);
//
if(ground0 > colour && ground1 > colour && ground2 <= colour && ground3 <= colour){
  sta = 2;
}
else if(ground2 > colour && ground3 > colour && ground1 <= colour && ground0 <= colour){
  sta = 3;
}
else if(ground1 > colour && ground2 > colour){
  sta = 4;
}
else{
  sta = 1;
}

switch(sta)
{
  case 1:
    leftforward(25);
    rightforward(35);//move forward
    break;
  case 2:
    rightstop();
    leftstop();
    //delay(500);
    rightback(40);
    leftback(30);
    delay(200);
    angle = random(80,100);
    turn = map(angle,0,360,0,1050);
    leftforward(25);
    rightback(34);
    delay(turn);
    rightstop();
    leftstop();
          //move left
    break;
  case 3:
    rightstop();
    leftstop();
    //delay(500);
    rightback(40);
    leftback(30);
    delay(200);
    angle = random(80,100);
    turn = map(angle,0,360,0,1050);
    leftback(25);
    rightforward(34);
    delay(turn);
    rightstop();
    leftstop();
       //move right
    break;
  case 4:
       // turn again i guess???
    rightstop();
    leftstop();
    //delay(500);
    rightback(40);
    leftback(30);
    delay(200);
    angle = random(90,100);
    turn = map(angle,0,360,0,1050);
    leftback(25);
    rightforward(34);
    delay(turn);
    rightstop();
    leftstop();
    break;

  
}


}

void groundledon(unsigned char ledno){
  PORTJ &= ~(1<<ledno);
}
void groundledoff(unsigned char ledno){
  PORTJ |= (1<<ledno);
  
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
