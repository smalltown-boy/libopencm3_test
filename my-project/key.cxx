#include <libopencm3/stm32/gpio.h>
#include "key.hpp"

void Key::config ()
{
  // Config main keys  
  gpio_mode_setup (GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
  gpio_mode_setup (GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);
  gpio_mode_setup (GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);
  
  // Config pre-charging line
  gpio_mode_setup (GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
}

void Key::control (uint8_t key, uint8_t state)
{
  uint16_t output_pins[] = {GPIO7, GPIO8, GPIO9, GPIO14};
  
  // For safety
  if(key > 3 || state > 1) 
    return;
    
  switch(key)
  {
    case KEY_MAIN_1: 
    case KEY_MAIN_2: 
    case KEY_MAIN_3:
      if(state)
      {
        gpio_set (GPIOC, output_pins[key]);
      }
      else
      {
        gpio_clear (GPIOC, output_pins[key]);
      }
    break;
    
    case KEY_PRECHARGE:
      if(state)
      {
        gpio_set (GPIOB, output_pins[key]);
      }
      else
      {
        gpio_clear (GPIOB, output_pins[key]);
      }
    break;
    
    default:
      return;
  }
}
