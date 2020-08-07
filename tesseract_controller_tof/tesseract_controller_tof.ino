#include "Adafruit_VL53L0X.h"
#define LOX2_XSHUT 4
#define LOX3_XSHUT 5

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
//Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
  
void setup() {
  Serial.begin(115200);
  pinMode(LOX2_XSHUT, OUTPUT);
  pinMode(LOX3_XSHUT, OUTPUT);
  pinMode(13,OUTPUT);

  // Put LOX2 and LOX3 in shutdown
  digitalWrite(LOX2_XSHUT, LOW);
  digitalWrite(LOX3_XSHUT, LOW);
  
  // wait until serial port opens for native USB device
  while (! Serial) {
    delay(1);
  }

  //Serial.println("Starting sensors");

  // Initialize the sensors
  //Serial.println("Starting sensor 1");
  if (!lox1.begin(0x30)) {
    digitalWrite(13, HIGH);
    //Serial.println("Failed to initialize sensor 1");
    while(1);
  }

  //Serial.println("Starting sensor 2");
  digitalWrite(LOX2_XSHUT, HIGH);
  delay(10);
  if (!lox2.begin(0x31)) {
    digitalWrite(13, HIGH);
    //Serial.println("Failed to initialize sensor 2");
    while(1);
  }

  
//  digitalWrite(LOX3_XSHUT, HIGH);
//  if (!lox3.begin(0x32)) {
//    digitalWrite(13, HIGH);
//    while(1);
//  }

  //Serial.println("Sensors initialized");
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure1;
  VL53L0X_RangingMeasurementData_t measure2;
//  VL53L0X_RangingMeasurementData_t measure3;
     
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
//  lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!

  unsigned long measureMm1 = measure1.RangeMilliMeter; 
  unsigned long measureMm2 = measure2.RangeMilliMeter; 
//  unsigned long measureMm3 = measure3.RangeMilliMeter; 
  //unsigned long delimiter  = 0xFFFFFFFF;

  //unsigned char measureChar1 = map(constrain(measureMm1, 40, 400), 40, 400, 1, 255);
  //unsigned char measureChar2 = map(constrain(measureMm2, 40, 400), 40, 400, 1, 255);

  Serial.print(constrain(measureMm1, 40, 361));
  Serial.print(" ");
  Serial.print(constrain(measureMm2, 40, 361));
//  Serial.print(" ");
//  Serial.print(constrain(measureMm3, 40, 360));
  Serial.println();

  //Serial.write((char*)&measureMm1, 4);
  //Serial.write((char*)&measureMm2, 4); 
  //Serial.write((char*)&delimiter, 4); 

//  Serial.write(measureChar1);
//  Serial.write(measureChar2);
//  Serial.write(0);
//Serial.println();
//
//  Serial.print("Measuring...");
//  Serial.print(" Range 1:"); 
//  Serial.print(measureMm1);
//  Serial.print(" Char 1:");
//  Serial.print(measureChar1, DEC);
//  Serial.print(" Range 2:"); 
//  Serial.print(measureMm2);
//  Serial.println();
    
  //delay(100);
}
