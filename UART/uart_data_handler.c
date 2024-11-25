#include "uart_data_handler.h"

UART_HandleTypeDef huart2;
volatile static uint8_t button_percent = 0;
volatile static UART_VOLTAGE_CB voltage_cb_function = NULL;

//static function declarations
static void MX_USART2_UART_Init(void);
static void uart_transmit_byte(uint8_t data);

void uart_data_handler_init(){
    MX_USART2_UART_Init();
    HAL_UART_Receive_IT(&huart2,&button_percent,1);
}

void uart_data_handler_register_callback(UART_VOLTAGE_CB function){
  voltage_cb_function = function;
}

static void MX_USART2_UART_Init(void){
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK){
    Error_Handler();
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if (huart == &huart2){
        if(voltage_cb_function != NULL) voltage_cb_function(button_percent);
        uart_transmit_byte(button_percent);
        HAL_UART_Receive_IT(&huart2,&button_percent,1);
    }
}

static void uart_transmit_byte(uint8_t data){
    HAL_UART_Transmit(&huart2, &data, 1, 100);
}