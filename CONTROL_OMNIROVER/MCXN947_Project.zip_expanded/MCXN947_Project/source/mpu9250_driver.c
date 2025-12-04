/*
 * mpu9250_driver.c
 *
 *  Created on: 3 dic. 2025
 *      Author: davidbav
 */
#include "mpu9250_driver.h"

/* Registros Internos */
#define WHO_AM_I_REG        0x75
#define PWR_MGMT_1_REG      0x6B
#define CONFIG_REG          0x1A
#define ACCEL_CONFIG_2_REG  0x1D
#define ACCEL_XOUT_H        0x3B
#define CALIB_SAMPLE        1000U

/* --- Funciones Privadas (Helpers) --- */

static status_t MPU_WriteReg(LPI2C_Type *base, uint8_t reg, uint8_t value)
{
    lpi2c_master_transfer_t masterXfer = {0};
    uint8_t data[2] = {reg, value};

    masterXfer.slaveAddress = MPU9250_ADDR;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.data = data;
    masterXfer.dataSize = 2;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(base, &masterXfer);
}

static status_t MPU_ReadRegs(LPI2C_Type *base, uint8_t reg, uint8_t *data, uint8_t length)
{
    lpi2c_master_transfer_t masterXfer = {0};

    masterXfer.slaveAddress = MPU9250_ADDR;
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = reg;
    masterXfer.subaddressSize = 1;
    masterXfer.data = data;
    masterXfer.dataSize = length;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(base, &masterXfer);
}

/* --- Funciones Públicas --- */

status_t MPU9250_Init(mpu9250_handle_t *handle, LPI2C_Type *base)
{
    status_t status;
    uint8_t whoami = 0;

    /* Guardar referencia del periférico I2C */
    handle->i2cBase = base;

    /* Inicializar offsets en 0 */
    for(int i=0; i<3; i++){
        handle->accelOffset[i] = 0;
        handle->gyroOffset[i] = 0;
    }

    /* Resetear el MPU */
    status = MPU_WriteReg(handle->i2cBase, PWR_MGMT_1_REG, 0x80);
    if (status != kStatus_Success) return status;
    SDK_DelayAtLeastUs(100000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    /* Despertar y seleccionar reloj (Auto select) */
    MPU_WriteReg(handle->i2cBase, PWR_MGMT_1_REG, 0x01);
    SDK_DelayAtLeastUs(100000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    /* Configurar DLPF (~41Hz) */
    MPU_WriteReg(handle->i2cBase, CONFIG_REG, 0x03);
    MPU_WriteReg(handle->i2cBase, ACCEL_CONFIG_2_REG, 0x03);

    /* Verificar ID */
    MPU_ReadRegs(handle->i2cBase, WHO_AM_I_REG, &whoami, 1);

    if (whoami != 0x71 && whoami != 0x73) {
        return kStatus_Fail; // ID Incorrecto
    }

    return kStatus_Success;
}

void MPU9250_Calibrate(mpu9250_handle_t *handle)
{
    uint8_t sensorData[14];
    int16_t tempRaw[6]; // ax, ay, az, gx, gy, gz temporales
    int32_t sum[6] = {0}; // Acumuladores

    /* Muestreo */
    for (int i = 0; i < CALIB_SAMPLE; i++) {
        MPU_ReadRegs(handle->i2cBase, ACCEL_XOUT_H, sensorData, 14);

        // Accel
        tempRaw[0] = (int16_t)((sensorData[0] << 8) | sensorData[1]); // AX
        tempRaw[1] = (int16_t)((sensorData[2] << 8) | sensorData[3]); // AY
        tempRaw[2] = (int16_t)((sensorData[4] << 8) | sensorData[5]); // AZ
        // Gyro
        tempRaw[3] = (int16_t)((sensorData[8] << 8) | sensorData[9]);  // GX
        tempRaw[4] = (int16_t)((sensorData[10] << 8) | sensorData[11]); // GY
        tempRaw[5] = (int16_t)((sensorData[12] << 8) | sensorData[13]); // GZ

        for(int j=0; j<6; j++) {
            sum[j] += tempRaw[j];
        }

        SDK_DelayAtLeastUs(1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }

    /* Cálculo de Promedios */
    handle->accelOffset[0] = sum[0] / CALIB_SAMPLE;
    handle->accelOffset[1] = sum[1] / CALIB_SAMPLE;
    handle->accelOffset[2] = sum[2] / CALIB_SAMPLE;

    handle->gyroOffset[0] = sum[3] / CALIB_SAMPLE;
    handle->gyroOffset[1] = sum[4] / CALIB_SAMPLE;
    handle->gyroOffset[2] = sum[5] / CALIB_SAMPLE;

    /* Corrección de gravedad en Z (El sensor debe estar plano) */
    handle->accelOffset[2] -= MPU9250_ACCEL_1G;
}

status_t MPU9250_ReadSensor(mpu9250_handle_t *handle)
{
    uint8_t sensorData[14];
    status_t status;

    status = MPU_ReadRegs(handle->i2cBase, ACCEL_XOUT_H, sensorData, 14);
    if(status != kStatus_Success) return status;

    /* Conversión y Resta de Offsets */
    // Acelerómetro
    handle->accelRaw[0] = ((int16_t)((sensorData[0] << 8) | sensorData[1])) - handle->accelOffset[0];
    handle->accelRaw[1] = ((int16_t)((sensorData[2] << 8) | sensorData[3])) - handle->accelOffset[1];
    handle->accelRaw[2] = ((int16_t)((sensorData[4] << 8) | sensorData[5])) - handle->accelOffset[2];

    // Temperatura
    handle->tempRaw = (int16_t)((sensorData[6] << 8) | sensorData[7]);

    // Giroscopio
    handle->gyroRaw[0] = ((int16_t)((sensorData[8] << 8) | sensorData[9])) - handle->gyroOffset[0];
    handle->gyroRaw[1] = ((int16_t)((sensorData[10] << 8) | sensorData[11])) - handle->gyroOffset[1];
    handle->gyroRaw[2] = ((int16_t)((sensorData[12] << 8) | sensorData[13])) - handle->gyroOffset[2];

    return kStatus_Success;
}

