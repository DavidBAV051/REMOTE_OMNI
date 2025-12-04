/*
 * GPIO_DRIVER.h
 *
 *  Created on: Sep 29, 2025
 *      Author: diego
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#define ARM_DRIVER_VERSION_MAJOR_MINOR(major,minor) (((major) << 8) | (minor))


/**
\brief Driver Version
*/
typedef struct _ARM_DRIVER_VERSION {
  uint16_t api;                         ///< API version
  uint16_t drv;                         ///< Driver version
} ARM_DRIVER_VERSION;

/* General return codes */
#define ARM_DRIVER_OK                 0 ///< Operation succeeded
#define ARM_DRIVER_ERROR             -1 ///< Unspecified error
#define ARM_DRIVER_ERROR_BUSY        -2 ///< Driver is busy
#define ARM_DRIVER_ERROR_TIMEOUT     -3 ///< Timeout occurred
#define ARM_DRIVER_ERROR_UNSUPPORTED -4 ///< Operation not supported
#define ARM_DRIVER_ERROR_PARAMETER   -5 ///< Parameter error
#define ARM_DRIVER_ERROR_SPECIFIC    -6 ///< Start of driver specific errors

/**
\brief General power states
*/
typedef enum _ARM_POWER_STATE {
  ARM_POWER_OFF,                        ///< Power off: no operation possible
  ARM_POWER_LOW,                        ///< Low Power mode: retain state, detect and signal wake-up events
  ARM_POWER_FULL                        ///< Power on: full operation at maximum performance
} ARM_POWER_STATE;


/**
\brief GPIO Pin
*/
typedef uint32_t ARM_GPIO_Pin_t;

/**
\brief GPIO Direction
*/
typedef enum {
  ARM_GPIO_INPUT,                       ///< Input (default)
  ARM_GPIO_OUTPUT                       ///< Output
} ARM_GPIO_DIRECTION;

/**
\brief GPIO Output Mode
*/
typedef enum {
  ARM_GPIO_PUSH_PULL,                   ///< Push-pull (default)
  ARM_GPIO_OPEN_DRAIN                   ///< Open-drain
} ARM_GPIO_OUTPUT_MODE;

/**
\brief GPIO Pull Resistor
*/
typedef enum {
  ARM_GPIO_PULL_NONE,                   ///< None (default)
  ARM_GPIO_PULL_UP,                     ///< Pull-up
  ARM_GPIO_PULL_DOWN                    ///< Pull-down
} ARM_GPIO_PULL_RESISTOR;

/**
\brief GPIO Event Trigger
*/
typedef enum {
  ARM_GPIO_TRIGGER_NONE,                ///< None (default)
  ARM_GPIO_TRIGGER_RISING_EDGE,         ///< Rising-edge
  ARM_GPIO_TRIGGER_FALLING_EDGE,        ///< Falling-edge
  ARM_GPIO_TRIGGER_EITHER_EDGE          ///< Either edge (rising and falling)
} ARM_GPIO_EVENT_TRIGGER;


/****** GPIO Event *****/
#define ARM_GPIO_EVENT_RISING_EDGE      (1UL << 0)  ///< Rising-edge detected
#define ARM_GPIO_EVENT_FALLING_EDGE     (1UL << 1)  ///< Falling-edge detected
#define ARM_GPIO_EVENT_EITHER_EDGE      (1UL << 2)  ///< Either edge detected (only when hardware cannot distinguish between rising and falling edge)


/****** GPIO specific error codes *****/
#define ARM_GPIO_ERROR_PIN              (ARM_DRIVER_ERROR_SPECIFIC - 1) ///< Specified Pin not available

typedef void (*ARM_GPIO_SignalEvent_t) (void);  /* Pointer to \ref ARM_GPIO_SignalEvent : Signal GPIO Event */


typedef struct _gpio_cmsis_config
{
    GPIO_Type *gpio_base;
    PORT_Type *port_base;
} gpio_cmsis_config_t;

typedef struct _gpio_cmsis_handle
{
    gpio_cmsis_config_t const *config;
    ARM_GPIO_SignalEvent_t cb_event;
} gpio_cmsis_handle_t;

typedef void (*gpio_isr_t)(gpio_cmsis_handle_t *gpioHandle);


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void CMSIS_GPIO_SetOutput(gpio_cmsis_handle_t *handle, ARM_GPIO_Pin_t pin, uint32_t val);
int32_t CMSIS_GPIO_SetDirection(gpio_cmsis_handle_t *handle, ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction);
int32_t CMSIS_GPIO_Setup(gpio_cmsis_handle_t *handle, ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);
//PORT specific functions
//PORT0
void PORT0_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val);
int32_t PORT0_SetDirection(ARM_GPIO_Pin_t pin, uint32_t direction);
int32_t PORT0_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);
int32_t PORT0_SetEventTrigger(ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger);

//PORT1
void PORT1_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val);
int32_t PORT1_SetDirection(ARM_GPIO_Pin_t pin, uint32_t direction);
int32_t PORT1_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);

//PORT2
void PORT2_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val);
int32_t PORT2_SetDirection(ARM_GPIO_Pin_t pin, uint32_t direction);
int32_t PORT2_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);

//PORT3
void PORT3_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val);
int32_t PORT3_SetDirection(ARM_GPIO_Pin_t pin, uint32_t direction);
int32_t PORT3_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);

//PORT4
void PORT4_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val);
int32_t PORT4_SetDirection(ARM_GPIO_Pin_t pin, uint32_t direction);
int32_t PORT4_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);

void clear_interrupt_flag(GPIO_Type* gpio_base);
void init_interrupts(GPIO_Type* gpio_base, uint32_t* pins, uint32_t number_of_pins, void* handler_ptr);
uint32_t get_interrupt_flag(GPIO_Type* gpio_base);
#endif /* GPIO_DRIVER_H_ */
