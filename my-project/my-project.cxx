#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdio.h>
#include <stdbool.h>

// Собственные библиотеки
#include "usart.hpp"
#include "adc.hpp"
#include "key.hpp"
#include "beep.hpp"

// Дефайны
#define CYCLE_COUNT 1000
#define LOWER_EDGE  2200
#define UPPER_EDGE  2700

// Переменные
char output_voltage_buffer[32]; // Для вывода напряжения батареи
char output_temp_buffer[32];    // Для вывода температуры резистора

uint16_t adc_voltage;           // Переменные для "сырых" знацений АЦП
uint16_t adc_temp;

uint32_t battery_voltage;       // Для получения итоговых значений
float resistor_temp;

bool failure = false;           // Флаг для отслеживания сбоя

// Прототипы функций
void clock_config (void);
bool test (void);

// Настройка тактирования
void clock_config (void)
{
  // Настройка частоты микроконтроллера
  rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    
  // Включение тактирования портов ввода-вывода
  rcc_periph_clock_enable (RCC_GPIOA);
  rcc_periph_clock_enable (RCC_GPIOB);
  rcc_periph_clock_enable (RCC_GPIOC);
  
  // Включение тактирования серийного интерфейса
  rcc_periph_clock_enable(RCC_USART1);
  
  // Включение тактирования АЦП
  rcc_periph_clock_enable(RCC_ADC1);
}

int main(void) 
{
  clock_config ();    // Включение тактирования
  
  Usart uart;
  Adc adc;
  Key key;
  Beeper beep;
  
  uart.config();      // Конфигурирование серийного интерфейса
  adc.config();       // Конфигурирование АЦП
  key.config();       // Конфигурирование ключей
  beep.config();      // Настройка сигнализации
  beep.off();         // Сигнализация отключена
  
  while(1)
  {        
    for(int i = 0; i < CYCLE_COUNT; i++)                                  // Цикл на 1000 итераций
    {
      adc_voltage = adc.read_native(3);                                   // Получаем "сырое" значение из АЦП (батарея, канал 3)
      battery_voltage   = adc.to_mvolts(adc_voltage);                     // Конвертируем его в милливольты
    
      if(battery_voltage > LOWER_EDGE && battery_voltage < UPPER_EDGE)    // Проводим приблизительное сравнение (акб?)
      {
        beep.on();          // Если условие истинно, включаем сигнализацию
        failure = true;     // Поднимаем флаг ошибки
        break;              // Выходим из цикла
      }
      
      key.control(KEY_PRECHARGE, KEY_ON);               // Активируем цепь предзаряда
      
      // На реализацию нормальных задержек не хватило времени (2.5 сек)
      for (uint32_t x = 0; x < 52500000; x++)
      {
        __asm__("nop");
      } 
      
      key.control(KEY_MAIN_1, KEY_ON);                  // Активируем основные ключи
      key.control(KEY_MAIN_2, KEY_ON);
      key.control(KEY_MAIN_3, KEY_ON);
      
      // 3 сек.
      for (uint32_t y = 0; y < 63000000; y++)
      {
        __asm__("nop");
      }

      key.control(KEY_PRECHARGE, KEY_OFF);              // Выключаем все ключи
      
      key.control(KEY_MAIN_1, KEY_OFF);
      key.control(KEY_MAIN_2, KEY_OFF);
      key.control(KEY_MAIN_3, KEY_OFF);
      
      adc_temp = adc.read_native(7);                    // Получение "сырого" значения из АЦП (температура, канал 7)
      resistor_temp = adc.calc_temp(adc_temp);          // Получаем значение температуры термистора
      
      // Включение поддержки формата чисел с плавающей точкой не сработала через мэйкфайл
      // Был найден такой выход
      int temp = (int)(resistor_temp * 10.0f);
      snprintf(output_temp_buffer, sizeof(output_temp_buffer), "Temperature = %d.%d C", temp / 10, temp % 10);
      
      uart.send_message((uint8_t *)output_temp_buffer); // Отправление текущей температуры через серийный интерфейс
    }
    
    if(failure)   // Если был получен флаг сбоя - просто крутимся в бесконечном цикле
    {
      continue;
    }
  }
}
