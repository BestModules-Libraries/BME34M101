/*************************************************
File:               BME34M101.cpp
Author:             BESTMODULES
Description:        UART communication with the BMN31M221 and obtain the corresponding value
History：            
  V1.00.1   -- initial version；2023-03-31；Arduino IDE :  ≥v1.8.13
**************************************************/
#include "BME34M101.h"

/*************************************************
Description:        Constructed function
Parameters:         *theSerial：HardwareSerial
                    Arduino Uno: &Serial
                    BMduino: &Serial、&Serial1、&Serial2、&Serial3、&Serial4
Return:             None
Others:             
*************************************************/
BME34M101::BME34M101(HardwareSerial *theSerial)
{
  _softSerial = NULL;
  _hardSerial = theSerial;
}

/*************************************************
Description:        Constructed function
Parameters:         rxPin：RX PIN of SoftwareSerial
                    txPin：TX PIN of SoftwareSerial
Return:             None
Others:             
*************************************************/
BME34M101::BME34M101(uint8_t rxPin, uint8_t txPin)
{
  _hardSerial = NULL;
  _rxPin = rxPin;
  _txPin = txPin;
  _softSerial = new SoftwareSerial(_rxPin,_txPin); 
}
/*************************************************
Description:        Initialize
Parameters: 
Return:             None
Others:             
*************************************************/
void BME34M101::begin()
{
  if (_softSerial != NULL)
  {
    _softSerial->begin(9600);
  }
  else
  {
    _hardSerial->begin(9600);
  }
}

/*************************************************
Description:        Is the modules ready
Parameters: 
Return:             Connected - 0x01; Disconnect - 0x00;
Others:             
*************************************************/
bool BME34M101::isConnected()
{
  uint8_t retVal = 1;
  dataTxBuf[0] = BROADCAST_ID;
  dataTxBuf[0] = (dataTxBuf[0] << 4) + 2;
  dataTxBuf[1] = CMD_IS_MODULE_READY;
  dataTxBuf[2] = 0;
  for (int i = 0; i < 2; i++)
  {
    dataTxBuf[2] += dataTxBuf[i];
  }
  
  writeBytes(dataTxBuf, 3);
  delay(40); //Response delay time
  if (readBytes(dataRxBuf,3,10) == CHECK_OK)
  {
     retVal = dataRxBuf[1];    
  }
  if(retVal == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}
/*************************************************
Description:        Get the number of modules
Parameters:                 
Return:             moduleNumberVal: Variables for storing number of cascaded modules 
Others:             
*************************************************/
uint8_t BME34M101::getNumber()
{
  uint8_t moduleNumberVal=0;
  dataTxBuf[0] = BROADCAST_ID;
  dataTxBuf[0] = (dataTxBuf[0] << 4) + 2;
  dataTxBuf[1] = CMD_GET_MODULE_NUMBER;
  dataTxBuf[2] = 0;
  for (int i = 0; i < 2; i++)
  {
    dataTxBuf[2] += dataTxBuf[i];
  }
  
  writeBytes(dataTxBuf, 3);
  delay(40); //Response delay time
  if (readBytes(dataRxBuf,4,10) == CHECK_OK)
  {
    moduleNumberVal = dataRxBuf[2];
  }
  return moduleNumberVal;
}

/*************************************************
Description:        Get a module soil moisture detection values
Parameters:         id：module id                   
Return:             moisVal: Variables for storing soil moisture detection values 
Others:             
*************************************************/
uint8_t BME34M101::getMoisture(uint8_t id)
{
  uint8_t moisVal=0;
  dataTxBuf[0] = id;
  dataTxBuf[0] = (dataTxBuf[0] << 4) + 2;
  dataTxBuf[1] = CMD_READ_MOISTURE;
  dataTxBuf[2] = 0;
  for (int i = 0; i < 2; i++)
  {
    dataTxBuf[2] += dataTxBuf[i];
  }
  writeBytes(dataTxBuf, 3);
  delay(40); //Response delay time
  if (readBytes(dataRxBuf,4,10) == CHECK_OK)
  {
    moisVal = dataRxBuf[2];
  }
  return moisVal;
}

/*************************************************
Description:        Get a module temperature detection value
Parameters:         id：module id                     
Return:             tempVal: Variables for storing temperature detection values 
Others:             
*************************************************/
uint8_t BME34M101::getTemperature(uint8_t id)
{
  uint8_t tempVal=0;
  dataTxBuf[0] = id;
  dataTxBuf[0] = (dataTxBuf[0] << 4) + 2;
  dataTxBuf[1] = CMD_READ_TEMPERATURE;
  dataTxBuf[2] = 0;
  for (int i = 0; i < 2; i++)
  {
    dataTxBuf[2] += dataTxBuf[i];
  }
  
  writeBytes(dataTxBuf, 3);
  delay(40); //Response delay time
  if (readBytes(dataRxBuf,4,10) == CHECK_OK)
  {
    tempVal = dataRxBuf[2];
  }
  return tempVal;
}

/*************************************************
Description:        Get all module soil moisture detection values
Parameters:         moisBuff：The array that stores all soil moisture detection values 
Return:             
Others:             
*************************************************/
void BME34M101::getAllMoisture(uint8_t moisBuff[])
{
  uint8_t dataIndex;
  uint8_t retVal = 0;
  dataTxBuf[0] = BROADCAST_ID;
  dataTxBuf[0] = (dataTxBuf[0] << 4) + 2;
  dataTxBuf[1] = CMD_READ_MOISTURE;
  dataTxBuf[2] = 0;
  for (int i = 0; i < 2; i++)
  {
    dataTxBuf[2] += dataTxBuf[i];
  }
  
  writeBytes(dataTxBuf, 3);
  delay(40); //Response delay time
  if (readBytes(dataRxBuf,8,10) == CHECK_OK)
  {
    retVal = dataRxBuf[1];
  }
  if(retVal == 0)
  {
    for (dataIndex = 0; dataIndex <= dataRxBuf[2]; dataIndex++)
    {
      moisBuff[dataIndex] = dataRxBuf[2 + dataIndex];
    }
  }
}

/*************************************************
Description:        Get all module temperature detection values
Parameters:         tempBuff：The array that stores all temperature detection values 
Return:             
Others:                
*************************************************/
void BME34M101::getAllTemperature(uint8_t tempBuff[])
{
  uint8_t dataIndex;
  uint8_t retVal = 0;

  dataTxBuf[0] = BROADCAST_ID;
  dataTxBuf[0] = (dataTxBuf[0] << 4) + 2;
  dataTxBuf[1] = CMD_READ_TEMPERATURE;
  dataTxBuf[2] = 0;
  for (int i = 0; i < 2; i++)
  {
    dataTxBuf[2] += dataTxBuf[i];
  }
  
  writeBytes(dataTxBuf, 3);
  if (readBytes(dataRxBuf,8,10) == CHECK_OK)
  {
    retVal = dataRxBuf[1];
  }
  if(retVal == 0)
  {
    for (dataIndex = 0; dataIndex <= dataRxBuf[2]; dataIndex++)
    {
      tempBuff[dataIndex] = dataRxBuf[2 + dataIndex];
    }
  }
}

/*************************************************
Description:        Sending Command frame and receiving response data frame
Parameters:         dataSend：Send data frame
                    sendDataLen：Transmission data frame length
                    dataRec：Array for storing response frame
Return:             Communication status: COMMUNICATION_OK(0x00) or COMMUNICATION_ERROR(0xFF)
Others:            
*************************************************/
void BME34M101::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
  //Clear the UART receiver buff and send data frames
  if (_softSerial != NULL)
  {
    while (_softSerial->available())
    {
      _softSerial->read();
    }
    _softSerial->write(wbuf, wlen);
  }
  else
  {
    while (_hardSerial->available() > 0)
    {
     _hardSerial->read();
    }
    _hardSerial->write(wbuf, wlen);
  }
}

uint8_t BME34M101::readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeout)
{
  uint16_t delayCnt = 0;
  uint8_t i = 0, checkSum = 0;
/* Select SoftwareSerial Interface */
  if (_softSerial != NULL)
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_softSerial->available() == 0)
      {
        if (delayCnt > timeout)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _softSerial->read();
      
    }
  }
/* Select HardwareSerial Interface */
  else
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_hardSerial->available() == 0)
      {
        if (delayCnt > timeout)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _hardSerial->read();
    }
  }

  /* check Sum */
  for (i = 0; i < (rlen - 1); i++)
  {
    checkSum += rbuf[i];
  }
  //checkSum = ~checkSum + 1;
  if (checkSum == rbuf[rlen - 1])
  {
    return CHECK_OK; // Check correct
  }
  else
  {
    return CHECK_ERROR; // Check error
  }
}
