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

#include "SEGGER_RTT.h"

#define LOG(fmt, ...)    printf(fmt "\r\n", ##__VA_ARGS__)

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

    LOG("Heap free: %d, min_ever: %d", xPortGetFreeHeapSize(), xPortGetMinimumEverFreeHeapSize());

    while (1) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_7);
        vTaskDelay(pdMS_TO_TICKS(500));

        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
        vTaskDelay(pdMS_TO_TICKS(500));

        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9);
        vTaskDelay(pdMS_TO_TICKS(500));

        LOG("led flash: %d", cnt++);
    }

    vTaskDelete(NULL);
}

int main(int argc, char *argv[])
{
    BaseType_t ret = pdPASS;

    HAL_Init();

    SEGGER_RTT_Init();

    /**
     * 确保进入 Sleep 休眠模式后, 还能使用 RTT
     * 1. Sleep 模式下 DEBUG 保持使能, 允许调试器在低功耗模式中连接和查看内核状态
     * 2. Sleep 模式下 SRAM1 保持使能, 数据正常保留
     * 3. Sleep 模式下 DMA1 保持使能, 防止总线矩阵被关闭, 调试器能通过 AHB-AP 访问 SRAM1 中的 RTT 数据
     */
    HAL_DBGMCU_EnableDBGSleepMode();
    __HAL_RCC_SRAM1_CLK_SLEEP_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

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
