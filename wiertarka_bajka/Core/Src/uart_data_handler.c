#include "uart_data_handler.h"

volatile static uint8_t button_percent = 0;
volatile static UART_VOLTAGE_CB voltage_cb_function = NULL;

//static function declarations
static void uart_transmit_byte(uint8_t data);

void uart_data_handler_init(){
    HAL_UART_Receive_IT(&huart2,&button_percent,1);
}

void uart_data_handler_register_callback(UART_VOLTAGE_CB function){
	voltage_cb_function = function;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    // HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    if (huart == &huart2){
       if(voltage_cb_function != NULL) voltage_cb_function(button_percent);
//        uart_transmit_byte(button_percent);
        HAL_UART_Receive_IT(&huart2,&button_percent,1);
    }
}

// static void uart_transmit_byte(uint8_t data){
//     HAL_UART_Transmit(&huart2, &data, 1, 100);
// }
