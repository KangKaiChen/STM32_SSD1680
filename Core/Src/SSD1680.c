#include "SSD1680.h"
#include "main.h"
#include "stm32c0xx_hal_spi.h"

static void Epd_Reset(void);
static void Epd_Select(void);
static void Epd_Unselect(void);
static void Epd_SendCommand(uint8_t cmd);
static void Epd_SendData(uint8_t data);

static const uint8_t lut_full_update[] = {
    0x80, 0x60, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0x03, 0x00, 0x02, 0x00,
    0x0F, 0x03, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00
};
extern SPI_HandleTypeDef hspi1;
void Epd_Init(void) {
    Epd_Reset();

    Epd_SendCommand(SW_RESET);
    Epd_WaitBusy();

    // 設定掃描方向 & 面板大小
    Epd_SendCommand(DRIVER_OUTPUT_CONTROL);
    Epd_SendData(0x27);  // 296 - 1 = 0x0127 → LSB
    Epd_SendData(0x01);  // MSB
    Epd_SendData(0x00);  // GD/LUT

    Epd_SendCommand(DATA_ENTRY_MODE_SETTING);
    Epd_SendData(0x03);  // X+, Y+ 增加

    Epd_SendCommand(SET_RAM_X_ADDRESS_START_END);
    Epd_SendData(0x00);
    Epd_SendData(0x0F);  // 128px / 8 = 16 - 1 = 15 = 0x0F

    Epd_SendCommand(SET_RAM_Y_ADDRESS_START_END);
    Epd_SendData(0x00);  // LSB
    Epd_SendData(0x00);  // MSB
    Epd_SendData(0x27);  // 296 - 1 = 0x0127 → LSB
    Epd_SendData(0x01);  // MSB

    Epd_SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    Epd_SendData(0x00);

    Epd_SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    Epd_SendData(0x00);
    Epd_SendData(0x00);

    // 寫 LUT
    Epd_SendCommand(WRITE_LUT);
    for (int i = 0; i < sizeof(lut_full_update); i++) {
        Epd_SendData(lut_full_update[i]);
    }

    Epd_WaitBusy();
}

void Epd_Clear(void) {
    // 設定範圍
    Epd_SendCommand(SET_RAM_X_ADDRESS_START_END);
    Epd_SendData(0x00);
    Epd_SendData(0x0F);  // 16 bytes - 1

    Epd_SendCommand(SET_RAM_Y_ADDRESS_START_END);
    Epd_SendData(0x00);
    Epd_SendData(0x00);
    Epd_SendData(0x27);  // 296 - 1
    Epd_SendData(0x01);

    Epd_SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    Epd_SendData(0x00);

    Epd_SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    Epd_SendData(0x00);
    Epd_SendData(0x00);

    // 寫入白色畫面
    Epd_SendCommand(WRITE_RAM);
    for (int i = 0; i < (128 / 8) * 296; i++) {
        Epd_SendData(0xFF);  // 白色
    }

    Epd_SendCommand(DISPLAY_UPDATE_CONTROL_2);
    Epd_SendData(0xF7);
    Epd_SendCommand(MASTER_ACTIVATION);
    Epd_WaitBusy();
}

void Epd_DisplayFrame(const uint8_t *buffer) {
    Epd_SendCommand(WRITE_RAM);
    for (int i = 0; i < (128 / 8) * 296; i++) {
        Epd_SendData(buffer[i]);
    }

    Epd_SendCommand(DISPLAY_UPDATE_CONTROL_2);
    Epd_SendData(0xF7);
    Epd_SendCommand(MASTER_ACTIVATION);
    Epd_WaitBusy();
}

void Epd_WaitBusy(void) {
    while (HAL_GPIO_ReadPin(EPD_BUSY_PORT, EPD_BUSY_PIN) == GPIO_PIN_SET) {
        HAL_Delay(10);
    }
}

static void Epd_Reset(void) {
    HAL_GPIO_WritePin(EPD_RST_PORT, EPD_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(EPD_RST_PORT, EPD_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(200);
}

static void Epd_Select(void) {
    HAL_GPIO_WritePin(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_RESET);
}

static void Epd_Unselect(void) {
    HAL_GPIO_WritePin(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_SET);
}

static void Epd_SendCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(EPD_DC_PORT, EPD_DC_PIN, GPIO_PIN_RESET);
    Epd_Select();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    Epd_Unselect();
}

static void Epd_SendData(uint8_t data) {
    HAL_GPIO_WritePin(EPD_DC_PORT, EPD_DC_PIN, GPIO_PIN_SET);
    Epd_Select();
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
    Epd_Unselect();
}
