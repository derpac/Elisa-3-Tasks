void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DDRA = 11111111;
}

void loop() {
  // put your main code here, to run repeatedly:
  proxLEDon();
  delay(1);
  readProxSensor();
  proxLEDoff();
  delay(1);
  
}
int Prox[8];

void readProxSensor()
{
  for( int i = A0; i<A1; i++)
  {
    int reading =analogRead(i);
    int reallight = map(reading,0,1023,0,1000);
    int cm = 0.0003*exp(0.0108*reallight);
    Prox[i-A0] = cm;
    Serial.print(Prox[i-A0]);
    Serial.print(" ");
    
    
  }
  Serial.println("");
  delay(1000);
}
void proxLEDon()
{
  PORTA |= 11111111;
}
void proxLEDoff()
{
  PORTA &= 00000000;
}
