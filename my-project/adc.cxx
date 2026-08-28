#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <math.h>
#include "adc.hpp"

void Adc::config   (void)
{
  // Переводим выводы 3 и 7 порта А в аналоговый режим работы
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO3);
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO7);
  
  // Настройка АЦП
  adc_power_off(ADC1);
  adc_disable_scan_mode(ADC1);
  adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);

  // Включение АЦП
  adc_power_on(ADC1);
}

uint16_t  Adc::read_native(uint8_t channel)
{
  // Получение списка каналов АЦП для опроса (нужно сделать напрямую)
  uint8_t channel_array[16];
  channel_array[0] = channel;
  adc_set_regular_sequence(ADC1, 1, channel_array);
  // Запуск преобразования
  adc_start_conversion_regular(ADC1);
  while (!adc_eoc(ADC1));
  uint16_t reg16 = adc_read_regular(ADC1);
  return reg16;
}

uint32_t Adc::to_mvolts(uint16_t value)
{
  // Перевод "сырых" значений АЦП в мВ
  return ((uint32_t)value * 3300) / 4095;
}

uint32_t Adc::calc_temp(uint16_t value)
{
  /*
    Мне не было известно, какой именно тип датчика используется в схеме, поэтому я наугад выбрал
    термистор от Вишай (даташит лежит в корне проекта).
    
    Есть сопротивление термистора при 25 градусах Цельсия (10 кОм)
    А также материальная константа В, указанная для данного сопротивления конкретно этого датчика
    
    Подробнее о расчётах: https://smacont.ru/wiki/33/
  */
    float res_nom    = 10000.0;     
    float kelvin_nom = 298.15;   
    float b_coiff    = 3950.0;     

    float resistance         = calc_res(value);
    float temperature_kelvin = 1.0 / (1.0 / kelvin_nom + log(resistance / res_nom) / b_coiff);

    return temperature_kelvin - 273.15;
}

uint32_t Adc::calc_res(uint16_t value)
{
    float ntc_resistance = 10000.0;    // Сопротивление термистора
    float vcc_voltage    = 3.3;        // Напряжение, прикладываемое к нему

    float voltage        = ((float)value * vcc_voltage) / 4095.0; // Расчёт текущего напряжения
    
    return ntc_resistance * (vcc_voltage - voltage) / voltage;    // Получаем из напряжения сопротивление
}

