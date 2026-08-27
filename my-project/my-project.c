#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

// System functions
void clock_config (void);
void gpio_config  (void);
void usart_config (void);

// User finctions
void keys_control (uint8_t key, uint8_t state);
void usart_send_message (const uint8_t *message);

// Realization
void clock_config (void)
{
  // Frequency
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_170MHZ]);
    
  // Clock for port C
  rcc_periph_clock_enable (RCC_GPIOC);
  
  // Clock for usart1
  rcc_periph_clock_enable(RCC_USART1);
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
  gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO4 | GPIO5);
  gpio_set_af(GPIOC, GPIO_AF7, GPIO4 | GPIO5);
  
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
  gpio_config  (); 
  usart_config ();
  
  
  while(1)
  {
    usart_send_message ("Ground control to Major Tom");
    
    for (int i = 0; i < 30000000; i++) 
    {	
      __asm__("NOP");
    }
  }
}
