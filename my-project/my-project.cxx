#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <libopencm3/stm32/adc.h>
#include <stdio.h>

// Custom
#include "usart.hpp"
#include "adc.hpp"

// Define's
#define CYCLE_COUNT 1000
#define LOWER_EDGE  2200
#define UPPER_EDGE  2700

// Variable's
char output_buffer[32];
uint16_t adc_value;
uint32_t voltage;

// System functions
void clock_config (void);
void gpio_config  (void);

// User finctions
void keys_control        (uint8_t key, uint8_t state);

// Realization
void clock_config (void)
{
  // Frequency
  rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    
  // Clock for port C
  rcc_periph_clock_enable (RCC_GPIOC);
  
  // Clock for port A
  rcc_periph_clock_enable (RCC_GPIOA);
  
  // Clock for port B
  rcc_periph_clock_enable (RCC_GPIOB);
  
  // Clock for usart1
  rcc_periph_clock_enable(RCC_USART1);
  
  // Clock for ADC
  rcc_periph_clock_enable(RCC_ADC1);
}

void gpio_config  (void)
{  
  // Config main keys  
  gpio_mode_setup (GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
  gpio_mode_setup (GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);
  gpio_mode_setup (GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);
  
  // Config pre-charging line
  gpio_mode_setup (GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
}

void keys_control (uint8_t key, uint8_t state)
{
  uint16_t output_pins[] = {GPIO7, GPIO8, GPIO9};
  
  // For safety
  if(key > 2 || state > 1) 
    return;

  if(state)
  {
    gpio_set    (GPIOC, output_pins[key]);
  }
  else
  {
    gpio_clear  (GPIOC, output_pins[key]);
  }
}

int main(void) 
{
  clock_config ();
  
  Usart uart;
  Adc adc;
  
  uart.usart_config();
  adc.adc_config();
  
  gpio_config  (); 
  
  while(1)
  {
    adc_value = adc.adc_read_native(ADC_CHANNEL3);
    voltage   = adc.adc_to_mvolts(adc_value);
    sprintf(output_buffer, "batt_voltage = %lu mV", voltage);
    uart.usart_send_message((uint8_t *)output_buffer);
    
    for (int i = 0; i < 30000000; i++) 
    {	
      __asm__("NOP");
    }

    /*
    for(int i = 0; i < CYCLE_COUNT; i++)
    {
      adc_value = read_adc_native(ADC_CHANNEL3);
      voltage   = adc_to_mvolts(adc_value);
      
      if(voltage > LOWER_EDGE && voltage < UPPER_EDGE)
      {
        // Beeper on and stop test
      }
    }
    */
  }
}
