/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Sep 14, 2026
 *      Author: robbie
 */
#include "stm32f407xx_spi_driver.h"
#include <stdint.h>


void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
        if (pSPIx == SPI1)
        {
            SPI1_PCLK_EN();
        }
        else if (pSPIx == SPI2)
        {
            SPI2_PCLK_EN();
        }
        else if (pSPIx == SPI3)
        {
            SPI3_PCLK_EN();
        }
        else if (pSPIx == SPI4)
        {
            SPI4_PCLK_EN();
        }
    }
    else {
        if (pSPIx == SPI1)
        {
            SPI1_PCLK_DI();
        }
        else if (pSPIx == SPI2)
        {
            SPI2_PCLK_DI();
        }
        else if (pSPIx == SPI3)
        {
            SPI3_PCLK_DI();
        }
        else if (pSPIx == SPI4)
        {
            SPI4_PCLK_DI();
        }
    }
}


void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	uint32_t temp_reg = 0;  // temp var for CR1 register (control register 1)

    // Device Mode
	temp_reg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

    // Bus config
    if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
    {
        // Clear bidirectional mode to 0
        temp_reg &= ~(1 << SPI_CR1_BIDIMODE);
    }
    else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
    {
        temp_reg |= (1 << SPI_CR1_BIDIMODE);
    }
    else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
    {
        temp_reg |= (1 << SPI_CR1_BIDIMODE);
        temp_reg |= (1 << SPI_CR1_RXONLY);
    }

    // SCLK speed (baud rate)
    temp_reg |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR);

	// Data frame format
	temp_reg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	// Clock polarity
	temp_reg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

    // Clock phase
	temp_reg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

    // Software slave management
	temp_reg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = temp_reg; // write temp variable to actual CR1 register
}


void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
    if (pSPIx == SPI1)
    {
        SPI1_REG_RESET();
    }
    // TODO
}


// Returns 1 or 0 basically
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagMask)
{
    if (pSPIx->SR & FlagMask)
    {
        return FLAG_SET;
    }
    return FLAG_RESET;
}


/*********************************************************************
 * @fn      		  - SPI_SendData
 *
 * @brief             -
 *
 * @param[in]         - pSPIx: pointer to base address of SPIx peripheral
 * @param[in]         - pTxBuffer: pointer to buffer with data to send
 * @param[in]         - Len: length of data to send in bytes
 *
 * @return            -
 *
 * @Note              -  This is a blocking call 

 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
    while (Len > 0)
    {
        // Poll until transmit buffer is empty before continuing (TXE is SET)
        while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG_MASK) == FLAG_RESET);

        // Check DFF
        if (pSPIx->CR1 & (1 << SPI_CR1_DFF)) // 16 bit DFF
        {
            // Load data into data register
            pSPIx->DR = *((uint16_t*)pTxBuffer);
            // decrement Len by 2 bytes
            Len = Len - 2;
            // increment buffer pointer by 2 bytes
            pTxBuffer = pTxBuffer + 2;
        }
        else // 8 bit DFF
        {
            pSPIx->DR = *pTxBuffer;
            Len--;
            pTxBuffer++;
        }
    }
}
