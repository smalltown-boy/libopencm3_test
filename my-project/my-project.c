#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/adc.h>
#include <stdio.h>

// System functions
void clock_config (void);
void gpio_config  (void);
void usart_config (void);
void adc_config   (void);

// User finctions
void keys_control        (uint8_t key, uint8_t state);
void usart_send_message  (const uint8_t *message);
static uint16_t read_adc_native (uint8_t channel);
uint32_t adc_to_mvolts          (uint16_t adc_value);

// Realization
void clock_config (void)
{
  // Frequency
  rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    
  // Clock for port C
  rcc_periph_clock_enable (RCC_GPIOC);
  
  // Clock for port A
  rcc_periph_clock_enable (RCC_GPIOA);
  
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
}

void usart_config (void)
{
  // Alternate function for tx
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO9 | GPIO10);
  
  // Set usart parameters
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_mode(USART1, USART_MODE_TX);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

  // Enable usart
  usart_enable(USART1);
}

void adc_config   (void)
{
  // Config pin A0 for analog mode
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO3);
  
  adc_power_off(ADC1);
  adc_disable_scan_mode(ADC1);
  adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);

  adc_power_on(ADC1);
}

static uint16_t read_adc_native(uint8_t channel)
{
  uint8_t channel_array[16];
  channel_array[0] = channel;
  adc_set_regular_sequence(ADC1, 1, channel_array);
  adc_start_conversion_regular(ADC1);
  while (!adc_eoc(ADC1));
  uint16_t reg16 = adc_read_regular(ADC1);
  return reg16;
}

void usart_send_message (const uint8_t *message)
{
  while(*message)
  {
    usart_send_blocking(USART1, *message);
    message++;
  }
  
  usart_send_blocking(USART1, '\r');
  usart_send_blocking(USART1, '\n');
}

uint32_t adc_to_mvolts(uint16_t adc_value)
{
    return ((uint32_t)adc_value * 3300) / 4095;
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
  char output_buffer[32];
  
  clock_config ();
  gpio_config  (); 
  usart_config ();
  adc_config   ();
  
  while(1)
  {
    uint16_t adc_value = read_adc_native(ADC_CHANNEL3);
    uint32_t voltage   = adc_to_mvolts(adc_value);
    sprintf(output_buffer, "batt_voltage = %lu mV", voltage);
    usart_send_message((uint8_t *)output_buffer);
    
    for (int i = 0; i < 30000000; i++) 
    {	
      __asm__("NOP");
    }
  }
}
