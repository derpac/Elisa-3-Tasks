int proxthreshhold[8] = {700,700,650,600,650,750,700,640};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DDRA = 0b11111111;
}

void loop() {
  // put your main code here, to run repeatedly:
  proxLEDon();
  delay(3);
  unsigned char proximity = readProxSensor();
  proxLEDoff();
  delay(3);
  Serial.print(proximity);
  Serial.println("");
  delay(500);
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
