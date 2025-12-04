/*
 * source/RobotGUI.c
 */

#include "RobotGUI.h"
#include <stdio.h>

/*******************************************************************************
 * Variables
 ******************************************************************************/
static lv_obj_t * chart;
static lv_chart_series_t * ser_vx;
static lv_chart_series_t * ser_vy;
static lv_chart_series_t * ser_phi;

static lv_obj_t * lbl_vx_val;
static lv_obj_t * lbl_vy_val;
static lv_obj_t * lbl_phi_val;

/*******************************************************************************
 * Private Functions
 ******************************************************************************/
static void style_create_dark(void)
{
    /* Set a dark background for the main screen */
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x1E1E1E), 0);
    lv_obj_set_style_text_color(lv_scr_act(), lv_color_white(), 0);
}

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

void RobotGUI_Init(void)
{
    style_create_dark();

    /* --- 1. HEADER --- */
    lv_obj_t * header = lv_obj_create(lv_scr_act());
    lv_obj_set_size(header, 300, 50);
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_bg_color(header, lv_color_hex(0x404040), 0);
    lv_obj_set_style_border_width(header, 0, 0);

    lv_obj_t * title = lv_label_create(header);
    lv_label_set_text(title, "NXP REMOTE CONTROL");
    lv_obj_center(title);


    /* --- 2. CHART (Speeds) --- */
    chart = lv_chart_create(lv_scr_act());
    lv_obj_set_size(chart, 280, 200);
    lv_obj_align(chart, LV_ALIGN_TOP_MID, 0, 80);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);

    /* Range: -100 to 100 (representing -1.00 to 1.00 m/s) */
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -100, 100);
    lv_chart_set_point_count(chart, 50);
    lv_chart_set_div_line_count(chart, 5, 7);

    /* Style */
    lv_obj_set_style_bg_color(chart, lv_color_hex(0x2D2D2D), 0);
    lv_obj_set_style_line_width(chart, 2, LV_PART_ITEMS);

    /* [FIXED] Hide points by setting width and height to 0 */
    lv_obj_set_style_size(chart, 0, 0, LV_PART_INDICATOR);

    /* Series */
    ser_vx  = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    ser_vy  = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    ser_phi = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);


    /* --- 3. DATA LABELS --- */
    lv_obj_t * cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 300, 120);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x2D2D2D), 0);
    lv_obj_set_style_border_width(cont, 0, 0);

    /* Labels */
    lv_obj_t * lbl_vx_title = lv_label_create(cont);
    lv_label_set_text(lbl_vx_title, "VX (m/s):");
    lv_obj_set_style_text_color(lbl_vx_title, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_align(lbl_vx_title, LV_ALIGN_TOP_LEFT, 10, 10);

    lbl_vx_val = lv_label_create(cont);
    lv_label_set_text(lbl_vx_val, "0.00");
    lv_obj_align(lbl_vx_val, LV_ALIGN_TOP_RIGHT, -20, 10);

    lv_obj_t * lbl_vy_title = lv_label_create(cont);
    lv_label_set_text(lbl_vy_title, "VY (m/s):");
    lv_obj_set_style_text_color(lbl_vy_title, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_align(lbl_vy_title, LV_ALIGN_TOP_LEFT, 10, 40);

    lbl_vy_val = lv_label_create(cont);
    lv_label_set_text(lbl_vy_val, "0.00");
    lv_obj_align(lbl_vy_val, LV_ALIGN_TOP_RIGHT, -20, 40);

    lv_obj_t * lbl_phi_title = lv_label_create(cont);
    lv_label_set_text(lbl_phi_title, "PHI (rad/s):");
    lv_obj_set_style_text_color(lbl_phi_title, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_align(lbl_phi_title, LV_ALIGN_TOP_LEFT, 10, 70);

    lbl_phi_val = lv_label_create(cont);
    lv_label_set_text(lbl_phi_val, "0.00");
    lv_obj_align(lbl_phi_val, LV_ALIGN_TOP_RIGHT, -20, 70);
}

void RobotGUI_Update(float vx, float vy, float phi)
{
    if(!chart) return;

    /* 1. Update Chart Series */
    /* Multiply by 100 to convert float 0.50 -> integer 50 for charting */
    lv_chart_set_next_value(chart, ser_vx,  (int32_t)(vx * 100));
    lv_chart_set_next_value(chart, ser_vy,  (int32_t)(vy * 100));
    lv_chart_set_next_value(chart, ser_phi, (int32_t)(phi * 20)); // Scale Phi differently if needed

    /* 2. Update Text Labels */
    /* Use static buffers to avoid heap fragmentation if using small libc */
    static char buf_vx[16];
    static char buf_vy[16];
    static char buf_phi[16];

    /* Format float to string */
    /* Note: If your printf doesn't support floats, cast to int parts */
    snprintf(buf_vx, sizeof(buf_vx), "%.2f", vx);
    snprintf(buf_vy, sizeof(buf_vy), "%.2f", vy);
    snprintf(buf_phi, sizeof(buf_phi), "%.2f", phi);

    lv_label_set_text(lbl_vx_val, buf_vx);
    lv_label_set_text(lbl_vy_val, buf_vy);
    lv_label_set_text(lbl_phi_val, buf_phi);

    /* Refresh chart immediately (optional, task handler will do it anyway) */
    /* lv_chart_refresh(chart); */
}
