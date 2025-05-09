/*
 * i2c_slave.c
 *
 *  Created on: Jun 9, 2023
 *      Author: controllerstech
 */


#include "main.h"
#include "i2c_slave.h"

extern I2C_HandleTypeDef hi2c2;

#define RxSIZE  6
uint8_t RxData[RxSIZE];
uint8_t i2c_tx_buf[1] = { 0x42 };  // 主機 read 時回傳這個
uint8_t i2c_rx_buf[1];             // 主機 write 時放到這裡
int count = 0;

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    if (hi2c->Instance != I2C2) return;

    if (TransferDirection == I2C_DIRECTION_RECEIVE) {
        HAL_I2C_Slave_Transmit_IT(&hi2c2, i2c_tx_buf, sizeof(i2c_tx_buf));
    } else {
        HAL_I2C_Slave_Receive_IT(&hi2c2, i2c_rx_buf, sizeof(i2c_rx_buf));
    }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	count++;
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}
