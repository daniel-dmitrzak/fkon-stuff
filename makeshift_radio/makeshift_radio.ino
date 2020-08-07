#include <TM1637.h>

#define DAC_MASK 0x03
#define TM_CLK 19
#define TM_DIO 18
#define DOT   512
#define DASH  1024

unsigned int correctValues[] = { 0, 0, 0, 0 };
unsigned int currentValues[] = { 0, 0, 0, 0 };
signed int callibValues[3][9] = {
                                    {0, 114, 243, 384, 534, 688, 829, 966, 1023},
                                    {0, 106, 231, 366, 524, 684, 827, 964, 1023},
                                    {0,  85, 217, 351, 497, 664, 807, 943, 1023}};
char serialChar;

TM1637 tm(TM_CLK, TM_DIO);

void setup() {
  // put your setup code here, to run once:
  DDRB |= DAC_MASK;
  tm.init();
  tm.set(7);
  Serial.begin(9600);
  createNewSolution();
  pinMode(13, OUTPUT);
  pinMode(9,  OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //writeDac(random(0, DAC_MASK)); 
  analogWrite(9, random(0,255));
  delay(10); 
  
//  currentValues[0] = map(analogRead(0), 0, 1023, 1, 9);// / 100; 
//  currentValues[1] = map(analogRead(1), 0, 1023, 1, 9);// / 100;  
//  currentValues[2] = map(analogRead(2), 0, 1023, 1, 9);// / 100; 
//  currentValues[3] = map(analogRead(3), 0, 1023, 1, 9);// / 100; 

  for(int i = 0; i < 3; i ++) currentValues[i] = findClosest(i, analogRead(i));

  Serial.print(currentValues[0]);Serial.print(" ");Serial.print(currentValues[1]);Serial.print(" ");Serial.print(currentValues[2]);Serial.print(" ");Serial.print(currentValues[3]);Serial.println();
  //Serial.print(correctValues[0]);Serial.print(" ");Serial.print(correctValues[1]);Serial.print(" ");Serial.print(correctValues[2]);Serial.print(" ");Serial.print(correctValues[3]);Serial.println();
//  Serial.print(analogRead(0));Serial.print(" ");
//  Serial.print(analogRead(1));Serial.print(" ");
//  Serial.print(analogRead(2));Serial.print(" ");
//  Serial.println();
  //if(compareArrays(currentValues, correctValues, sizeof(currentValues), 50))
  if(correctValues[0] == currentValues[0] && correctValues[1] == currentValues[1] && correctValues[2] == currentValues[2])// && correctValues[3] == currentValues[3])
  {
    // Play morse here 
    Serial.println("SOLVED!");
    dot();dot();dot();dash();dash();dash();dot();dot();dot();
    delay(1000);
    // Generate new values for correct answer
    createNewSolution();
  }

  if(Serial.available())
  {
    serialChar = Serial.read();
    if(serialChar == 'g') createNewSolution();
  }
}

void writeDac(char value)
{
  PORTB = (PORTB & ~DAC_MASK) | (value & DAC_MASK);
}

bool compareArrays(unsigned int *array1, unsigned int *array2, unsigned int arraySize, unsigned int marginOfError)
{
  for(int i = 0; i < arraySize; i++) if(array1[i] != array2[i]) return false;
    //if(!((array1[i] >= array2[i] - marginOfError) && (array1[i] <= array2[i] + marginOfError))) return false;
  return true;
}

void encodeAndDisplay(unsigned int *arrayToEncode)
{
  for(int i = 0; i < 4; i++)
    tm.display(i, arrayToEncode[i]);
}

void createNewSolution()
{
  unsigned int  displayedValue[4]; 
  unsigned char textBuffer[16]; 
  unsigned long product;

  generate:
  displayedValue[0] = random(0, 15);
  displayedValue[1] = random(0, 15);
  displayedValue[2] = random(0, 15);
  displayedValue[3] = random(0, 15);

  product = displayedValue[0]*displayedValue[1]*displayedValue[2]*displayedValue[3];
  //if(product < 1000 || product > 9999) goto generate; 
  if(product < 100 || product > 999) goto generate; 
    
  //correctValues[0] = (product / 1000);
  correctValues[0] = (product % 1000) / 100;
  correctValues[1] = (product % 100)  / 10;
  correctValues[2] = (product % 10);
  if(correctValues[0] == 0 || correctValues[1] == 0 || correctValues[2] == 0) goto generate;// || correctValues[3] == 0) goto generate; 

  encodeAndDisplay(displayedValue);

  Serial.print("Product:");Serial.print(product);Serial.print(" Correct value:");Serial.print(correctValues[0]);Serial.print(" ");Serial.print(correctValues[1]);Serial.print(" ");Serial.print(correctValues[2]);Serial.print(" ");Serial.print(correctValues[3]);Serial.println();
}

void dot()
{
  analogWrite(9, 128);
  delay(250);
  analogWrite(9, 0); 
  delay(250);
}

void dash()
{
  analogWrite(9, 128);
  delay(500);
  analogWrite(9, 0); 
  delay(500);
}

short findClosest(short sensor, int value)
{
  short index = 0;
  signed int distance = 2048;
  for(int i = 0; i < 9; i++)
  {
    if(abs(callibValues[sensor][i] - value) <= distance) 
    {
      distance = abs(callibValues[sensor][i] - value);
      index = i;
    }
  }
  return index + 1; 
}

