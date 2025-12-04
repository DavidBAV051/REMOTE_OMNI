/*
 * lvgl_support.h
 *
 *  Created on: Dec 3, 2025
 *      Author: diego
 */

#ifndef LVGL_SUPPORT_H_
#define LVGL_SUPPORT_H_

#include "lvgl.h"
#include "ST7796_MCX.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* * Buffer size: 1/10th of screen
 * 320 * 480 = 153,600 pixels
 * 1/10th ~ 15,360 pixels
 * RGB565 = 2 bytes per pixel -> ~30KB buffer
 */
#define LVGL_BUF_SIZE_PIXELS (320 * 48)
#define LVGL_BUF_SIZE_BYTES  (LVGL_BUF_SIZE_PIXELS * 2)



void my_disp_flush(lv_display_t * display, const lv_area_t * area, uint8_t * px_map);
void lv_port_disp_init(void);

#endif /* LVGL_SUPPORT_H_ */
