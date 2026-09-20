
#include <stdint.h>

/*
Overview: Measuring high speed internal clock (HSI) waveform using oscilloscope

HSI 16 MHz
   │
   │ (RCC_CFGR select MCO1 to use HSI as clock source)
   ▼
MCO1 prescaler /4
   │
   ▼
4 MHz signal
   │
   ▼
PA8 (Set as Alternate Function AF0)
   │
   ▼
Oscilloscope measurement
*/

#define RCC_BASE_ADDR         0x40023800UL
#define RCC_CFGR_OFFSET       0x08UL
#define RCC_AHB1ENR_OFFSET    0x30UL
#define RCC_CFGR_ADDR         (RCC_BASE_ADDR + RCC_CFGR_OFFSET)
#define RCC_AHB1ENR_ADDR      (RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET)
#define GPIOA_BASE_ADDR       0x40020000UL

int main(void) {

  // Declare pointer to RCC clock config register
  uint32_t *pRccCfgr = (uint32_t*) RCC_CFGR_ADDR;

  // Select HSI for MCO1, set bits 22/21 to 00
  *pRccCfgr &= ~(0x03 << 21);

  // Configure MCO1 presclaer division by 4, set bits 25/26 to 11
  *pRccCfgr |= (1 << 25);
  *pRccCfgr |= (1 << 26);

  // Configure PA8 to AF0 (alternate function) mode to behave as MCO1 signal (PA8 pin belongs in GPIOA peripheral)

  // PA8 pin is in GPIOA port group, and GPIOA peripheral is on AHB1 bus
  uint32_t * pRccAhb1Enr = (uint32_t*) RCC_AHB1ENR_ADDR;
  *pRccAhb1Enr |= (1 << 0); // enable GPIOA periphral clock


  // ** some stuff that hasn't been taught yet **
  // Configure PA8 as alternate function mode
  uint32_t *pGPIOAModeReg = (uint32_t*)(GPIOA_BASE_ADDR + 00);
	*pGPIOAModeReg &= ~( 0x3 << 16); //clear
	*pGPIOAModeReg |= ( 0x2 << 16);  //set

	// Configure the alternation function register to set the mode 0 for PA8

	uint32_t *pGPIOAAltFunHighReg = (uint32_t*)(GPIOA_BASE_ADDR + 0x24);
	*pGPIOAAltFunHighReg &= ~( 0xf << 0);


  for(;;);
}
