#ifndef ADC_HPP
#define ADC_HPP

#include <cstdint>

class Adc
{
    public:
        void adc_config                 (void);
        static uint16_t adc_read_native (uint8_t channel);
        uint32_t adc_to_mvolts          (uint16_t value);
};

#endif
