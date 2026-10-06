/**
 * main.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdio.h>
#include <string.h>

#include "stm32l4xx.h"
#include "bsp_uart.h"
#include "FreeRTOS.h"
#include "task.h"

static TaskHandle_t s_app_task_tid = NULL;

void vApplicationTickHook(void)
{
    HAL_IncTick();
}

static void priv_app_task(void *args)
{
    uint32_t cnt = 0;

    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET);
    HAL_GPIO_Init(GPIOE, &gpio_init);

    printf("Heap free: %d, min_ever: %d\n", xPortGetFreeHeapSize(), xPortGetMinimumEverFreeHeapSize());

    while (1) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_7);
        vTaskDelay(pdMS_TO_TICKS(500));

        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
        vTaskDelay(pdMS_TO_TICKS(500));

        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9);
        vTaskDelay(pdMS_TO_TICKS(500));

        printf("led flash: %d\n", cnt++);
    }

    vTaskDelete(NULL);
}

int main(int argc, char *argv[])
{
    BaseType_t ret = pdPASS;

    HAL_Init();

    bsp_uart_init();

    ret = xTaskCreate(priv_app_task,
                      "app",
                      512 * sizeof(StackType_t),
                      NULL,
                      15,
                      &s_app_task_tid);

    vTaskStartScheduler();

    /* should not be here */
    while (1);

    return 0;
}
