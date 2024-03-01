
unsigned char LEDno = 4;

void setup() {
  // put your setup code here, to run once:
  DDRL |= B11110111;
  DDRC &= B11110000;
  DDRG |= B00001000;
}
void loop() {
  // put your main code here, to run repeatedly:
if (SelectorPosition() == 1){
  PORTL &= B00001000;
  PORTG &= B11110111;
}
else if (SelectorPosition() == 2){
  greenLEDon(4);
  delay(1000);
  greenLEDoff(4);
  delay(1000);
}
else{
  PORTL |= B00001000;
  PORTG |= B11110111;
}}
  
void greenLEDon(unsigned char LEDno)
{
if (LEDno ==4){
    PORTL |= (1<<LEDno);
}
else {
  PORTL &= (1<<LEDno);
}}
void greenLEDoff(unsigned char LEDno)
{
  if (LEDno ==4){
    PORTL &= (1<<LEDno);
}
else {
  PORTL |= (1<<LEDno);
}}
unsigned char SelectorPosition()
{
  return PINC & 0x0F;
}
