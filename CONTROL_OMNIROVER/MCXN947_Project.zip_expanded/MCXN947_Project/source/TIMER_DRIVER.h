/*
 * TIMER_DRIVER.h
 *
 *  Created on: Nov 22, 2025
 *      Author: diego
 */

#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include "fsl_lptmr.h"
#include "fsl_ctimer.h"
#include "fsl_debug_console.h"

#define CLOCK_SOURCE_LPTMR 12000000U

void init_LPTMR_12MHz(LPTMR_Type* lptmr_base, uint32_t period_ticks);
void lptmr_attach_callback(LPTMR_Type* lptmr_base, void* callback);

#endif /* TIMER_DRIVER_H_ */
