/*
 * PWM_DRIVER.h
 *
 *  Created on: Nov 21, 2025
 *      Author: diego
 */

#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_

#include "app.h"
#include "fsl_pwm.h"

#define DEMO_PWM_DISABLE_MAP_OP

int init_pwm(void);
void PWM_DRV_Init3PhPwm(void);

#endif /* PWM_DRIVER_H_ */
