#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "usart.hpp"

void Usart::usart_config(void)
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

void Usart::usart_send_message (const uint8_t *message)
{
  while(*message)
  {
    usart_send_blocking(USART1, *message);
    message++;
  }
  
  usart_send_blocking(USART1, '\r');
  usart_send_blocking(USART1, '\n');
}


