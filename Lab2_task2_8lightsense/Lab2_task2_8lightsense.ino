void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  readLightSensor();
}
int light[8];
void readLightSensor()
{
  for(int i = A0; i<A8; i++)
  {
    int reading = analogRead(i);
    int reallight = map(reading,0,1023,200,0);
    light[i-A0] = reallight;
    Serial.print(light[i-A0]);
    Serial.print(" ");
  }
 Serial.println("");
}
