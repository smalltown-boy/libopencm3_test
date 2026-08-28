#ifndef USART_HPP
#define USART_HPP

#include <cstdint>

class Usart
{
    public:
        void usart_config (void);
        void usart_send_message (const uint8_t *message);
};

#endif
