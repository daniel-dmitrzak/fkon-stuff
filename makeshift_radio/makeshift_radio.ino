#include <TM1637.h>

#define DAC_MASK 0x03
#define TM_CLK 2;
#define TM_DIO 3; 
#define DOT   1024;
#define DASH  2048;

unsigned int correctValues[] = { 0, 0, 0, 0 };
unsigned int currentValues[] = { 0, 0, 0, 0 };

TM1637 tm(TM_CLK, TM_DIO);

void setup() {
  // put your setup code here, to run once:
  DDRB |= DAC_MASK;
  tm.init();
  tm.set(7);
  
  createNewSolution()
}

void loop() {
  // put your main code here, to run repeatedly:
  writeDac(random(0, DAC_MASK)); 
  delay(10); 
  
  currentValues[0] = analogRead(0); 
  currentValues[1] = analogRead(1); 
  currentValues[2] = analogRead(2); 
  currentValues[3] = analogRead(3); 

  if(!compareArrays(currentValues, correctValues, sizeof(currentValues), 10))
  {
    // Play morse here 
    dot();dot();dot();dash();dash();dash();dot();dot();dot();
    delay(1000);
    // Generate new values for correct answer
    createNewSolution()
  }
}

void writeDac(char value)
{
  PORTB = (PORTB & ~DAC_MASK) | (value & DAC_MASK);
}

bool compareArrays(unsigned int *array1, unsigned int *array2, unsigned int arraySize, unsigned int marginOfError)
{
  for(int i = 0; i < arraySize; i++)
    if(!((array1[i] >= array2[i] - marginOfError) && (array1[i] <= array2[i] + marginOfError))) return false;
  return true;
}

void encodeAndDisplay(unsigned int *arrayToEncode)
{
  for(int i = 0; i < 4; i++)
    tm.display(i, arrayToEncode[i]/64);
}

void createNewSolution()
{
  correctValues[0] = random(0, 1023);
  correctValues[1] = random(0, 1023);
  correctValues[2] = random(0, 1023);
  correctValues[3] = random(0, 1023);

  encodeAndDisplay(correctValues);
}

void dot()
{
  for(int i = 0; i < DOT; i++)
  {
    writeDac(DAC_MASK);
    delay(1);
    writeDac(0);
    delay(1);
  }
}

void dash()
{
  for(int i = 0; i < DASH; i++)
  {
    writeDac(DAC_MASK);
    delay(1);
    writeDac(0);
    delay(1);
  }
}
