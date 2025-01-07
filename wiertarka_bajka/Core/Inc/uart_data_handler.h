#include <stdbool.h>
#include "main.h"

typedef void (*UART_CB_SPEED)(uint8_t speed); //speed range 0-100
typedef void (*UART_CB_POWER)(bool state); // true - power on, false - power off
typedef void (*UART_CB_DIRECTION)(bool direction); // true - right, false - left

//init function - call after full hal uart library initialization!
void uart_data_handler_init(); //start transmission - huart2

//register callback functions to get data: speed, power, direction
void uart_data_handler_register_callback_speed(UART_CB_SPEED function_speed_handler); //pass once any function returning void and getting uint8_t to call it with percent value after change
void uart_data_handler_register_callback_power(UART_CB_POWER function_power_handler); //pass function taking bool, returning void with handling value true - power on, false - power off
void uart_data_handler_register_callback_direction(UART_CB_DIRECTION function_direction_handler); //pass function taking bool, returning void with handling direction true - right, false - left

//simplified uart and led functions :)
void uart_transmit_byte(uint8_t data); //send one byta via uart with 100ms timeout
void uart_transmit_data(uint8_t* data, uint16_t length); //send bytes array via uart with 100ms timeout
void led_set(bool state);
void led_toggle();
