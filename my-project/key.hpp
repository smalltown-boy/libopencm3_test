#ifndef KEY_HPP
#define KEY_HPP

#include <cstdint>

#define KEY_MAIN_1 0
#define KEY_MAIN_2 1
#define KEY_MAIN_3 2
#define KEY_PRECHARGE 3

class Key
{
    public:
        void config (void);
        void control (uint8_t key, uint8_t state);
        
};

#endif
