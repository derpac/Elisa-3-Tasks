

void setup() {
  // put your setup code here, to run once:
DDRJ |= 0b00000110;
PORTJ |= 0b00000110;
Serial.begin(9600);
int sta = 1;

}

void loop() {
  // put your main code here, to run repeatedly:
int sta = 1;
int colour = 970;
groundledon(1);
delay(3);
int ground1 = analogRead(A9);
//groundledoff(1);
Serial.print(ground1);
//delay(3);
Serial.print(" ");
groundledon(2);
delay(3);
int ground2 = analogRead(A10);
//groundledoff(2);
Serial.println(ground2);

if(ground1 > colour && ground2 <= colour){
  sta = 2;
}
else if(ground1 <= colour && ground2 >colour){
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
    leftforward(30);
    rightforward(51);//move forward
    break;
  case 2:
    rightforward(34);
    leftback(15);
    //delay(2);
          //move left
    break;
  case 3:
    leftforward(20);
    rightback(25);
    //delay(2);
       //move right
    break;
  case 4:
    leftstop();
    rightstop();      //stop
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
