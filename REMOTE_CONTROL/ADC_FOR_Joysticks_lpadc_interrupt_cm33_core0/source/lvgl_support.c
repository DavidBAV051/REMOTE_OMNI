/*
 * source/lvgl_support.c
 * Adapted for LVGL v9
 */

#include "lvgl_support.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* * In LVGL v9, buffers are just raw arrays.
 * We align them to 4 bytes for DMA safety (even if we use blocking SPI).
 */
static uint8_t buf1[LVGL_BUF_SIZE_BYTES] __attribute__((aligned(4)));

/* Display Object Pointer */
static lv_display_t * disp;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* * FLUSH CALLBACK (v9 style)
 * LVGL calls this to send pixel data to the screen.
 */
void my_disp_flush(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    /* 1. Calculate transfer size */
    uint32_t width = (area->x2 - area->x1 + 1);
    uint32_t height = (area->y2 - area->y1 + 1);
    uint32_t size_bytes = width * height * 2; /* RGB565 = 2 bytes/pixel */

    /* 2. Set the window on the ST7796 */
    ST7796_SetWindow(area->x1, area->y1, area->x2, area->y2);

    /* 3. Send the color data via SPI */
    /* px_map is essentially the color buffer */
    ST7796_WritePixels(px_map, size_bytes);

    /* 4. Tell LVGL we are ready */
    lv_display_flush_ready(display);
}

void lv_port_disp_init(void)
{
    /* 1. Initialize Low Level Hardware Driver */
    ST7796_Init();

    /* 2. Create the Display Object (v9 API) */
    disp = lv_display_create(ST7796_HEIGHT , ST7796_WIDTH);


    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);

    /* 3. Set the Flush Callback */
    lv_display_set_flush_cb(disp, my_disp_flush);

    /* 4. Initialize the Buffers */
    /* * Mode: Partial (Standard for MCU with limited RAM)
     * buf1: Main buffer
     * buf2: NULL (Single buffering)
     * Size: In bytes!
     */
    lv_display_set_buffers(disp, buf1, NULL, LVGL_BUF_SIZE_BYTES, LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Optional: Rotate if needed */
    /* lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_0); */
}
