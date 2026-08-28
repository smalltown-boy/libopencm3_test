#ifndef USART_HPP
#define USART_HPP

#include <cstdint>

class Usart
{
    public:
        void config       (void);                     // Конфигурирование интерфейса
        void send_message (const uint8_t *message);   // Отправка строки
};

#endif
