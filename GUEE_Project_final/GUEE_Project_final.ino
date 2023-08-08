/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL1Uq7K0KX"
#define BLYNK_TEMPLATE_NAME "IOT ENERGY METER"
#define BLYNK_AUTH_TOKEN "-RKdz7EENOCzuOYIhDiZSRaWwWAkffsp"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//#include "ACS712.h
//ACS712 sensor(ACS712_30A,A0);
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "OPPO K10 5G";
char pass[] = "123456789";
float voltage=230;

float energy=0,units=0;
float cost=100;
float getcur()
{
  float a0=analogRead(A0);
  float i=a0/1023/0.185;
  return i;
  
}

float getcurrent()
{
  unsigned int x=0;

float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;

  for (int x = 0; x < 150; x++){ //Get 150 samples
  AcsValue = analogRead(A0);     //Read current sensor values   
  Samples = Samples + AcsValue;  //Add samples together
  delay (3); // let ADC settle before next sample 3ms
}
AvgAcs=Samples/150.0;//Taking Average of Samples
AcsValueF = (3.3/2 - (AvgAcs * (3.3/2/ 1024.0)) +3*0.185)/(0.0185*2);
return(AcsValueF/1000);
}
float current=getcurrent();
void setup()
{
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.syncVirtual(V4); 
}
BLYNK_WRITE(V4) {
  int storedEnergy = param.asInt(); // Get the value of virtual pin V4 from the server
  energy = storedEnergy; // Set the stored energy to the value retrieved from virtual pin V4
}

float power=0,amount=0;
void loop()
{
  Blynk.run();
 

  //energy=Blynk.virtualRead(V4);
  //timer.run();
  //current=sensor.getCurrentAC();
  current=getcurrent();
  power=current*voltage;
  //energy+=power/1000/3600;
  amount=energy*cost;
  int readvoltage=230;
  //int apppower=readvoltage*current;
  float pf=0.8;
  //int energy = Blynk.virtualRead(V4);
  Blynk.virtualWrite(V5, cost);
  Blynk.virtualWrite(V0, voltage);
  //float v1=-current;
  Blynk.virtualWrite(V1, current*1000);
  Serial.println(current*1000);
  
  Blynk.virtualWrite(V7, (current+5)*1000);
  Blynk.virtualWrite(V8, 1);
  Blynk.virtualWrite(V2, power);
  energy=voltage*current*pf+energy;
  units=energy/3600/1000;
  Blynk.virtualWrite(V4, energy);
  Blynk.virtualWrite(V3, units);
  Blynk.virtualWrite(V6, units*cost);
  delay(1000);
  
  
   
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
