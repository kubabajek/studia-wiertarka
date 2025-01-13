#include "uart_data_handler.h"

volatile static uint8_t received_data = 0;
volatile static UART_CB_SPEED cb_function_speed = NULL;
volatile static UART_CB_POWER cb_function_power = NULL;
volatile static UART_CB_DIRECTION cb_function_direction = NULL;

void uart_data_handler_init(){
    HAL_UART_Receive_IT(&huart2, &received_data, 1);
}

void uart_data_handler_register_callback_speed(UART_CB_SPEED function_speed_handler){
	cb_function_speed = function_speed_handler;
}

void uart_data_handler_register_callback_power(UART_CB_POWER function_power_handler){
	cb_function_power = function_power_handler;
}

void uart_data_handler_register_callback_direction(UART_CB_DIRECTION function_direction_handler){
	cb_function_direction = function_direction_handler;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if (huart == &huart2){
        if(received_data <= 100){
            if(cb_function_speed != NULL) cb_function_speed(received_data);
        }
        else{
            switch(received_data){
                case 200: if(cb_function_power != NULL) cb_function_power(true); led_set(true); break;
                case 201: if(cb_function_power != NULL) cb_function_power(false); led_set(false); break;
                case 202: if(cb_function_direction != NULL) cb_function_direction(true); led_set(true); break;
                case 203: if(cb_function_direction != NULL) cb_function_direction(false); led_set(false); break;
                default: NULL;
            }
        }
        uart_transmit_byte(received_data);
        HAL_UART_Receive_IT(&huart2, &received_data, 1);
    }
}

//simplified uart and led functions
void uart_transmit_byte(uint8_t data){
    HAL_UART_Transmit(&huart2, &data, 1, 100);
}

void uart_transmit_data(uint8_t* data, uint16_t length){
    HAL_UART_Transmit(&huart2, data, length, 100);
}

void led_set(bool state){
    if(state) HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    else HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void led_toggle(){
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}