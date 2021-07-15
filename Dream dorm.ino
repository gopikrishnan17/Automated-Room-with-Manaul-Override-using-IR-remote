#include<LiquidCrystal.h>
#include<IRremote.h>          //header file for IR
LiquidCrystal lcd(2,3,4,5,6,7);
int tempin=A0;                //input pin of emperature sensor
int bulbin=A2;
int fanout=10;                //pin number for DC motor output
int irpin=9;                  //pin number for IR input
double temp=0;                //to store temperature
int t=0;                      //to check whether IR input for fanspeed is given
int fanspeed=0;               //Analog walue of fanspeed
int tempmin=86;               //min temperature for fan to turn on
int tempmax=122;               //temperature after which the fan runs in highest speed
int fanper=0;
int bulbout=11;
int blb,b=0;
IRrecv irrecv(irpin);
decode_results results;
void setup()
{
  pinMode(fanout,OUTPUT);
  pinMode(tempin,INPUT);
  pinMode(bulbin,INPUT);
  pinMode(bulbout,OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  lcd.begin(16,2);
}
void loop()
{
  temp=tempread();
  blb=analogRead(bulbin);
  if (irrecv.decode(&results))
  {
    Serial.print(results.value,HEX);
    if(results.value==0xFD30CF)
    {
      fanspeed=0;
      Serial.println(" : 0");
      t=t+1;
    }
    else if(results.value==0xFD08F7)
    {
      fanspeed=81;
      Serial.println(" : 1");
      t=t+1;
    }
    else if(results.value==0xFD8877)
    {
      fanspeed=162;
      Serial.println(" : 2");
      t=t+1;
    }
    else if(results.value==0xFD48B7)
    {
      fanspeed=255;
      Serial.println(" : 3");
      t=t+1;
    }
    else if(results.value==0xFD00FF)
    {
      b=b+1;
      if(b%2==1)
        digitalWrite(bulbout,HIGH);
      else
        digitalWrite(bulbout,LOW);
    }
    irrecv.resume();
  }
  if(t==0)
  {
    if(temp<=tempmin)
      fanspeed=0;
    else if(temp>tempmax)
      fanspeed=255;
    else
    {
      fanspeed=map(temp,tempmin,tempmax,50,255);
    }
  }
  analogWrite(fanout,fanspeed);
  Serial.println(analogRead(bulbin));
  if(blb<=500 && b==0)
     digitalWrite(bulbout,HIGH);
  else if(blb>500 && b==0)
     digitalWrite(bulbout,LOW);
  fanper=(fanspeed*100)/255;
  lcd.print("TEMP: ");
  lcd.print(temp);
  lcd.print("F");
  lcd.setCursor(0,1);
  lcd.print("F.Speed: ");
  lcd.print(fanper);
  lcd.print("%");
  delay(200);
  lcd.clear();
}
double tempread()
{
  temp=analogRead(tempin);
  temp=temp*0.4878-49.6325;
  temp=temp*9/5+32;
  return temp;
}