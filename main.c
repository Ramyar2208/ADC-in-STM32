/* ADC in STM32
- Single mode, regular channel
- Right alignment
*/

#include "stm32f10x.h"
void RCC_config(void);
void ADC1_2_IRQHandler(void);
void delay(uint16_t);

static uint16_t data;

void delay(uint16_t d)
{
  for(; d; d --);
}

void ADC1_2_IRQHandler()
{
  if(ADC1 -> SR & ADC_SR_EOC)
  {
    data = (uint16_t)ADC1 -> DR;         // Read the data (16-bit), this will also clear the flag
    GPIOC -> BSRR = GPIO_BSRR_BR13;     // PC13 LED - ON
    // Turn off ADC if required using ADCON bit
  }
}

void RCC_config() // RCC clock configuration
{
  RCC -> CR |= RCC_CR_HSEON;                               // HSE ON
  RCC -> CFGR |= (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL4);    // Setting PLL = HSE * 4
  RCC -> CFGR |= RCC_CFGR_SW_1;                            // SysClk = PLLCLK
  RCC -> CR |= RCC_CR_PLLON;                                               
  // Turn ON PLL after above PLL configurations, making SysClk = HSE * 4 = 8 * 4 = 32 MHz
}

int main()
{
  RCC_config();
  
  /* PC13 LED configuration */
  RCC -> APB2ENR |= RCC_APB2ENR_IOPCEN;                  // Enable Port-C Clock
  GPIOC -> CRH |= GPIO_CRH_MODE13_1 | GPIO_CRH_CNF13_0;  // PortC_Pin 13 as output @2MHZ with open-drain
  GPIOC -> BSRR = GPIO_BSRR_BS13;                       // Initialize PC13 LED - OFF
  
  /* ADC configuration */
  RCC -> CFGR |= RCC_CFGR_ADCPRE_DIV8;                   // ADC Prescaler: SysClk / 8 = 32 / 8 = 4 MHz
  RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;                  // Enable ADC 1 clock
  /*ADC1 -> SQR1 = 0000 -> 1 conversion */
  ADC1 -> SQR3 |= ADC_SQR3_SQ1_4;                        // Seq 1 : channel 16 (bit 4)
  ADC1 -> SMPR1 |= ADC_SMPR1_SMP16_0 | ADC_SMPR1_SMP16_2;// Sample time = 55.5 cycle
  ADC1 -> CR1 |= ADC_CR1_EOCIE;                          // EOC interrupt enable
  NVIC_EnableIRQ(ADC1_2_IRQn);                           // Enable interrupt in NVIC
  ADC1 -> CR2 |= ADC_CR2_TSVREFE;                        // Enable Temperature sensor
  
  ADC1 -> CR2 |= ADC_CR2_ADON;                           // Turn ON ADC 
  delay(32);                                             // approximate 1 us delay for 32 MHz
  ADC1 -> CR2 |= ADC_CR2_ADON;                           // Turn ON ADC second time
  /* CONT = 0 **, ALIGN = 0 */
  ADC1 -> CR2 |= ADC_CR2_CAL;                            // Calibrate ADC before start
  ADC1 -> CR2 |= ADC_CR2_RSTCAL;                         // Indicates start of calibration
  while(ADC1 -> CR2 & ADC_CR2_RSTCAL);                   // Wait till calibration is done
  
  //ADC1 -> CR2 |= ADC_CR2_SWSTART;                        // Start the regular conversion
  
  while(1)
  {
    // Do nothing
  }
  return 0; 
}
