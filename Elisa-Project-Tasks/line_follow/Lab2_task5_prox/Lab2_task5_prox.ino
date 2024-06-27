void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DDRA = 0b11111111;
}

void loop() {
  // put your main code here, to run repeatedly:
  proxLEDon();
  delay(4);
  readProxSensor();
  //proxLEDoff();
  delay(4);
  
}
int Prox[8];

void readProxSensor()
{
  for( int i = A0; i<A8; i++)
  {
    int reading =analogRead(i);
    int reallight = map(reading,0,1023,0,1000);
    int mm = 11.98*exp(0.004343*reallight);
    //Prox[i-A0] = cm;
    Serial.print(mm);
    Serial.print(" ");
    Serial.print(reallight);
    Serial.print(" ");
    
    
  }
  Serial.println("");
  delay(100);
}
void proxLEDon()
{
  PORTA |= 0b11111111;
}
void proxLEDoff()
{
  //PORTA &= 0b00000000;
}
