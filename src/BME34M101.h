/*****************************************************************
File:             BME34M101.h
Author:           BESTMODULES
Description:      Define classes and required variables
History：         
V1.0.1-- initial version；2023-03-31；Arduino IDE : ≥v1.8.13
******************************************************************/
#ifndef _BME34M101_
#define _BME34M101_

#include <Arduino.h>
#include <SoftwareSerial.h>

#define BROADCAST_ID              (0x00)

#define CMD_READ_MOISTURE         (0x01)        
#define CMD_READ_TEMPERATURE      (0x02)  
#define CMD_GET_MODULE_NUMBER     (0x11)
#define CMD_IS_MODULE_READY       (0x12)

#define CHECK_OK        0
#define CHECK_ERROR     1
#define TIMEOUT_ERROR   2

class BME34M101
{
  public:
    BME34M101(HardwareSerial *theSerial = &Serial);               //constructed function：choose HW UART
    BME34M101(uint8_t rxPin, uint8_t txPin);                      //constructed function：choose SW UART
    void begin();                                                 //Initialize
    bool isConnected();                                      //
    uint8_t getNumber();            //
    uint8_t getMoisture(uint8_t id = 1);            //
    uint8_t getTemperature(uint8_t id = 1);                      //
    void getAllMoisture(uint8_t moisBuff[]);                    //
    void getAllTemperature(uint8_t tempBuff[]);                 //
    
  private:
    uint8_t _rxPin, _txPin;
    HardwareSerial *_hardSerial = NULL;                               //HW UART
    SoftwareSerial *_softSerial = NULL;                           //SW UART
    
    uint8_t dataTxBuf[8] = {0};                                   //Array of data frames sent
    uint8_t dataRxBuf[10] = {0};                                  //Array of data frames receive 

   void writeBytes(uint8_t wbuf[], uint8_t wLen);
   uint8_t readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeout);
};

#endif
