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
char output_voltage_buffer[32];
char output_temp_buffer[32];
uint16_t adc_voltage;
uint16_t adc_temp;
uint32_t battery_voltage;
float resistor_temp;

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
    for(int i = 0; i < CYCLE_COUNT; i++)
    {
      adc_voltage = adc.read_native(3);
      battery_voltage   = adc.to_mvolts(adc_voltage);
      
      sprintf(output_voltage_buffer, "batt_voltage = %lu mV", battery_voltage);
      uart.send_message((uint8_t *)output_voltage_buffer);
      
      if(battery_voltage > LOWER_EDGE && battery_voltage < UPPER_EDGE)
      {
        // Beeper on and stop test
      }
      
      key.control(KEY_PRECHARGE, KEY_ON);
      
      // 2.5 sec
      for (uint32_t x = 0; x < 52500000; x++)
      {
        __asm__("nop");
      } 
      
      key.control(KEY_MAIN_1, KEY_ON);
      key.control(KEY_MAIN_2, KEY_ON);
      key.control(KEY_MAIN_3, KEY_ON);
      
      // 3 sec
      for (uint32_t y = 0; y < 63000000; y++)
      {
        __asm__("nop");
      }

      key.control(KEY_PRECHARGE, KEY_OFF);
      
      key.control(KEY_MAIN_1, KEY_OFF);
      key.control(KEY_MAIN_2, KEY_OFF);
      key.control(KEY_MAIN_3, KEY_OFF);
      
      adc_temp = adc.read_native(7);
      resistor_temp = adc.calc_temp(adc_temp);
      
      //
      int temp = (int)(resistor_temp * 10.0f);
      snprintf(output_temp_buffer, sizeof(output_temp_buffer), "Temperature = %d.%d C", temp / 10, temp % 10);
      
      uart.send_message((uint8_t *)output_temp_buffer);
    }
    
  }
}
