#ifdef OneWire_DS18B20_H
#define OneWire_DS18B20_H

#include <Arduino.h> 
#include <Rak3172_Canopus.h>

void set_pin_as_output(GPIO_TypeDef *port, uint16_t pin);
void set_pin_as_input(GPIO_TypeDef *port, uint16_t pin);
void onewire_reset(void);
void onewire_write_bit(uint8_t bit) ;
uint8_t onewire_read_bit(void) ;
void onewire_write_byte(uint8_t data);
uint8_t onewire_read_byte(void);
float read_temperature(void) ;

#endif




