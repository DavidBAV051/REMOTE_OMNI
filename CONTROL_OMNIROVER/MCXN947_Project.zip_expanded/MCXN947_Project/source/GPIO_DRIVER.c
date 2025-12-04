/*
 * GPIO_DRIVER.c
 *
 *  Created on: Sep 29, 2025
 *      Author: diego
 */

#include "GPIO_DRIVER.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
//static gpio_isr_t s_gpioIsr; //Isr type to save pointer to event function
//Pointer for handler assignment
void (*handler)(void);

static const gpio_cmsis_config_t s_gpio_port0_cmsis_config = {
    .gpio_base = GPIO0,
    .port_base = PORT0,
};
static const gpio_cmsis_config_t s_gpio_port1_cmsis_config = {
    .gpio_base = GPIO1,
    .port_base = PORT1,
};

static const gpio_cmsis_config_t s_gpio_port2_cmsis_config = {
    .gpio_base = GPIO2,
    .port_base = PORT2,
};

static const gpio_cmsis_config_t s_gpio_port3_cmsis_config = {
    .gpio_base = GPIO3,
    .port_base = PORT3,
};

static const gpio_cmsis_config_t s_gpio_port4_cmsis_config = {
    .gpio_base = GPIO4,
    .port_base = PORT4,
};

static gpio_cmsis_handle_t s_gpio_port0_handle = {
    .config = &s_gpio_port0_cmsis_config,
};

static gpio_cmsis_handle_t s_gpio_port1_handle = {
    .config = &s_gpio_port1_cmsis_config,
};

static gpio_cmsis_handle_t s_gpio_port2_handle = {
    .config = &s_gpio_port2_cmsis_config,
};

static gpio_cmsis_handle_t s_gpio_port3_handle = {
    .config = &s_gpio_port3_cmsis_config,
};

static gpio_cmsis_handle_t s_gpio_port4_handle = {
    .config = &s_gpio_port4_cmsis_config,
};


/*******************************************************************************
 * Functions
 ******************************************************************************/

void CMSIS_GPIO_SetOutput(gpio_cmsis_handle_t *handle, ARM_GPIO_Pin_t pin, uint32_t val)
{
    GPIO_PinWrite(handle->config->gpio_base, pin, val);
}

int32_t CMSIS_GPIO_SetDirection(gpio_cmsis_handle_t *handle, ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction)
{

    if (direction == ARM_GPIO_INPUT)
    {
        handle->config->gpio_base->PDDR &= GPIO_FIT_REG(~(1UL << pin));
    }
    else
    {
        handle->config->gpio_base->PDDR |= GPIO_FIT_REG((1UL << pin));
    }

    return ARM_DRIVER_OK;
}

int32_t CMSIS_GPIO_Setup(gpio_cmsis_handle_t *handle, ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
    gpio_pin_config_t pinConfig = {
        kGPIO_DigitalInput,
        0,
    };

    if(handle->cb_event == NULL)
    {
        handle->cb_event= cb_event; /* cb_event is CMSIS driver callback. */
    }

    /* Set pin as GPIO input without pull-resistor and event trigger. */
    PORT_SetPinMux(handle->config->port_base, pin, kPORT_MuxAsGpio);
    handle->config->port_base->PCR[pin] = (handle->config->port_base->PCR[pin] & ~PORT_PCR_PE_MASK) | PORT_PCR_PE(0);

    GPIO_PinInit(handle->config->gpio_base, pin, &pinConfig);

    return ARM_DRIVER_OK;
}


int32_t CMSIS_GPIO_SetPullResistor(gpio_cmsis_handle_t *handle, ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor)
{
    switch (resistor)
    {
        case ARM_GPIO_PULL_NONE:
            handle->config->port_base->PCR[pin] = (handle->config->port_base->PCR[pin] & ~PORT_PCR_PE_MASK) | PORT_PCR_PE(0);
            break;
        case ARM_GPIO_PULL_UP:
            handle->config->port_base->PCR[pin] = (handle->config->port_base->PCR[pin] & ~PORT_PCR_PE_MASK) | PORT_PCR_PE(1);
            handle->config->port_base->PCR[pin] = (handle->config->port_base->PCR[pin] & ~PORT_PCR_PS_MASK) | PORT_PCR_PS(1);
            break;
        case ARM_GPIO_PULL_DOWN:
            handle->config->port_base->PCR[pin] = (handle->config->port_base->PCR[pin] & ~PORT_PCR_PE_MASK) | PORT_PCR_PE(1);
            handle->config->port_base->PCR[pin] = (handle->config->port_base->PCR[pin] & ~PORT_PCR_PS_MASK) | PORT_PCR_PS(0);
            break;
        default:
            assert(false);
            break;
    }

    return ARM_DRIVER_OK;
}



int32_t CMSIS_GPIO_SetEventTrigger(gpio_cmsis_handle_t *handle, ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger)
{

    switch (trigger)
    {
        case ARM_GPIO_TRIGGER_NONE:
            GPIO_SetPinInterruptConfig(handle->config->gpio_base, pin, kGPIO_InterruptStatusFlagDisabled);
            break;
        case ARM_GPIO_TRIGGER_RISING_EDGE:
            GPIO_SetPinInterruptConfig(handle->config->gpio_base, pin, kGPIO_InterruptRisingEdge);
            break;
        case ARM_GPIO_TRIGGER_FALLING_EDGE:
            GPIO_SetPinInterruptConfig(handle->config->gpio_base, pin, kGPIO_InterruptFallingEdge);
            break;
        case ARM_GPIO_TRIGGER_EITHER_EDGE:
            GPIO_SetPinInterruptConfig(handle->config->gpio_base, pin, kGPIO_InterruptEitherEdge);
            break;
        default:
            assert(false);
            break;
    }


    return ARM_DRIVER_OK;
}

/*Abstract functions for port*/
//Port 0
void PORT0_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val)
{
	CMSIS_GPIO_SetOutput(&s_gpio_port0_handle, pin, val);
}

int32_t PORT0_SetDirection(ARM_GPIO_Pin_t pin, uint32_t direction)
{
	return CMSIS_GPIO_SetDirection(&s_gpio_port0_handle, pin, direction);
}

int32_t PORT0_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
	return CMSIS_GPIO_Setup(&s_gpio_port0_handle, pin, cb_event);
}

int32_t PORT0_SetPullResistor(ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor)
{
    return CMSIS_GPIO_SetPullResistor(&s_gpio_port0_handle, pin, resistor);
}

int32_t PORT0_SetEventTrigger(ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger)
{

    EnableIRQ(GPIO00_IRQn);
    return CMSIS_GPIO_SetEventTrigger(&s_gpio_port0_handle, pin, trigger);
}



//Port 1
void PORT1_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val)
{
	return CMSIS_GPIO_SetOutput(&s_gpio_port1_handle, pin, val);
}

int32_t PORT1_SetDirection(ARM_GPIO_Pin_t pin, uint32_t direction)
{
	return CMSIS_GPIO_SetDirection(&s_gpio_port1_handle, pin, direction);
}

int32_t PORT1_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
	return CMSIS_GPIO_Setup(&s_gpio_port1_handle, pin, cb_event);
}

//Port 2
void PORT2_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val)
{
	CMSIS_GPIO_SetOutput(&s_gpio_port2_handle, pin, val);
}

int32_t PORT2_SetDirection(ARM_GPIO_Pin_t pin, uint32_t direction)
{
	return CMSIS_GPIO_SetDirection(&s_gpio_port2_handle, pin, direction);
}

int32_t PORT2_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
	return CMSIS_GPIO_Setup(&s_gpio_port2_handle, pin, cb_event);
}

//Port 3
void PORT3_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val)
{
	return CMSIS_GPIO_SetOutput(&s_gpio_port3_handle, pin, val);
}

int32_t PORT3_SetDirection(ARM_GPIO_Pin_t pin, uint32_t direction)
{
	return CMSIS_GPIO_SetDirection(&s_gpio_port3_handle, pin, direction);
}

int32_t PORT3_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
	return CMSIS_GPIO_Setup(&s_gpio_port3_handle, pin, cb_event);
}

//Port 4
void PORT4_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val)
{
	CMSIS_GPIO_SetOutput(&s_gpio_port4_handle, pin, val);
}

int32_t PORT4_SetDirection(ARM_GPIO_Pin_t pin, uint32_t direction)
{
	return CMSIS_GPIO_SetDirection(&s_gpio_port4_handle, pin, direction);
}

int32_t PORT4_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
	return CMSIS_GPIO_Setup(&s_gpio_port4_handle, pin, cb_event);
}

void GPIO10_IRQHandler(void){
    if(handler != NULL){
        handler();
    }
    SDK_ISR_EXIT_BARRIER;
}

void init_interrupts(GPIO_Type* gpio_base, uint32_t* pins, uint32_t number_of_pins, void* handler_ptr){
    // Define the init structure for the input switch pin
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };
    // Enable the callback for push buttons

    if(gpio_base == GPIO0){
        EnableIRQ(GPIO00_IRQn);
        EnableIRQ(GPIO01_IRQn);
    } else if(gpio_base == GPIO1){
        EnableIRQ(GPIO10_IRQn);
        EnableIRQ(GPIO11_IRQn);
    }else if(gpio_base == GPIO2){
        EnableIRQ(GPIO20_IRQn);
        EnableIRQ(GPIO21_IRQn);
    } else if(gpio_base == GPIO3){
        EnableIRQ(GPIO30_IRQn);
        EnableIRQ(GPIO31_IRQn);
    } else if(gpio_base == GPIO4){
        EnableIRQ(GPIO40_IRQn);
        EnableIRQ(GPIO41_IRQn);
    } else if(gpio_base == GPIO5){
        EnableIRQ(GPIO50_IRQn);
        EnableIRQ(GPIO51_IRQn);
    } else {
        return;
    }

    for(int i = 0; i < number_of_pins; i++){
        GPIO_PinInit(gpio_base,  pins[i], &sw_config);
        GPIO_SetPinInterruptConfig(gpio_base,  pins[i], kGPIO_InterruptRisingEdge);
    }

    handler = handler_ptr;
    return;
}

void clear_interrupt_flag(GPIO_Type* gpio_base){
	// Clear interrupt flags
	gpio_base->ISFR[0] = (1UL << 9) | (1UL << 12) | (1UL << 22) | (1UL << 7) | (1UL << 6);
}

uint32_t get_interrupt_flag(GPIO_Type* gpio_base){
	uint32_t raw_button_mask = 0;

	// Read all button states from ISFR (which buttons triggered the interrupt)
	if(gpio_base->ISFR[0] & (1UL << 9))  raw_button_mask |= (1UL << 0); // SW1
	if(gpio_base->ISFR[0] & (1UL << 12)) raw_button_mask |= (1UL << 1); // SW2
	if(gpio_base->ISFR[0] & (1UL << 22)) raw_button_mask |= (1UL << 2); // SW3
	if(gpio_base->ISFR[0] & (1UL << 7))  raw_button_mask |= (1UL << 3); // SW4
	if(gpio_base->ISFR[0] & (1UL << 6))  raw_button_mask |= (1UL << 4); // SW5


	return raw_button_mask;
}
