#ifndef ADC_HPP
#define ADC_HPP

#include <cstdint>

class Adc
{
    public:
        void config                 (void);                 // Конфигурирование АЦП
        static uint16_t read_native (uint8_t channel);      // Чтение "сырых" значений АЦП
        uint32_t to_mvolts          (uint16_t value);       // Конвертирование значения АЦП в мВ
        uint32_t calc_temp          (uint16_t value);       // Расчёт температуры резистора (термистора)
        uint32_t calc_res           (uint16_t value);       // Расчёт текущего сопротивления термистора
};

#endif
