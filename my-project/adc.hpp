#ifndef ADC_HPP
#define ADC_HPP

#include <cstdint>

class Adc
{
    public:
        void config                 (void);
        static uint16_t read_native (uint8_t channel);
        uint32_t to_mvolts          (uint16_t value);
};

#endif
