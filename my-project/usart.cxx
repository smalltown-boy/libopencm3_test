#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "usart.hpp"

void Usart::config(void)
{
  // Для отправки и получения данных rx и tx необходимо перевести в альтернативный режим
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO9 | GPIO10);
  
  // Установка параметров передачи данных
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_mode(USART1, USART_MODE_TX);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

  // Инициализация интерфейса
  usart_enable(USART1);
}

void Usart::send_message (const uint8_t *message)
{
  // Отправка массива данных через интерфейс
  while(*message)
  {
    usart_send_blocking(USART1, *message);
    message++;
  }
  
  // Отправка символов перевода строки и возврата каретки
  usart_send_blocking(USART1, '\r');
  usart_send_blocking(USART1, '\n');
}


