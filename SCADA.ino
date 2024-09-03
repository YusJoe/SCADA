byte sensorInterrupt1 = 0;  
byte sensorPin1       = 2;
byte sensorInterrupt2 = 1;
byte sensorPin2       = 3;

volatile unsigned int count1;
volatile unsigned int count2;

float calibrationFactor = 4.5; 

float copyCount1;
float copyCount2;
unsigned int flowMilliLitres1;
unsigned long totalMilliLitres1;
unsigned int flowMilliLitres2;
unsigned long totalMilliLitres2;

unsigned long oldTime;

void setup()
{
  Serial.begin(9600);
  pinMode(sensorPin1, INPUT);
  digitalWrite(sensorPin1, HIGH);
  pinMode(sensorPin2, INPUT);
  digitalWrite(sensorPin2, HIGH);

  copyCount1         = 0.0;
  copyCount2         = 0.0;
  flowMilliLitres1   = 0;
  flowMilliLitres2   = 0;
  totalMilliLitres1  = 0;
  totalMilliLitres2  = 0;
  oldTime            = 0;

  attachInterrupt(0, count1_ISR, FALLING);
  attachInterrupt(1, count2_ISR, FALLING);
}

void loop()
{
   if((millis() - oldTime) > 1000)    
  { 
    noInterrupts();
    copyCount1 = ((1000.0 / (millis() - oldTime)) * count1) / calibrationFactor;
    copyCount2 = ((1000.0 / (millis() - oldTime)) * count2) / calibrationFactor;
    interrupts();
    flowMilliLitres1 = (copyCount1 / 60) * 1000;
    totalMilliLitres1 += flowMilliLitres1;
    flowMilliLitres2 = (copyCount2 / 60) * 1000;
    totalMilliLitres2 += flowMilliLitres2;
    oldTime = millis();

    unsigned int frac1;
    unsigned int frac2;
    
    Serial.print("Flow rate1: ");
    Serial.print(int(copyCount1)); 
    Serial.print(".");             
    frac1 = (copyCount1 - int(copyCount1)) * 10;
    Serial.print(frac1, DEC);
    Serial.print("L/min");
    Serial.print("  ll  Current Air Flowing1: ");             
    Serial.print(flowMilliLitres1);
    Serial.print("mL/Sec");
    Serial.print("  ll  Output Air Quantity1: ");             
    Serial.print(totalMilliLitres1);
    Serial.println("mL");    

    Serial.print("Flow rate2: ");
    Serial.print(int(copyCount2));  
    Serial.print(".");             
    frac2 = (copyCount2 - int(copyCount2)) * 10;
    Serial.print(frac2, DEC);
    Serial.print("L/min");
    Serial.print("  ll  Current Air Flowing2: ");            
    Serial.print(flowMilliLitres2);
    Serial.print("mL/Sec");
    Serial.print("  ll  Output Air Quantity2: ");          
    Serial.print(totalMilliLitres2);
    Serial.println("mL");
    Serial.println();

    count1 = 0;
    count2 = 0;

    attachInterrupt(0, count1_ISR, FALLING);
    attachInterrupt(1, count2_ISR, FALLING);
  }
}

void count1_ISR()
{
  count1++;
}

void count2_ISR()
{
  count2++;
}
