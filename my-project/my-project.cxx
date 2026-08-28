#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdio.h>

// Custom
#include "usart.hpp"
#include "adc.hpp"
#include "key.hpp"

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


int main(void) 
{
  clock_config ();
  
  Usart uart;
  Adc adc;
  Key key;
  
  uart.config();
  adc.config();
  key.config();
  
  while(1)
  {
    adc_value = adc.read_native(3); //ADC_CHANNEL_3
    voltage   = adc.to_mvolts(adc_value);
    sprintf(output_buffer, "batt_voltage = %lu mV", voltage);
    uart.send_message((uint8_t *)output_buffer);
    
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
