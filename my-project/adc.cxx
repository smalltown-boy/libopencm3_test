#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <math.h>
#include "adc.hpp"

void Adc::config   (void)
{
  // Config pin A0 for analog mode
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO3);
  
  adc_power_off(ADC1);
  adc_disable_scan_mode(ADC1);
  adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);

  adc_power_on(ADC1);
}

uint16_t  Adc::read_native(uint8_t channel)
{
  uint8_t channel_array[16];
  channel_array[0] = channel;
  adc_set_regular_sequence(ADC1, 1, channel_array);
  adc_start_conversion_regular(ADC1);
  while (!adc_eoc(ADC1));
  uint16_t reg16 = adc_read_regular(ADC1);
  return reg16;
}

uint32_t Adc::to_mvolts(uint16_t value)
{
    return ((uint32_t)value * 3300) / 4095;
}

uint32_t Adc::calc_temp(uint16_t value)
{
    const float res_nom = 10000.0f;     // 25 celsius, 10 kOm
    const float kelvin_nom = 298.15f;   // 25 celsius in kelvin
    const float b_coiff  = 3950.0f;     // B-coiff

    float resistance = calc_res(value);
    float temperature_kelvin = 1.0f / (1.0f / kelvin_nom + log(resistance / res_nom) / b_coiff);

    return temperature_kelvin - 273.15f;
}

uint32_t Adc::calc_res(uint16_t value)
{
    float ntc_resistance = 10000.0f;
    float vcc_voltage = 3.3f;

    float voltage = ((float)value * vcc_voltage) / 4095.0f;
    
    return ntc_resistance * (vcc_voltage - voltage) / voltage;   
}

