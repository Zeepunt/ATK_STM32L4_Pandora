/**
 * exception.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdio.h>
#include <string.h>

#include "stm32l4xx.h"

#include "FreeRTOS.h"
#include "task.h"

void SysTick_Handler(void)
{
    /**
     * 需要确保 configTICK_RATE_HZ 和 HAL_TICK_FREQ_DEFAULT 一致
     *
     * 当前的配置如下:
     * 1. configTICK_RATE_HZ = 1000
     * 2. HAL_TICK_FREQ_DEFAULT = HAL_TICK_FREQ_1KHZ
     */

    HAL_IncTick();

    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        extern void xPortSysTickHandler(void);
        xPortSysTickHandler();
    }
}