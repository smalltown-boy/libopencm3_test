#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

// System functions
void gpio_config  (void);
void usart_config (void);

// User finctions
void keys_control (uint8_t key, uint8_t state);

// Realization
void gpio_config  (void)
{  
  // Config main keys
  rcc_periph_clock_enable (RCC_GPIOC);
  
  gpio_mode_setup (GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
  gpio_mode_setup (GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);
  gpio_mode_setup (GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);
}

void keys_control (uint8_t key, uint8_t state)
{
  uint16_t output_pins[] = {GPIO7, GPIO8, GPIO9};
  
  // For safety
  if(key > 2 || state > 1) 
    return;

  if(state)
  {
    gpio_set    (GPIOC, pins[key]);
  }
  else
  {
    gpio_clear  (GPIOC, pins[key]);
  }
}

int main(void) 
{
  gpio_config();  
  
  while(1)
  {
    
  }
}
