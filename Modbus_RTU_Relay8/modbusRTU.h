/***********************************************************************
   Library      :     modbusRTU
   Description  :     รวบรวมไลบราลี่ที่เกี่ยวข้องกับ Modbus RTU
   Author       :     Tenergy Innovation Co., Ltd.
   Date         :     7 Mar 2020
   Revision     :     1.3
   Rev1.0       :     Relay ModbusRTU
   Rev1.1       :     SHT20 ModbusRTU (Temp & Humi sensor)
   Rev1.2       :     แก้ปัญหาและปรับปรุงข้อมูลของ SHT20 ModbusRTU
   Rev1.3       :     แก้ปัญหาและปรับปรุงข้อมูลของ status ของ Relay ModbusRTU
   www          :     https://www.tenergyinnovation.co.th
   Email        :     uten.boonliam@tenergyinnovation.co.th
   TEL          :     +6689-140-7205
 ***********************************************************************/
#include <Arduino.h>
#include <string.h>



/**************************************/
/*        define function define      */
/**************************************/
uint16_t crc16_update(uint16_t crc, uint8_t a);
boolean relayControl_modbusRTU(byte address, byte channel, boolean state);
byte relayStatus_modbusRTU(byte address, byte channel);
float_t sht20ReadTemp_modbusRTU(byte address);
float_t sht20ReadHumi_modbusRTU(byte address);


 /***********************************************************************
 * FUNCTION:    relayControl_modbusRTU
 * DESCRIPTION: ควบคุมการเปิด-ปิด Relay ModbusRTU 16channel
 * PARAMETERS:  byte address, boolean state
 * RETURNED:    boolean (TRUE/FALSE)
 ***********************************************************************/
boolean relayControl_modbusRTU(byte address, byte channel, boolean state)
{
    uint8_t _data_write[8];
    uint8_t _data_read[20];
    uint8_t _byte_cnt = 0;
    uint16_t _crc = 0xffff;

    _data_write[0] = address;
    _data_write[1] = 0x06;
    _data_write[2] = 0x00;
    _data_write[3] = channel;
    if(state)
        _data_write[4] = 0x01; //on relay
    else
        _data_write[4] = 0x02; //off relay
    _data_write[5] = 0x00;

    // Serial.printf("dataout[6] = %X %X %X %X %X %X \r\n",_data[0],_data[1],_data[2],_data[3],_data[4],_data[5]);

    // Generate CRC16
    for(byte _i=0; _i < sizeof(_data_write)-2; _i++){
        _crc = crc16_update(_crc, _data_write[_i]);
    } 

    // Insert CRC16 to data byte
    // Serial.printf("_crc = 0x%02X\r\n",_crc);
    _data_write[sizeof(_data_write)-1] = _crc >> 8;          
    _data_write[sizeof(_data_write)-2]= _crc - _data_write[sizeof(_data_write)-1]*0x0100 ;   


    //debug monitoring
    Serial.printf("Data write(%d): [ ",sizeof(_data_write));
    for(byte _i=0; _i<sizeof(_data_write); _i++){
      if( _data_write[_i] > 0x0F ){
        Serial.printf("0x%X ",_data_write[_i]);
      }
      else{
        Serial.printf("0x0%X ",_data_write[_i]);
      } 
    }
    Serial.printf("]\r\n");
     
    /**** Write data ****/ 
    rs485.flush();
    for(int _i=0; _i<8; _i++) rs485.write(_data_write[_i]);

    
    /**** Read data ****/
    vTaskDelay(300);
    if(rs485.available()){

    for(byte _i=0; _i<sizeof(_data_read); _i++) _data_read[_i] = 0x00; //clear buffer
    _byte_cnt = 0;

    //correct data
      do{
          _data_read[_byte_cnt++] = rs485.read();
          if(_data_read[0] == 0x00){ //แก้ไช bug เนื่องจากอ่านค่าแรกได้ 0x00
              _byte_cnt =0;
          }
      // }while(rs485.available()>0);
      }while(rs485.available()>0 && _byte_cnt<sizeof(_data_read));

      /***** Debug ****/
      Serial.printf("Data read(%d): [ ",_byte_cnt);
        for(byte _i=0; _i<_byte_cnt; _i++){
            if( _data_read[_i] > 0x0F ){
              Serial.printf("0x%X ",_data_read[_i]);
            }
            else{
              Serial.printf("0x0%X ",_data_read[_i]);
            } 
        }
        Serial.println("]");
    }
    return 1;
}

 /***********************************************************************
 * FUNCTION:    relayStatus_modbusRTU
 * DESCRIPTION: ตรวจสอบการเปิด-ปิด Relay ModbusRTU 16channel
 * PARAMETERS:  byte address, boolean state
 * RETURNED:    boolean (ON/OFF)
 ***********************************************************************/
byte relayStatus_modbusRTU(byte address, byte channel)
{
    uint16_t _crc = 0xffff;
    uint16_t _crc_r = 0xffff;

    uint8_t _data_write[8];
    uint8_t _data_read[20];
    uint8_t _byte_cnt = 0;
    uint8_t _data_check[7];

    _data_write[0] = address;
    _data_write[1] = 0x03;
    _data_write[2] = 0x00;
    _data_write[3] = channel;
    _data_write[4] = 0x00;
    _data_write[5] = 0x01;

    // Serial.printf("dataout[6] = %X %X %X %X %X %X \r\n",_data[0],_data[1],_data[2],_data[3],_data[4],_data[5]);

    // Generate CRC16
    for(byte _i=0; _i < sizeof(_data_write)-2; _i++){
        _crc = crc16_update(_crc, _data_write[_i]);
    } 

    // Insert CRC16 to data byte
    // Serial.printf("_crc = 0x%02X\r\n",_crc);
    _data_write[sizeof(_data_write)-1] = _crc >> 8;          
    _data_write[sizeof(_data_write)-2]= _crc - _data_write[sizeof(_data_write)-1]*0x0100 ;   

    //debug monitoring
    Serial.printf("Data write(%d): [ ",sizeof(_data_write));
    for(byte _i=0; _i<sizeof(_data_write); _i++){
      if( _data_write[_i] > 0x0F ){
        Serial.printf("0x%X ",_data_write[_i]);
      }
      else{
        Serial.printf("0x0%X ",_data_write[_i]);
      } 
    }
    Serial.printf("]\r\n");
     
    /**** Write data ****/ 
    rs485.flush(); 
    for(int _i=0; _i<8; _i++) rs485.write(_data_write[_i]);
    
    vTaskDelay(300);

    /**** Read data ****/
    if(rs485.available()){

    for(byte _i=0; _i<sizeof(_data_read); _i++) _data_read[_i] = 0x00; //clear buffer
    _byte_cnt = 0;

    //correct data
      do{
          _data_read[_byte_cnt++] = rs485.read();
          if(_data_read[0] == 0x00){ //แก้ไช bug เนื่องจากอ่านค่าแรกได้ 0x00
              _byte_cnt =0;
          }
      // }while(rs485.available()>0);
      }while(rs485.available()>0 && _byte_cnt<sizeof(_data_read));
      

      /***** Debug monitor ****/
      Serial.printf("Data read(%d): [ ",_byte_cnt);
        for(byte _i=0; _i<_byte_cnt; _i++){
            if( _data_read[_i] > 0x0F ){
              Serial.printf("0x%X ",_data_read[_i]);
            }
            else{
              Serial.printf("0x0%X ",_data_read[_i]);
            } 
        }
        Serial.println("]");
    }


    if(_byte_cnt == 6)
    {
      _data_check[0] = address;
      _data_check[1] = _data_read[0];
      _data_check[2] = _data_read[1];
      _data_check[3] = _data_read[2];
      _data_check[4] = _data_read[3];
      _data_check[5] = _data_read[4];
      _data_check[6] = _data_read[5]; 

      /***** Debug monitor ****/
      Serial.printf("Data check(%d): [ ",sizeof(_data_check));
        for(byte _i=0; _i<sizeof(_data_check); _i++){
            if( _data_check[_i] > 0x0F ){
              Serial.printf("0x%X ",_data_check[_i]);
            }
            else{
              Serial.printf("0x0%X ",_data_check[_i]);
            } 
        }
        Serial.println("]");
    }
    else if(_byte_cnt == 5)
    {
      _data_check[0] = address;
      _data_check[1] = 0x03;
      _data_check[2] = 0x02;
      _data_check[3] = 0x00;
      _data_check[4] = _data_read[2];
      _data_check[5] = _data_read[3];
      _data_check[6] = _data_read[4]; 

      /***** Debug monitor ****/
      Serial.printf("Data check(%d): [ ",sizeof(_data_check));
        for(byte _i=0; _i<sizeof(_data_check); _i++){
            if( _data_check[_i] > 0x0F ){
              Serial.printf("0x%X ",_data_check[_i]);
            }
            else{
              Serial.printf("0x0%X ",_data_check[_i]);
            } 
        }
        Serial.println("]");
    }
    else if(_byte_cnt == 7){
      _data_check[0] = _data_read[0];
      _data_check[1] = _data_read[1];
      _data_check[2] = _data_read[2];
      _data_check[3] = _data_read[3];
      _data_check[4] = _data_read[4];
      _data_check[5] = _data_read[5];
      _data_check[6] = _data_read[6]; 

      /***** Debug monitor ****/
      Serial.printf("Data check(%d): [ ",sizeof(_data_check));
        for(byte _i=0; _i<sizeof(_data_check); _i++){
            if( _data_check[_i] > 0x0F ){
              Serial.printf("0x%X ",_data_check[_i]);
            }
            else{
              Serial.printf("0x0%X ",_data_check[_i]);
            } 
        }
        Serial.println("]");
    }

  else if(_byte_cnt > 7)
    {
      _data_check[0] = _data_read[_byte_cnt-7];
      _data_check[1] = _data_read[_byte_cnt-6];
      _data_check[2] = _data_read[_byte_cnt-5];
      _data_check[3] = _data_read[_byte_cnt-4];
      _data_check[4] = _data_read[_byte_cnt-3];
      _data_check[5] = _data_read[_byte_cnt-2];
      _data_check[6] = _data_read[_byte_cnt-1]; 

      /***** Debug monitor ****/
      Serial.printf("Data check(%d): [ ",sizeof(_data_check));
      for(byte _i=0; _i<sizeof(_data_check); _i++){
          if( _data_check[_i] > 0x0F ){
            Serial.printf("0x%X ",_data_check[_i]);
          }
          else{
            Serial.printf("0x0%X ",_data_check[_i]);
          } 
      }
      Serial.println("]");

    }

    /*** crc check for data read ***/ 
    _crc = 0xffff;
    _crc_r = 0xffff;
    
    // Generate CRC16
    for(byte _i=0; _i < sizeof(_data_check)-2; _i++){
        _crc = crc16_update(_crc, _data_check[_i]);
    } 
    Serial.printf("Debug: _crc = 0x%X\r\n",_crc);

    // read crc byte from data_check
    _crc_r = _data_check[sizeof(_data_check)-1]; //Serial.print(">>"); Serial.println(_crc_r,HEX);
    _crc_r = _crc_r <<8; //Serial.print(">>"); Serial.println(_crc_r,HEX);
    _crc_r = _crc_r + _data_check[sizeof(_data_check)-2]; //Serial.print(">>"); Serial.println(_crc_r,HEX);       
    Serial.printf("Debug: _crc_r = 0x%X\r\n",_crc_r);

    //return ON/OFF status
    if(_crc_r == _crc)
    {
      if(_data_check[4] == 0x00)
        return 0;
      else if(_data_check[4] == 0x01)
        return 1;
      else
        return 0xff;
    }  
    else
    {
      return 0xff;
    }    
}


 /***********************************************************************
 * FUNCTION:    sht20ReadTemp_modbusRTU
 * DESCRIPTION: อ่านค่าอุณหภูมิจากเซนเซอร์ SHT22 ModbusRTU 
 * PARAMETERS:  byte address
 * RETURNED:    float temperature (C)
 ***********************************************************************/
float_t sht20ReadTemp_modbusRTU(byte address)
{
    uint16_t _crc = 0xffff;
    uint16_t _crc_r = 0xffff;
    uint16_t _temp_hex = 0xffff;

    uint8_t _data_write[8];
    uint8_t _data_read[20];
    uint8_t _byte_cnt = 0;
    uint8_t _data_check[7];

    _data_write[0] = address;
    _data_write[1] = 0x03;
    _data_write[2] = 0x00;
    _data_write[3] = 0x00;
    _data_write[4] = 0x00;
    _data_write[5] = 0x01;

    // Serial.printf("dataout[6] = %X %X %X %X %X %X \r\n",_data[0],_data[1],_data[2],_data[3],_data[4],_data[5]);

    // Generate CRC16
    for(byte _i=0; _i < sizeof(_data_write)-2; _i++){
        _crc = crc16_update(_crc, _data_write[_i]);
    } 

    // Insert CRC16 to data byte
    // Serial.printf("_crc = 0x%02X\r\n",_crc);
    _data_write[sizeof(_data_write)-1] = _crc >> 8;          
    _data_write[sizeof(_data_write)-2]= _crc - _data_write[sizeof(_data_write)-1]*0x0100 ;   

    //debug monitoring
    Serial.printf("Data write(%d): [ ",sizeof(_data_write));
    for(byte _i=0; _i<sizeof(_data_write); _i++){
      if( _data_write[_i] > 0x0F ){
        Serial.printf("0x%X ",_data_write[_i]);
      }
      else{
        Serial.printf("0x0%X ",_data_write[_i]);
      } 
    }
    Serial.printf("]\r\n");
     
    /**** Write data ****/ 
    rs485.flush(); 
    for(int _i=0; _i<8; _i++) rs485.write(_data_write[_i]);
    
    vTaskDelay(300);


    /**** Read data ****/
    if(rs485.available()){

    for(byte _i=0; _i<sizeof(_data_read); _i++) _data_read[_i] = 0x00; //clear buffer
    _byte_cnt = 0;

    //correct data
      do{
          _data_read[_byte_cnt++] = rs485.read();
          if(_data_read[0] == 0x00){ //แก้ไช bug เนื่องจากอ่านค่าแรกได้ 0x00
              _byte_cnt =0;
          }
      // }while(rs485.available()>0);
      }while(rs485.available()>0 && _byte_cnt<sizeof(_data_read));
      

      /***** Debug monitor ****/
      Serial.printf("Data read(%d): [ ",_byte_cnt);
        for(byte _i=0; _i<_byte_cnt; _i++){
            if( _data_read[_i] > 0x0F ){
              Serial.printf("0x%X ",_data_read[_i]);
            }
            else{
              Serial.printf("0x0%X ",_data_read[_i]);
            } 
        }
        Serial.println("]");
    } 


if(_byte_cnt == 7){
      _data_check[0] = _data_read[0];
      _data_check[1] = _data_read[1];
      _data_check[2] = _data_read[2];
      _data_check[3] = _data_read[3];
      _data_check[4] = _data_read[4];
      _data_check[5] = _data_read[5];
      _data_check[6] = _data_read[6]; 

      /***** Debug monitor ****/
      Serial.printf("Data check(%d): [ ",sizeof(_data_check));
      for(byte _i=0; _i<sizeof(_data_check); _i++){
          if( _data_check[_i] > 0x0F ){
            Serial.printf("0x%X ",_data_check[_i]);
          }
          else{
            Serial.printf("0x0%X ",_data_check[_i]);
          } 
      }
      Serial.println("]");
    }
    else if(_byte_cnt > 7)
    {
      _data_check[0] = _data_read[_byte_cnt-7];
      _data_check[1] = _data_read[_byte_cnt-6];
      _data_check[2] = _data_read[_byte_cnt-5];
      _data_check[3] = _data_read[_byte_cnt-4];
      _data_check[4] = _data_read[_byte_cnt-3];
      _data_check[5] = _data_read[_byte_cnt-2];
      _data_check[6] = _data_read[_byte_cnt-1]; 

      /***** Debug monitor ****/
      Serial.printf("Data check(%d): [ ",sizeof(_data_check));
      for(byte _i=0; _i<sizeof(_data_check); _i++){
          if( _data_check[_i] > 0x0F ){
            Serial.printf("0x%X ",_data_check[_i]);
          }
          else{
            Serial.printf("0x0%X ",_data_check[_i]);
          } 
      }
      Serial.println("]");

    }
   
    /*** crc check for data read ***/ 
    _crc = 0xffff;
    _crc_r = 0xffff;
    
    // Generate CRC16
    for(byte _i=0; _i < sizeof(_data_check)-2; _i++){
        _crc = crc16_update(_crc, _data_check[_i]);
    } 
    Serial.printf("Debug: _crc = 0x%X\r\n",_crc);

    // read crc byte from data_check
    _crc_r = _data_check[sizeof(_data_check)-1]; //Serial.print(">>"); Serial.println(_crc_r,HEX);
    _crc_r = _crc_r <<8; //Serial.print(">>"); Serial.println(_crc_r,HEX);
    _crc_r = _crc_r + _data_check[sizeof(_data_check)-2]; //Serial.print(">>"); Serial.println(_crc_r,HEX);       
    Serial.printf("Debug: _crc_r = 0x%X\r\n",_crc_r);

    //return ON/OFF status
    if(_crc_r == _crc)
    {
      _temp_hex = _data_check[3];  //Serial.printf("Debug: _temp_hex = 0x%X\r\n",_temp_hex);
      _temp_hex = _temp_hex<<8;   //Serial.printf("Debug: _temp_hex = 0x%X\r\n",_temp_hex);
      _temp_hex = _temp_hex | _data_check[4]; //Serial.printf("Debug: _temp_hex = 0x%X(%d)\r\n",_temp_hex,_temp_hex);

      Serial.printf("Info: Temperature => %.1f\r\n",(float)((float)_temp_hex/10));
      return (float)((float)_temp_hex/10);
    }  
    else
    {
      Serial.printf("Error: crc16\r\n");
      return 0xffff;
    }    
}

/***********************************************************************
 * FUNCTION:    sht20ReadHumi_modbusRTU
 * DESCRIPTION: อ่านค่าความชื้นในอากาศจากเซนเซอร์ SHT22 ModbusRTU 
 * PARAMETERS:  byte address
 * RETURNED:    float temperature (C)
 ***********************************************************************/
float_t sht20ReadHumi_modbusRTU(byte address)
{
    uint16_t _crc = 0xffff;
    uint16_t _crc_r = 0xffff;
    uint16_t _humi_hex = 0xffff;

    uint8_t _data_write[8];
    uint8_t _data_read[20];
    uint8_t _byte_cnt = 0;
    uint8_t _data_check[7];

    _data_write[0] = address;
    _data_write[1] = 0x03;
    _data_write[2] = 0x00;
    _data_write[3] = 0x01;
    _data_write[4] = 0x00;
    _data_write[5] = 0x01;

    // Serial.printf("dataout[6] = %X %X %X %X %X %X \r\n",_data[0],_data[1],_data[2],_data[3],_data[4],_data[5]);

    // Generate CRC16
    for(byte _i=0; _i < sizeof(_data_write)-2; _i++){
        _crc = crc16_update(_crc, _data_write[_i]);
    } 

    // Insert CRC16 to data byte
    // Serial.printf("_crc = 0x%02X\r\n",_crc);
    _data_write[sizeof(_data_write)-1] = _crc >> 8;          
    _data_write[sizeof(_data_write)-2]= _crc - _data_write[sizeof(_data_write)-1]*0x0100 ;   

    //debug monitoring
    Serial.printf("Data write(%d): [ ",sizeof(_data_write));
    for(byte _i=0; _i<sizeof(_data_write); _i++){
      if( _data_write[_i] > 0x0F ){
        Serial.printf("0x%X ",_data_write[_i]);
      }
      else{
        Serial.printf("0x0%X ",_data_write[_i]);
      } 
    }
    Serial.printf("]\r\n");
     
    /**** Write data ****/ 
    rs485.flush(); 
    for(int _i=0; _i<8; _i++) rs485.write(_data_write[_i]);
    
    vTaskDelay(300);


    /**** Read data ****/
    if(rs485.available()){

    for(byte _i=0; _i<sizeof(_data_read); _i++) _data_read[_i] = 0x00; //clear buffer
    _byte_cnt = 0;

    //correct data
      do{
          _data_read[_byte_cnt++] = rs485.read();
          if(_data_read[0] == 0x00){ //แก้ไช bug เนื่องจากอ่านค่าแรกได้ 0x00
              _byte_cnt =0;
          }
      // }while(rs485.available()>0);
      }while(rs485.available()>0 && _byte_cnt<sizeof(_data_read));
      

      /***** Debug monitor ****/
      Serial.printf("Data read(%d): [ ",_byte_cnt);
        for(byte _i=0; _i<_byte_cnt; _i++){
            if( _data_read[_i] > 0x0F ){
              Serial.printf("0x%X ",_data_read[_i]);
            }
            else{
              Serial.printf("0x0%X ",_data_read[_i]);
            } 
        }
        Serial.println("]");
    }


   if(_byte_cnt == 7){
      _data_check[0] = _data_read[0];
      _data_check[1] = _data_read[1];
      _data_check[2] = _data_read[2];
      _data_check[3] = _data_read[3];
      _data_check[4] = _data_read[4];
      _data_check[5] = _data_read[5];
      _data_check[6] = _data_read[6]; 

      /***** Debug monitor ****/
      Serial.printf("Data check(%d): [ ",sizeof(_data_check));
        for(byte _i=0; _i<sizeof(_data_check); _i++){
            if( _data_check[_i] > 0x0F ){
              Serial.printf("0x%X ",_data_check[_i]);
            }
            else{
              Serial.printf("0x0%X ",_data_check[_i]);
            } 
        }
        Serial.println("]");
    }
    else if(_byte_cnt > 7)
    {
      _data_check[0] = _data_read[_byte_cnt-7];
      _data_check[1] = _data_read[_byte_cnt-6];
      _data_check[2] = _data_read[_byte_cnt-5];
      _data_check[3] = _data_read[_byte_cnt-4];
      _data_check[4] = _data_read[_byte_cnt-3];
      _data_check[5] = _data_read[_byte_cnt-2];
      _data_check[6] = _data_read[_byte_cnt-1]; 

      /***** Debug monitor ****/
      Serial.printf("Data check(%d): [ ",sizeof(_data_check));
      for(byte _i=0; _i<sizeof(_data_check); _i++){
          if( _data_check[_i] > 0x0F ){
            Serial.printf("0x%X ",_data_check[_i]);
          }
          else{
            Serial.printf("0x0%X ",_data_check[_i]);
          } 
      }
      Serial.println("]");

    }
   
    /*** crc check for data read ***/ 
    _crc = 0xffff;
    _crc_r = 0xffff;
    
    // Generate CRC16
    for(byte _i=0; _i < sizeof(_data_check)-2; _i++){
        _crc = crc16_update(_crc, _data_check[_i]);
    } 
    Serial.printf("Debug: _crc = 0x%X\r\n",_crc);

    // read crc byte from data_check
    _crc_r = _data_check[sizeof(_data_check)-1]; //Serial.print(">>"); Serial.println(_crc_r,HEX);
    _crc_r = _crc_r <<8; //Serial.print(">>"); Serial.println(_crc_r,HEX);
    _crc_r = _crc_r + _data_check[sizeof(_data_check)-2]; //Serial.print(">>"); Serial.println(_crc_r,HEX);       
    Serial.printf("Debug: _crc_r = 0x%X\r\n",_crc_r);

    //return ON/OFF status
    if(_crc_r == _crc)
    {
      _humi_hex = _data_check[3];  //Serial.printf("Debug: _humi_hex = 0x%X\r\n",_humi_hex);
      _humi_hex = _humi_hex<<8;   //Serial.printf("Debug: _humi_hex = 0x%X\r\n",_humi_hex);
      _humi_hex = _humi_hex | _data_check[4]; //Serial.printf("Debug: _humi_hex = 0x%X(%d)\r\n",_humi_hex,_humi_hex);

      Serial.printf("Info: Humidity => %.1f\r\n",(float)((float)_humi_hex/10));
      return (float)((float)_humi_hex/10);

    }  
    else
    {
      Serial.printf("Error: crc16");
      return 0xffff;
    }    
}

 /***********************************************************************
 * FUNCTION:    crc16_update
 * DESCRIPTION: CRC16 check
 * PARAMETERS:  uint16_t crc, uint8_t a
 * RETURNED:    uint16_t
 ***********************************************************************/
uint16_t crc16_update(uint16_t crc, uint8_t a)
    {
  int i;

  crc ^= a;
  for (i = 0; i < 8; ++i)
  {
      if (crc & 1)
    crc = (crc >> 1) ^ 0xA001;
      else
    crc = (crc >> 1);
  }

  return crc;
    }