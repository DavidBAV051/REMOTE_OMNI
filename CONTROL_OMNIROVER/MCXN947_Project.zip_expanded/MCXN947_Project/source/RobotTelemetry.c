/* source/RobotTelemetry.c */
#include "RobotTelemetry.h"
#include "ESP_SPI.h"
#include "omnidriver.h"
#include "ADC_DRIVER.h"

/* External references to your Global Objects */
extern MOTOR_T M1, M2, M3, M4;
extern ROBOT_T ROBOT; // [NEW] Access the global ROBOT structure

/* Buffers for SPI Driver */
static uint8_t telemetryTxBuffer[ESP_SPI_TRANSFER_SIZE];
static uint8_t telemetryRxBuffer[ESP_SPI_TRANSFER_SIZE];

static uint32_t packet_counter = 0;
static uint32_t software_tick = 0;

void Robot_SendTelemetry(void)
{
    /* Increment internal timebase */
    software_tick++;

    /* 1. Check if SPI is busy. If so, skip this cycle. */
    if (!ESP_SPI_IsTransferCompleted())
    {
        return;
    }

    /* -----------------------------------------------------------
     * STEP A: PROCESS RECEIVED DATA (From Previous Transaction)
     * ----------------------------------------------------------- */
    RemoteCommand_t *rx_cmd = (RemoteCommand_t *)telemetryRxBuffer;

    /* Check for the specific Header Byte (0xC5) to verify data integrity */
    if ((rx_cmd->header >> 24) == 0xC5)
    {
        /* Update Robot Velocities directly */
        ROBOT.vx  = rx_cmd->vx;
        ROBOT.vy  = rx_cmd->vy;
        ROBOT.phi = rx_cmd->phi;

        /* Optional: Handle buttons here */
        // if (rx_cmd->buttons & 0x01) { ... }
    }
    else
    {
        /* Optional: Safety logic if no valid packet received for X cycles */
        // ROBOT.vx = 0; ROBOT.vy = 0; ROBOT.phi = 0;
    }

    /* -----------------------------------------------------------
     * STEP B: PREPARE NEW TELEMETRY PACKET
     * ----------------------------------------------------------- */
    RobotTelemetry_t *packet = (RobotTelemetry_t *)telemetryTxBuffer;

    /* Header Construction */
    packet->packet_header = (TELEMETRY_PACKET_ID << 24) | (packet_counter & 0x00FFFFFF);

    /* Fill Speed Data */
    packet->speed_m1 = M1.speed;
    packet->speed_m2 = M2.speed;
    packet->speed_m3 = M3.speed;
    packet->speed_m4 = M4.speed;

    /* Fill ADC Data (Access via Motor Structs) */
    if (M1.ADC != NULL) {
        packet->adc_m1 = (uint16_t)read_ADC(M1.ADC->adc_base, M1.ADC->commandId);
        M1.ADC->last_raw_value = packet->adc_m1;
    } else { packet->adc_m1 = 0; }

    if (M2.ADC != NULL) {
        packet->adc_m2 = (uint16_t)read_ADC(M2.ADC->adc_base, M2.ADC->commandId);
        M2.ADC->last_raw_value = packet->adc_m2;
    } else { packet->adc_m2 = 0; }

    if (M3.ADC != NULL) {
        packet->adc_m3 = (uint16_t)read_ADC(M3.ADC->adc_base, M3.ADC->commandId);
        M3.ADC->last_raw_value = packet->adc_m3;
    } else { packet->adc_m3 = 0; }

    if (M4.ADC != NULL) {
        packet->adc_m4 = (uint16_t)read_ADC(M4.ADC->adc_base, M4.ADC->commandId);
        M4.ADC->last_raw_value = packet->adc_m4;
    } else { packet->adc_m4 = 0; }

    /* Fill Timestamp/Flags */
    packet->timestamp = software_tick;
    packet->reserved_flags = 0;

    /* -----------------------------------------------------------
     * STEP C: START SPI TRANSFER
     * ----------------------------------------------------------- */
    /* This sends 'telemetryTxBuffer' AND receives into 'telemetryRxBuffer' simultaneously */
    ESP_SPI_StartTransfer(telemetryTxBuffer, telemetryRxBuffer);

    packet_counter++;
}
