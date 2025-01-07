#include "main.h"

typedef void (*UART_CB_SPEED)(uint8_t speed); //speed range 0-100
typedef void (*UART_CB_POWER)(bool state); // true - power on, false - power off
typedef void (*UART_CB_DIRECTION)(bool direction); // true - right, false - left


void uart_data_handler_init(); //configure uart and start transmission
void uart_data_handler_register_callback_speed(UART_CB_SPEED function_speed_handler); //pass once any function returning void and getting uint8_t to call it with percent value after change
void uart_data_handler_register_callback_power(UART_CB_POWER function_power_handler); //pass function taking bool, returning void with handling value true - power on, false - power off
void uart_data_handler_register_callback_direction(UART_CB_DIRECTION function_direction_handler); //pass function taking bool, returning void with handling direction true - right, false - left