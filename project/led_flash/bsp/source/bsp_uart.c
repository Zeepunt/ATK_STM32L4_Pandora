/**
 * bsp_uart.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdbool.h>

#include "stm32l4xx.h"
#include "bsp_uart.h"

static UART_HandleTypeDef s_uart1_handler = {0};

static bool s_uart1_init = false;

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&s_uart1_handler);
}

/**
 * 调用关系:
 * HAL_UART_Init
 *   -> HAL_UART_MspInit
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init;

    if (huart->Instance == USART1) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();

        gpio_init.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        gpio_init.Mode      = GPIO_MODE_AF_PP;
        gpio_init.Pull      = GPIO_PULLUP;
        gpio_init.Speed     = GPIO_SPEED_FAST;
        gpio_init.Alternate = GPIO_AF7_USART1;

        HAL_GPIO_Init(GPIOA, &gpio_init);
    }
}

int bsp_uart_init(void)
{
    HAL_StatusTypeDef status = HAL_OK;

    s_uart1_handler.Instance        = USART1;
    s_uart1_handler.Init.BaudRate   = 115200;
    s_uart1_handler.Init.WordLength = UART_WORDLENGTH_8B;
    s_uart1_handler.Init.StopBits   = UART_STOPBITS_1;
    s_uart1_handler.Init.Parity     = UART_PARITY_NONE;
    s_uart1_handler.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    s_uart1_handler.Init.Mode       = UART_MODE_TX_RX;

    status = HAL_UART_Init(&s_uart1_handler);
    if (status != HAL_OK) {
        return -1;
    }

    __HAL_UART_ENABLE_IT(&s_uart1_handler, UART_IT_RXNE);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);

    s_uart1_init = true;

    return 0;
}

int bsp_uart_write(const char *buf, int len)
{
    if (!s_uart1_init) {
        return -1;
    }

    /* 这里的 Timeout 是依赖 HAL 库里面的 uwTick, uwTick 的更新是通过 HAL_IncTick() 来实现的 */
    return HAL_UART_Transmit(&s_uart1_handler, buf, len, HAL_MAX_DELAY);
}
