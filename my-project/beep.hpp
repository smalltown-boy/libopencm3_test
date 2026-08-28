#ifndef BEEP_HPP
#define BEEP_HPP

#include <cstdint>


class Beeper
{
    public:
        void config  (void);     // Конфигурирование сигнализации
        void on      (void);     // Включение сигнализации                   
        void off     (void);     // Отключение сигнализации
        
};

#endif
