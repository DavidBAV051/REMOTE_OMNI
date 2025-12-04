/*
 * ESP_SPI.c
 *
 * Abstracted SPI Master Driver for MCXN947
 * Handles 8MHz SPI communication with ESP32 slave
 * Independent of board-specific app.h
 */

#include "ESP_SPI.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Driver State */
static LPSPI_Type *g_espSpiBase = NULL;
static uint8_t *g_masterRxData = NULL;
static uint8_t *g_masterTxData = NULL;

static volatile uint32_t g_masterTxCount = 0;
static volatile uint32_t g_masterRxCount = 0;
static volatile uint8_t g_masterRxWatermark = 0;
static volatile uint8_t g_masterFifoSize = 0;
static volatile bool g_isMasterTransferCompleted = true;

/*******************************************************************************
 * Code
 ******************************************************************************/

void ESP_SPI_Init(LPSPI_Type *base, uint32_t srcClock_Hz, lpspi_which_pcs_t whichPcs)
{
    lpspi_master_config_t masterConfig;

    g_espSpiBase = base;

    /* Master config */
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = ESP_SPI_BAUDRATE;
    masterConfig.whichPcs = whichPcs;
    masterConfig.pcsToSckDelayInNanoSec        = 1000000000U / (masterConfig.baudRate * 2U);
    masterConfig.lastSckToPcsDelayInNanoSec    = 1000000000U / (masterConfig.baudRate * 2U);
    masterConfig.betweenTransferDelayInNanoSec = 1000000000U / (masterConfig.baudRate * 2U);

    LPSPI_MasterInit(g_espSpiBase, &masterConfig, srcClock_Hz);

    /* The TX and RX FIFO sizes are always the same. */
    g_masterFifoSize = LPSPI_GetRxFifoSize(g_espSpiBase);

    /* Set the RX and TX watermarks to reduce the ISR times. */
    uint8_t txWatermark;
    if (g_masterFifoSize > 1)
    {
        txWatermark         = 1;
        g_masterRxWatermark = g_masterFifoSize - 2;
    }
    else
    {
        txWatermark         = 0;
        g_masterRxWatermark = 0;
    }

    LPSPI_SetFifoWatermarks(g_espSpiBase, txWatermark, g_masterRxWatermark);

    /* Enable/Disable toggling to apply specific configs if needed */
    LPSPI_Enable(g_espSpiBase, false);
    g_espSpiBase->CFGR1 &= (~LPSPI_CFGR1_NOSTALL_MASK);
    LPSPI_Enable(g_espSpiBase, true);

    /* Flush FIFO, clear status, disable all the interrupts. */
    LPSPI_FlushFifo(g_espSpiBase, true, true);
    LPSPI_ClearStatusFlags(g_espSpiBase, kLPSPI_AllStatusFlag);
    LPSPI_DisableInterrupts(g_espSpiBase, kLPSPI_AllInterruptEnable);

    /* Select PCS and set Continuous mode */
    LPSPI_SelectTransferPCS(g_espSpiBase, whichPcs);
    LPSPI_SetPCSContinous(g_espSpiBase, true);

    /* Note: NVIC Enable (EnableIRQ) must be handled by the application or upper layer
       as it requires the specific IRQn_Type which depends on the instance used. */
}

void ESP_SPI_StartTransfer(uint8_t *txData, uint8_t *rxData)
{
    /* Wait for previous transfer to complete if called prematurely */
    while (!g_isMasterTransferCompleted) {}

    g_masterTxData = txData;
    g_masterRxData = rxData;
    g_masterTxCount = 0;
    g_masterRxCount = 0;
    g_isMasterTransferCompleted = false;

    /* Flush FIFOs before starting */
    LPSPI_FlushFifo(g_espSpiBase, true, true);
    LPSPI_ClearStatusFlags(g_espSpiBase, kLPSPI_AllStatusFlag);

    /* Ensure continuous mode is set for the start of transfer */
    LPSPI_SetPCSContinous(g_espSpiBase, true);

    /* Fill up the TX data in FIFO initially */
    while ((LPSPI_GetTxFifoCount(g_espSpiBase) < g_masterFifoSize) &&
           (g_masterTxCount - g_masterRxCount < g_masterFifoSize))
    {
        /* Write the word to TX register */
        LPSPI_WriteData(g_espSpiBase, g_masterTxData[g_masterTxCount]);
        ++g_masterTxCount;

        if (g_masterTxCount == ESP_SPI_TRANSFER_SIZE)
        {
            /* Wait for room in FIFO for TCR write if needed (though we just write data here) */
            while (LPSPI_GetTxFifoCount(g_espSpiBase) == g_masterFifoSize)
            {
            }
            /* Set the PCS back to discontinuous to finish the transfer if all tx data pushed. */
            LPSPI_SetPCSContinous(g_espSpiBase, false);
            break;
        }
    }

    /* Enable RX Interrupt to handle the rest of the transfer in ISR */
    LPSPI_EnableInterrupts(g_espSpiBase, kLPSPI_RxInterruptEnable);
}

bool ESP_SPI_IsTransferCompleted(void)
{
    return g_isMasterTransferCompleted;
}

void ESP_SPI_MasterIRQHandler(void)
{
    if (g_espSpiBase == NULL)
    {
        return;
    }

    /* Reading RX FIFO */
    LPSPI_DisableInterrupts(g_espSpiBase, kLPSPI_RxInterruptEnable);
    if (g_masterRxCount < ESP_SPI_TRANSFER_SIZE)
    {
        while (LPSPI_GetRxFifoCount(g_espSpiBase))
        {
            /* Read out the data. */
            g_masterRxData[g_masterRxCount] = LPSPI_ReadData(g_espSpiBase);
            g_masterRxCount++;

            if (g_masterRxCount == ESP_SPI_TRANSFER_SIZE)
            {
                break;
            }
        }
    }

    /* Update rxWatermark dynamically */
    if ((ESP_SPI_TRANSFER_SIZE - g_masterRxCount) <= g_masterRxWatermark)
    {
        g_espSpiBase->FCR =
            (g_espSpiBase->FCR & (~LPSPI_FCR_RXWATER_MASK)) |
            LPSPI_FCR_RXWATER(((ESP_SPI_TRANSFER_SIZE - g_masterRxCount) > 1U) ?
                              ((ESP_SPI_TRANSFER_SIZE - g_masterRxCount) - 1U) : (0U));
    }

    /* Writing TX FIFO */
    if (g_masterTxCount < ESP_SPI_TRANSFER_SIZE)
    {
        while ((LPSPI_GetTxFifoCount(g_espSpiBase) < g_masterFifoSize) &&
               (g_masterTxCount - g_masterRxCount < g_masterFifoSize))
        {
            LPSPI_WriteData(g_espSpiBase, g_masterTxData[g_masterTxCount]);
            ++g_masterTxCount;

            if (g_masterTxCount == ESP_SPI_TRANSFER_SIZE)
            {
                while ((uint32_t)g_masterFifoSize == LPSPI_GetTxFifoCount(g_espSpiBase))
                {
                }
                /* End of transfer signal */
                LPSPI_SetPCSContinous(g_espSpiBase, false);
                break;
            }
        }
    }

    /* Check completion */
    if ((g_masterTxCount == ESP_SPI_TRANSFER_SIZE) && (g_masterRxCount == ESP_SPI_TRANSFER_SIZE))
    {
        g_isMasterTransferCompleted = true;
    }

    /* Re-enable interrupts if not finished */
    if (g_masterRxCount < ESP_SPI_TRANSFER_SIZE)
    {
        LPSPI_EnableInterrupts(g_espSpiBase, kLPSPI_RxInterruptEnable);
    }
    SDK_ISR_EXIT_BARRIER;
}

void PrepareTxBuffer(uint8_t *txBuffer, uint32_t packetCount)
{
    uint32_t i;

    for (i = 0; i < ESP_SPI_TRANSFER_SIZE; i++)
    {
        /* Bytes 0-3: 32-bit Packet Counter (Big Endian) */
        if (i < 4)
        {
            txBuffer[i] = (packetCount >> (8 * (3 - i))) & 0xFF;
        }
        /* Bytes 4-19: Payload Data (Incrementing pattern for testing) */
        else
        {
            txBuffer[i] = i % 256;
        }
    }
}
