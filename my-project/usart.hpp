#ifndef USART_HPP
#define USART_HPP

#include <cstdint>

class Usart
{
    public:
        void config (void);
        void send_message (const uint8_t *message);
};

#endif
