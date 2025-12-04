/*
 * TIMER_DRIVER.c
 *
 *  Created on: Nov 22, 2025
 *      Author: diego
 */

#include "TIMER_DRIVER.h"

void (*callback_lptmr0)(void* args);
void (*callback_lptmr1)(void* args);

void init_LPTMR_12MHz(LPTMR_Type* lptmr_base, uint32_t period_ticks){

    lptmr_config_t lptmrConfig;

    /*Enable 12MHz clock*/
    CLOCK_SetupClockCtrl(kCLOCK_FRO12MHZ_ENA);

    lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
    lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
    lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
    lptmrConfig.enableFreeRunning = false;
    lptmrConfig.bypassPrescaler = true;
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_0;
    lptmrConfig.value = kLPTMR_Prescale_Glitch_0;

    if( !(lptmr_base == LPTMR0 || lptmr_base == LPTMR1) ){
		PRINTF("Not a valid lptmr base: %p", lptmr_base );
		return;
	}

    // Initialize the LPTMR
    LPTMR_Init(lptmr_base, &lptmrConfig);



    // Set timer period.
    LPTMR_SetTimerPeriod(lptmr_base, period_ticks);


}

void LPTMR0_IRQHandler(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);

    if(callback_lptmr0 != NULL){
    	callback_lptmr0(NULL);
    }
    __DSB();
    __ISB();
}

void LPTMR1_IRQHandler(void)
{
    LPTMR_ClearStatusFlags(LPTMR1, kLPTMR_TimerCompareFlag);

    if(callback_lptmr1 != NULL){
    	callback_lptmr1(NULL);
    }

    __DSB();
    __ISB();
}


void lptmr_attach_callback(LPTMR_Type* lptmr_base, void* callback){

	// Enable timer interrupt
	LPTMR_EnableInterrupts(lptmr_base, kLPTMR_TimerInterruptEnable);

	// Enable at the NVIC

	if(lptmr_base == LPTMR0){
		EnableIRQ(LPTMR0_IRQn);
	}else if(lptmr_base == LPTMR1){
		EnableIRQ(LPTMR1_IRQn);
	} else {
		PRINTF("Not a valid lptmr base: %p", lptmr_base );
		return;
	}

	if(lptmr_base == LPTMR0){
		callback_lptmr0 = callback;
	} else {
		callback_lptmr1 = callback;
	}



}
