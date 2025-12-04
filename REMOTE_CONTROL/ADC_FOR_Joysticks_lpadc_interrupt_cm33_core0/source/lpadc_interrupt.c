/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022, 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_lpadc.h"
#include "ESP_SPI.h"
#include "RemoteData.h"
#include "ST7796_MCX.h"
#include "lvgl_support.h"
#include "lvgl.h"
#include "RobotGUI.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SPI Configuration */
#define LPSPI_MASTER_CLK_FREQ CLOCK_GetLPFlexCommClkFreq(1u)
#define REMOTE_LPSPI_BASE     LPSPI1
#define REMOTE_LPSPI_IRQN     LP_FLEXCOMM1_IRQn
#define REMOTE_LPSPI_PCS      kLPSPI_Pcs0

/* ADC & Joystick Constants */
#define MAX_ADC 4095.0f
#define ADC_CENTER 2048.0f
#define DEADZONE 150

/* Robot Speed Limits */
#define MAX_LINEAR_SPEED  0.5f  // m/s
#define MAX_ANGULAR_SPEED 2.0f  // rad/s

/* ADC Channels */
#define JOYSTICK_LPADC_USER_CHANNEL     1U
#define LEFT_JOY_CHANNEL                2U

#define XVALUE_LPADC_USER_CMDID         1U
#define YVALUE_LPADC_USER_CMDID         2U
#define LEFT_X_LPADC_USER_CMDID         3U

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_LpadcConversionCompletedFlag = false;
volatile uint32_t ADC_Valx = 0;
volatile uint32_t ADC_Valy = 0;
volatile uint32_t ADC_Val_LeftX = 0;

/* SPI Buffers */
static uint8_t txBuffer[ESP_SPI_TRANSFER_SIZE] = {0};
static uint8_t rxBuffer[ESP_SPI_TRANSFER_SIZE] = {0};
static uint32_t packet_count = 0;

/*******************************************************************************
 * Helper Functions
 ******************************************************************************/

/* Connect SPI Driver Interrupt */
void LP_FLEXCOMM1_IRQHandler(void)
{
    ESP_SPI_MasterIRQHandler();
}

/* Map raw ADC to speed with deadzone */
float MapJoystickToSpeed(uint32_t raw, float max_speed, bool invert)
{
    float val = (float)raw;
    float result = 0.0f;
    float center = ADC_CENTER;

    if (val > (center + DEADZONE))
    {
        result = (val - (center + DEADZONE)) / (MAX_ADC - (center + DEADZONE));
    }
    else if (val < (center - DEADZONE))
    {
        result = (val - (center - DEADZONE)) / (center - DEADZONE);
    }
    else
    {
        return 0.0f;
    }

    if (result > 1.0f) result = 1.0f;
    if (result < -1.0f) result = -1.0f;

    return result * max_speed * (invert ? -1.0f : 1.0f);
}

void LPADC_IRQHandler_Func(void)
{
    lpadc_conv_result_t tmpResultStruct;

    while (LPADC_GetConvResult(DEMO_LPADC_BASE, &tmpResultStruct, 0U))
    {
        uint32_t result = (tmpResultStruct.convValue >> 3U);

        if (tmpResultStruct.commandIdSource == XVALUE_LPADC_USER_CMDID)
        {
            ADC_Valx = result;
        }
        else if (tmpResultStruct.commandIdSource == YVALUE_LPADC_USER_CMDID)
        {
            ADC_Valy = result;
        }
        else if (tmpResultStruct.commandIdSource == LEFT_X_LPADC_USER_CMDID)
        {
            ADC_Val_LeftX = result;
        }
    }
    g_LpadcConversionCompletedFlag = true;
    SDK_ISR_EXIT_BARRIER;
}

void DEMO_LPADC_IRQ_HANDLER_FUNC(void)
{
    LPADC_IRQHandler_Func();
}

/*******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    /* 1. Hardware Init */
    BOARD_InitHardware();
    /* --------------------------------------------------------------- */

    PRINTF("Remote Control Start\r\n");

    /* 2. Initialize SPI Driver (Comms) */
    ESP_SPI_Init(REMOTE_LPSPI_BASE, LPSPI_MASTER_CLK_FREQ, REMOTE_LPSPI_PCS);
    EnableIRQ(REMOTE_LPSPI_IRQN);

    /* 3. Initialize LPADC */
    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.powerLevelMode = kLPADC_PowerLevelAlt4;
    mLpadcConfigStruct.referenceVoltageSource = DEMO_LPADC_VREF_SOURCE;
    mLpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage128;
    LPADC_Init(DEMO_LPADC_BASE, &mLpadcConfigStruct);
    LPADC_DoOffsetCalibration(DEMO_LPADC_BASE);
    LPADC_DoAutoCalibration(DEMO_LPADC_BASE);

    /* ADC Commands */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = JOYSTICK_LPADC_USER_CHANNEL;
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
    mLpadcCommandConfigStruct.chainedNextCommandNumber = YVALUE_LPADC_USER_CMDID;
    LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, XVALUE_LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    mLpadcCommandConfigStruct.channelNumber = JOYSTICK_LPADC_USER_CHANNEL;
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelSingleEndSideB;
    mLpadcCommandConfigStruct.chainedNextCommandNumber = LEFT_X_LPADC_USER_CMDID;
    LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, YVALUE_LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    mLpadcCommandConfigStruct.channelNumber = LEFT_JOY_CHANNEL;
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 0U;
    LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, LEFT_X_LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = XVALUE_LPADC_USER_CMDID;
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(DEMO_LPADC_BASE, 0U, &mLpadcTriggerConfigStruct);

    LPADC_EnableInterrupts(DEMO_LPADC_BASE, kLPADC_FIFO0WatermarkInterruptEnable);
    EnableIRQ(DEMO_LPADC_IRQn);

    RemoteCommand_t *cmd = (RemoteCommand_t *)txBuffer;
    int ui_refresh_div = 0;

    /* 4. LVGL Init */
    lv_init();
    lv_port_disp_init();

    /* [FIX] Use the Professional GUI Init we created */
    RobotGUI_Init();

    while (1)
    {
        /* A. Trigger ADC */
        LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U);
        while (!g_LpadcConversionCompletedFlag) {}
        g_LpadcConversionCompletedFlag = false;

        /* B. Process Data */
        cmd->vy  = MapJoystickToSpeed(ADC_Valy, MAX_LINEAR_SPEED, false);
        cmd->vx  = MapJoystickToSpeed(ADC_Valx, MAX_LINEAR_SPEED, false);
        cmd->phi = MapJoystickToSpeed(ADC_Val_LeftX, MAX_ANGULAR_SPEED, false);

        /* C. Prepare Packet */
        cmd->header = (0xC5 << 24) | (packet_count & 0xFFFFFF);
        cmd->buttons = 0;
        cmd->timestamp = packet_count;

        /* D. Send via SPI */
        if (ESP_SPI_IsTransferCompleted())
        {
            ESP_SPI_StartTransfer(txBuffer, rxBuffer);
            packet_count++;
        }

        // PRINTF("Sent -> VX: %.2f VY: %.2f Phi: %.2f\r\n", cmd->vx, cmd->vy, cmd->phi);

        /* E. Update GUI (Throttled to ~20Hz) */
        /* [FIX] This logic MUST be inside the while loop */
        if (ui_refresh_div++ >= 10)
        {
            RobotGUI_Update(cmd->vx, cmd->vy, cmd->phi);
            ui_refresh_div = 0;
        }

        /* F. LVGL Tasks */
        lv_task_handler();
        lv_tick_inc(5);

        /* G. Loop Delay */
        SDK_DelayAtLeastUs(5000, SystemCoreClock); // 5ms
    }
}
