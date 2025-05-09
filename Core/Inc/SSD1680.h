#ifndef __SSD1680_H
#define __SSD1680_H

#include "main.h"
// Pin 定義
#define EPD_RST_PIN     GPIO_PIN_6
#define EPD_RST_PORT    GPIOC

#define EPD_DC_PIN      GPIO_PIN_9
#define EPD_DC_PORT     GPIOA

#define EPD_CS_PIN      GPIO_PIN_5
#define EPD_CS_PORT     GPIOA

#define EPD_BUSY_PIN    GPIO_PIN_10
#define EPD_BUSY_PORT   GPIOA

// 指令定義
#define DRIVER_OUTPUT_CONTROL          0x01
#define BOOSTER_SOFT_START_CONTROL     0x0C
#define GATE_SCAN_START_POSITION       0x0F
#define DEEP_SLEEP_MODE                0x10
#define DATA_ENTRY_MODE_SETTING        0x11
#define SW_RESET                       0x12
#define TEMPERATURE_SENSOR_CONTROL     0x1A
#define MASTER_ACTIVATION              0x20
#define DISPLAY_UPDATE_CONTROL_1       0x21
#define DISPLAY_UPDATE_CONTROL_2       0x22
#define WRITE_RAM                      0x24
#define WRITE_VCOM_REGISTER            0x2C
#define WRITE_LUT                      0x32
#define SET_DUMMY_LINE_PERIOD          0x3A
#define SET_GATE_TIME                  0x3B
#define BORDER_WAVEFORM_CONTROL        0x3C
#define SET_RAM_X_ADDRESS_START_END    0x44
#define SET_RAM_Y_ADDRESS_START_END    0x45
#define SET_RAM_X_ADDRESS_COUNTER      0x4E
#define SET_RAM_Y_ADDRESS_COUNTER      0x4F


void Epd_Init(void);
void Epd_Clear(void);
void Epd_DisplayFrame(const uint8_t *buffer);
void Epd_WaitBusy(void);

#endif
