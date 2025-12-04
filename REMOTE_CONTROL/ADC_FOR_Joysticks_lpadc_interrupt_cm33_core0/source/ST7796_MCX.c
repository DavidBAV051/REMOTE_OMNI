/*
 * ST7796_MCX.c
 * Implementation of ST7796 Driver for MCXN947
 */

#include "ST7796_MCX.h"
#include "board.h"

/*******************************************************************************
 * Private Helper Functions
 ******************************************************************************/

static void ST7796_Select(bool enable)
{
    if (enable)
    {
        GPIO_PinWrite(ST7796_GPIO_PORT, ST7796_CS_PIN, 0);
    }
    else
    {
        GPIO_PinWrite(ST7796_GPIO_PORT, ST7796_CS_PIN, 1);
    }
}

static void ST7796_WriteByte(uint8_t data)
{
    lpspi_transfer_t xfer;
    xfer.txData      = &data;
    xfer.rxData      = NULL;
    xfer.dataSize    = 1;
    xfer.configFlags = kLPSPI_MasterPcsContinuous; /* Keep CS asserted if hardware controlled */

    LPSPI_MasterTransferBlocking(ST7796_SPI_MASTER_BASE, &xfer);
}

static void ST7796_WriteCmd(uint8_t cmd)
{
    GPIO_PinWrite(ST7796_GPIO_PORT, ST7796_DC_PIN, 0); /* Command Mode */
    ST7796_Select(true);
    ST7796_WriteByte(cmd);
    ST7796_Select(false);
}

static void ST7796_WriteData(uint8_t data)
{
    GPIO_PinWrite(ST7796_GPIO_PORT, ST7796_DC_PIN, 1); /* Data Mode */
    ST7796_Select(true);
    ST7796_WriteByte(data);
    ST7796_Select(false);
}

static void ST7796_WriteData16(uint16_t data)
{
    uint8_t buf[2];
    buf[0] = (data >> 8) & 0xFF;
    buf[1] = data & 0xFF;

    GPIO_PinWrite(ST7796_GPIO_PORT, ST7796_DC_PIN, 1); /* Data Mode */
    ST7796_Select(true);

    lpspi_transfer_t xfer;
    xfer.txData      = buf;
    xfer.rxData      = NULL;
    xfer.dataSize    = 2;
    xfer.configFlags = kLPSPI_MasterPcsContinuous;

    LPSPI_MasterTransferBlocking(ST7796_SPI_MASTER_BASE, &xfer);

    ST7796_Select(false);
}

/*******************************************************************************
 * Public API
 ******************************************************************************/

void ST7796_Init(void)
{
    lpspi_master_config_t masterConfig;
    gpio_pin_config_t outputConfig = {kGPIO_DigitalOutput, 1};

    /* 1. Init GPIOs */
    /* P2_2 (DC), P2_6 (CS), P2_7 (RST) */
    GPIO_PinInit(ST7796_GPIO_PORT, ST7796_DC_PIN, &outputConfig);
    GPIO_PinInit(ST7796_GPIO_PORT, ST7796_CS_PIN, &outputConfig);
    GPIO_PinInit(ST7796_GPIO_PORT, ST7796_RST_PIN, &outputConfig);

    /* Port 3 Backlight Pin */
	GPIO_PinInit(ST7796_BL_GPIO, ST7796_BL_PIN, &outputConfig); // <--- NEW
	GPIO_PinWrite(ST7796_BL_GPIO, ST7796_BL_PIN, 1);            // <--- Turn Backlight ON

    /* 2. Init LPSPI9 */
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = ST7796_SPI_BAUDRATE;
    masterConfig.cpol = 0;
    masterConfig.cpha = 0;
    masterConfig.whichPcs = kLPSPI_Pcs0; /* We use GPIO CS, but this config is required */
    masterConfig.pcsToSckDelayInNanoSec = 50;
    masterConfig.lastSckToPcsDelayInNanoSec = 50;
    masterConfig.betweenTransferDelayInNanoSec = 50;

    LPSPI_MasterInit(ST7796_SPI_MASTER_BASE, &masterConfig, ST7796_SPI_SRC_CLK_FREQ);

    /* 3. Hard Reset */
    GPIO_PinWrite(ST7796_GPIO_PORT, ST7796_CS_PIN, 1);
    GPIO_PinWrite(ST7796_GPIO_PORT, ST7796_RST_PIN, 0);
    SDK_DelayAtLeastUs(10000, SystemCoreClock); /* 10ms */
    GPIO_PinWrite(ST7796_GPIO_PORT, ST7796_RST_PIN, 1);
    SDK_DelayAtLeastUs(120000, SystemCoreClock); /* 120ms */

    /* 4. Initialization Sequence */
    ST7796_WriteCmd(0x01); /* SW Reset */
    SDK_DelayAtLeastUs(120000, SystemCoreClock);

    ST7796_WriteCmd(0x11); /* Sleep Out */
    SDK_DelayAtLeastUs(120000, SystemCoreClock);

    ST7796_WriteCmd(0x3A); /* Pixel Format */
    ST7796_WriteData(0x55); /* 16 bits/pixel */

    ST7796_WriteCmd(0x36); /* Memory Access Control */
    ST7796_WriteData(0x88); /* MX | BGR (Adjust this 0x48/0x88/0x28 based on rotation) */

    //ST7796_WriteCmd(0x21); /* Invertion On */
    ST7796_WriteCmd(0x29); /* Display On */
    SDK_DelayAtLeastUs(10000, SystemCoreClock);
}

void ST7796_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    ST7796_WriteCmd(0x2A); /* Column Address Set */
    ST7796_WriteData16(x1);
    ST7796_WriteData16(x2);

    ST7796_WriteCmd(0x2B); /* Row Address Set */
    ST7796_WriteData16(y1);
    ST7796_WriteData16(y2);

    ST7796_WriteCmd(0x2C); /* Memory Write */
}

void ST7796_WritePixels(uint8_t *color_buff, size_t size_bytes)
{
    GPIO_PinWrite(ST7796_GPIO_PORT, ST7796_DC_PIN, 1); /* Data Mode */
    ST7796_Select(true);

    lpspi_transfer_t xfer;
    xfer.txData      = color_buff;
    xfer.rxData      = NULL;
    xfer.dataSize    = size_bytes;
    xfer.configFlags = kLPSPI_MasterPcsContinuous;

    /* Blocking transfer for simplicity. Use DMA for higher performance later. */
    LPSPI_MasterTransferBlocking(ST7796_SPI_MASTER_BASE, &xfer);

    ST7796_Select(false);
}

void ST7796_FillScreen(uint16_t color)
{
    /* Simple fill implementation */
    ST7796_SetWindow(0, 0, ST7796_WIDTH - 1, ST7796_HEIGHT - 1);

    /* Create a small line buffer to speed up fill */
    uint8_t line_buff[320 * 2];
    for(int i=0; i<320; i++) {
        line_buff[i*2]     = (color >> 8) & 0xFF;
        line_buff[i*2 + 1] = color & 0xFF;
    }

    /* Send buffer repeatedly */
    GPIO_PinWrite(ST7796_GPIO_PORT, ST7796_DC_PIN, 1);
    ST7796_Select(true);

    for(int y=0; y<ST7796_HEIGHT; y++) {
        lpspi_transfer_t xfer;
        xfer.txData = line_buff;
        xfer.rxData = NULL;
        xfer.dataSize = sizeof(line_buff);
        xfer.configFlags = kLPSPI_MasterPcsContinuous;
        LPSPI_MasterTransferBlocking(ST7796_SPI_MASTER_BASE, &xfer);
    }

    ST7796_Select(false);
}
