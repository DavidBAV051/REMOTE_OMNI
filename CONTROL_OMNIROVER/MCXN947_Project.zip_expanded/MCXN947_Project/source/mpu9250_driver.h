/*
 * mpu9250_driver.h
 *
 *  Created on: 3 dic. 2025
 *      Author: davidbav
 */

#ifndef MPU9250_DRIVER_H_
#define MPU9250_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsl_common.h"
#include "fsl_lpi2c.h"

#define EXAMPLE_I2C_MASTER_BASE (LPI2C7_BASE)

/* Configuraciones Físicas */
#define MPU9250_ADDR            0x68
#define MPU9250_ACCEL_1G        16384U
//#define I2C_BAUDRATE        100000U // 400k es mejor si tu hardware lo soporta, pero 100k es seguro
#define LPI2C_MASTER_BASE   ((LPI2C_Type *)EXAMPLE_I2C_MASTER_BASE)
#define CALIB_SAMPLE		1000U

/* Estructura principal del Driver (Handle) */
typedef struct {
    /* Configuración Hardware */
    LPI2C_Type *i2cBase;        // Puntero a la base del periférico (LPI2C0, LPI2C1, etc.)

    /* Datos Raw (Crudos con Offset aplicado) */
    int16_t accelRaw[3];        // [0]=X, [1]=Y, [2]=Z
    int16_t gyroRaw[3];         // [0]=X, [1]=Y, [2]=Z
    int16_t tempRaw;

    /* Offsets de Calibración */
    int32_t accelOffset[3];
    int32_t gyroOffset[3];

} mpu9250_handle_t;

/* Prototipos de Funciones */

/* Inicializa el sensor, configura DLPF y verifica WHO_AM_I */
status_t MPU9250_Init(mpu9250_handle_t *handle, LPI2C_Type *base);

/* Realiza la rutina de calibración (Bloqueante) */
void MPU9250_Calibrate(mpu9250_handle_t *handle);

/* Lee los registros, convierte a int16 y aplica los offsets guardados en el handle */
status_t MPU9250_ReadSensor(mpu9250_handle_t *handle);

#endif /* MPU9250_DRIVER_H_ */
