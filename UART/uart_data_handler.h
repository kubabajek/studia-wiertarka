#include "main.h"

typedef void (*UART_VOLTAGE_CB)(uint8_t voltage);


void uart_data_handler_init(); //configure uart and start transmission
void uart_data_handler_register_callback(UART_VOLTAGE_CB); //pass once any function returning void and getting uint8_t to call it with percent value after change