#include "stm32f4xx.h"

void SystemClock_Config(void);
void GPIO_Config(void);
void TIM6_Config(void);
volatile uint32_t ticks;

int main(void)
{
  SystemClock_Config();
  GPIO_Config();
  TIM6_Config();

  while (1)
  {
    if (ticks >= 34380)
    {
      ticks = 0;
      GPIOC->ODR ^= (1 << 15);
    }
  }
}

void SystemClock_Config(void)
{

}

void GPIO_Config(void)
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  GPIOC->MODER |= (1 << (15 * 2));
}

void TIM6_Config(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
  TIM6->PSC = 8399;
  TIM6->ARR = 1000;
  TIM6->DIER |= TIM_DIER_UIE;
  TIM6->CR1 |= TIM_CR1_CEN;

  NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

void TIM6_DAC_IRQHandler(void)
{
  if (TIM6->SR & TIM_SR_UIF)
  {
    TIM6->SR &= ~TIM_SR_UIF;
    ticks++;
  }
}
