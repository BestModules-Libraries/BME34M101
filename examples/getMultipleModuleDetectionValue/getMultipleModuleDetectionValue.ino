/*************************************************
File:             getMultipleModuleDetectionValueWithUart.ino
Description:      Multiple modules are cascaded, and then connected to the development board. 
                  The development board obtains the soil moisture detection value and temperature value of the module, 
                  and displays them on the serial monitor
Note:               
**************************************************/
#include <BME34M101.h>

BME34M101 mySoilMoistureSensor(5,4);//Please comment out this line of code if you don't use Serial
//BME34M101 mySoilMoistureSensor(&Serial1);//Please comment out this line of code if you  use Serial1 on BMduino
//BME34M101 mySoilMoistureSensor(&Serial2);//Please comment out this line of code if you  use Serial2 on BMduino

uint8_t moduleNumble;
uint8_t moistureValBuff[5] = {0};
uint8_t temperatureValBuff[5] = {0};

void setup() {
  Serial.begin(115200);
  
  mySoilMoistureSensor.begin();
  Serial.println("Check whether the module is connected,waiting...");
  while(mySoilMoistureSensor.isConnected() == false)
  {
    delay(1000);
  }
  Serial.println("Module is connecting.");

  moduleNumble = mySoilMoistureSensor.getNumber();
  if(moduleNumble==0)
  {
    Serial.println("Get module numble error.");
  }
  else
  {
    Serial.print("Module numble is: ");
    Serial.println(moduleNumble);
  }
}

void loop() 
{
  uint8_t i;
  uint8_t id;

  for(id = 1; id <= moduleNumble; id++)
  {
    Serial.print("Get id ");
    Serial.print(id);
    Serial.print(" module soil moisture value is: ");
    Serial.println(mySoilMoistureSensor.getMoisture(id));

    delay(1000);
 
    Serial.print("Get id ");
    Serial.print(id);
    Serial.print(" module temperature value is: ");
    Serial.println( mySoilMoistureSensor.getTemperature(id));
    delay(1000);
  }

  mySoilMoistureSensor.getAllMoisture(moistureValBuff);
  Serial.print("Get all module soil moisture value is: ");
  for(i = 0; i <= moduleNumble; i++)
  {
    Serial.print(moistureValBuff[i]);
    Serial.print(" ");
  }
  Serial.println();

  delay(1000);
  
  mySoilMoistureSensor.getAllTemperature(temperatureValBuff);
  Serial.print("Get all module soil temperature value is: ");
  for(i = 0; i <= moduleNumble; i++)
  {
    Serial.print(temperatureValBuff[i]);
    Serial.print(" ");
  }
  Serial.println();

  delay(1000);
}
