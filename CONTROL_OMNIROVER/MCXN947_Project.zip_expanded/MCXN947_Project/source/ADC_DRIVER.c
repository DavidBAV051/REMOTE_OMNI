/*
 * ADC_DRIVER.c
 *
 * Created on: Nov 22, 2025
 * Author: diego
 */

#include "ADC_DRIVER.h"

// Define voltage reference if not defined elsewhere
#ifndef EX_LPADC_VREF_VOLTAGE
#define EX_LPADC_VREF_VOLTAGE (3.3f)
#endif

const uint32_t g_LpadcFullRange   = 4096U;
const uint32_t g_LpadcResultShift = 3U;

// Flag to ensure global initialization happens only once
static bool g_isAdcInitialized = false;

void init_ADC(ADC_Type * adc_base, SPC_Type * spc_base, VREF_Type * vref_base, uint32_t user_channel, uint32_t user_cmdid){

    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    // 1. GLOBAL INITIALIZATION (Run Only Once)
    if (!g_isAdcInitialized)
    {
        /* attach FRO HF to ADC0 */
        if(adc_base == ADC0){
            CLOCK_SetClkDiv(kCLOCK_DivAdc0Clk, 1U);
            CLOCK_AttachClk(kFRO_HF_to_ADC0);
        } else if (adc_base == ADC1){
            CLOCK_SetClkDiv(kCLOCK_DivAdc1Clk, 1U);
            CLOCK_AttachClk(kFRO_HF_to_ADC1);
        } else {
            PRINTF("INVALID ADC BASE \r\n");
            return;
        }

        /* enable VREF - Assumed handled in init_hardware via SPC/VREF calls */

        LPADC_GetDefaultConfig(&mLpadcConfigStruct);
        mLpadcConfigStruct.enableAnalogPreliminary = true;
        // Ensure this matches your board's VREF setup.
        // If using internal VREF initialized in main, Alt3 or Alt4 is common.
        mLpadcConfigStruct.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
        mLpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage128;

        LPADC_Init(adc_base, &mLpadcConfigStruct);

        // Calibration takes time, do it only once
        LPADC_DoOffsetCalibration(adc_base);
        LPADC_DoAutoCalibration(adc_base);

        g_isAdcInitialized = true;
    }

    // 2. CHANNEL/COMMAND CONFIGURATION (Run per Motor)
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = user_channel;
    // Assume Single End Side A as per your pin mux (A2, A8, A9, A10)
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
    LPADC_SetConvCommandConfig(adc_base, user_cmdid, &mLpadcCommandConfigStruct);

    // 3. TRIGGER CONFIGURATION (Run per Motor - Unique Trigger per Command)
    // We map user_cmdid 1 -> Trigger 0, cmdid 2 -> Trigger 1, etc.
    // This requires user_cmdid to be 1, 2, 3, 4.
    uint32_t triggerId = user_cmdid - 1U;

    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId       = user_cmdid;
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;

    // Configure the specific trigger ID for this motor
    LPADC_SetConvTriggerConfig(adc_base, triggerId, &mLpadcTriggerConfigStruct);
}

uint32_t read_ADC(ADC_Type * adc_base, uint32_t user_cmdid)
{
    lpadc_conv_result_t mLpadcResultConfigStruct;

    // Calculate trigger mask based on cmdid.
    // cmdid 1 -> Trigger 0 -> Mask 1 (1<<0)
    // cmdid 2 -> Trigger 1 -> Mask 2 (1<<1)
    uint32_t triggerId = user_cmdid - 1U;
    uint32_t triggerMask = (1U << triggerId);

    // Trigger the specific conversion
    LPADC_DoSoftwareTrigger(adc_base, triggerMask);

    // Wait for result. Note: 0U here is the FIFO index (usually FIFO0)
    while (!LPADC_GetConvResult(adc_base, &mLpadcResultConfigStruct, 0U))
    {
    }

    // Optional: Check if the result matches the requested command ID
    // if (mLpadcResultConfigStruct.commandIdSource != user_cmdid) { ... }

    uint32_t adcRaw = (mLpadcResultConfigStruct.convValue >> g_LpadcResultShift);
    return adcRaw;
}
