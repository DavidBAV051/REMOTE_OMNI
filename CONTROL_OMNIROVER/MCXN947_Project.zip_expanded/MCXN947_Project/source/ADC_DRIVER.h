/*
 * ADC_DRIVER.h
 *
 *  Created on: Nov 22, 2025
 *      Author: diego
 */

#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include "fsl_lpadc.h"
#include "fsl_debug_console.h"

void init_ADC(ADC_Type * adc_base, SPC_Type * spc_base, VREF_Type * vref_base, uint32_t user_channel, uint32_t user_cmdid);
uint32_t read_ADC(ADC_Type * adc_base, uint32_t user_cmdid);

#endif /* ADC_DRIVER_H_ */
