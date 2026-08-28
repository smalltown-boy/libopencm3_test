#include <libopencm3/stm32/gpio.h>
#include "beep.hpp"

void Beeper::config ()
{
  gpio_mode_setup (GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
}

void Beeper::on ()
{
  gpio_set (GPIOB, GPIO3);
}

void Beeper::off ()
{
  gpio_clear (GPIOB, GPIO3);
}
