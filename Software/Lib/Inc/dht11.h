/*
 * dht11.h
 *
 *  Created on: Feb 8, 2025
 *      Author: dougl
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_



//IO方向设置
void DS18B20_IO_IN();
void DS18B20_IO_OUT();

uint8_t DHT11_Init();//初始化DHT11
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi);//读取温湿度
uint8_t DHT11_Read_Byte();//读出一个字节
uint8_t DHT11_Read_Bit();//读出一个位
uint8_t DHT11_Check();//检测是否存在DHT11
void DHT11_Rst();//复位DHT11



#endif /* INC_DHT11_H_ */
