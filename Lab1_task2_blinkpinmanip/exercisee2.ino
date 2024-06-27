
unsigned char LEDno = 1;

void setup() {
  // put your setup code here, to run once:
  DDRL |= (1<<LEDno);
}

void loop() {
  // put your main code here, to run repeatedly:
  greenLEDon(LEDno);
  delay(2000);
  greenLEDoff(LEDno);
  delay(2000);
}
void greenLEDon(unsigned char LEDno)
{
    PORTL |= (1<<LEDno);
}
void greenLEDoff(unsigned char LEDno)
{
  PORTL &= ~(1<<LEDno);
}
