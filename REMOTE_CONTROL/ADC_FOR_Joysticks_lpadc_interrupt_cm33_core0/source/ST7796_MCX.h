/*
 * ST7796_MCX.h
 * * Hardware SPI Driver for ST7796 LCD on MCXN947
 * Uses LPSPI9 (Flexcomm 9) and GPIOs
 */

#ifndef ST7796_MCX_H_
#define ST7796_MCX_H_

#include "fsl_common.h"
#include "fsl_lpspi.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Hardware Configuration (Arduino Header Compatibility)
 ******************************************************************************/
/* SPI Peripheral: Flexcomm 9 */
#define ST7796_SPI_MASTER_BASE      LPSPI9
#define ST7796_SPI_MASTER_IRQN      FLEXCOMM9_IRQn
#define ST7796_SPI_SRC_CLK_FREQ     CLOCK_GetLPFlexCommClkFreq(9u)
#define ST7796_SPI_BAUDRATE         20000000U  /* 20MHz (ST7796 handles up to 40MHz) */

/* Control Pins (Mapped to PORT2) */
#define ST7796_GPIO_PORT            GPIO2

#define ST7796_DC_PIN               2U   /* D9  (P2_2) - Data/Command */
#define ST7796_CS_PIN               6U   /* D10 (P2_6) - Chip Select */
#define ST7796_RST_PIN              7U   /* D8  (P2_7) - Reset */

/* Note: Backlight (BL) usually on P3_x or hardwired VCC. Add if needed. */
#define ST7796_BL_GPIO              GPIO1
#define ST7796_BL_PIN               22U  /* D2 (P1_22) */


/* Screen Resolution */
#define ST7796_WIDTH                320
#define ST7796_HEIGHT               480

/*******************************************************************************
 * API Prototypes
 ******************************************************************************/

/*!
 * @brief Initialize the LPSPI peripheral and GPIO pins for the screen.
 * Call this after BOARD_InitBootPins() and Clock Setup.
 */
void ST7796_Init(void);

/*!
 * @brief Set the address window for drawing.
 * @param x1 Start X
 * @param y1 Start Y
 * @param x2 End X
 * @param y2 End Y
 */
void ST7796_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/*!
 * @brief Write a color block to the screen (Blocking).
 * Used for flushing buffers (e.g., from LVGL).
 * @param color_buff Pointer to the RGB565 color data.
 * @param size_bytes Number of bytes to write.
 */
void ST7796_WritePixels(uint8_t *color_buff, size_t size_bytes);

/*!
 * @brief Fill the entire screen with a single color.
 * @param color RGB565 color value.
 */
void ST7796_FillScreen(uint16_t color);

#endif /* ST7796_MCX_H_ */
