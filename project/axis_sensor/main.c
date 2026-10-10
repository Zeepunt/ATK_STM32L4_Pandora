/**
 * main.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdio.h>
#include <string.h>

#include "stm32l4xx.h"

#include "FreeRTOS.h"
#include "task.h"

#include "SEGGER_RTT.h"

#include "drv_icm20608.h"

#define LOG(fmt, ...)    printf(fmt "\r\n", ##__VA_ARGS__)

static TaskHandle_t s_app_task_tid = NULL;

void SystemClock_Config(void)
{
    HAL_StatusTypeDef status = HAL_OK;

    RCC_OscInitTypeDef osc_init = {0};
    RCC_ClkInitTypeDef clk_init = {0};

    status = HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
    if (status != HAL_OK) {
        LOG("HAL_PWREx_ControlVoltageScaling faield: %d", status);
    }

    osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc_init.HSEState       = RCC_HSE_ON;
    osc_init.PLL.PLLState   = RCC_PLL_ON;
    osc_init.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    osc_init.PLL.PLLM       = 1;
    osc_init.PLL.PLLN       = 20;
    osc_init.PLL.PLLR       = RCC_PLLR_DIV2;
    osc_init.PLL.PLLQ       = RCC_PLLQ_DIV2;
    osc_init.PLL.PLLP       = RCC_PLLP_DIV7;
    status = HAL_RCC_OscConfig(&osc_init);
    if (status != HAL_OK) {
        LOG("HAL_RCC_OscConfig faield: %d", status);
    }

    clk_init.ClockType      = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk_init.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    clk_init.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
    clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
    status = HAL_RCC_ClockConfig(&clk_init, FLASH_LATENCY_4);
    if (status != HAL_OK) {
        LOG("HAL_RCC_ClockConfig faield: %d", status);
    }
}

static void priv_app_task(void *args)
{
    int16_t gx = 0;
    int16_t gy = 0;
    int16_t gz = 0;

    int16_t ax = 0;
    int16_t ay = 0;
    int16_t az = 0;

    LOG("SYSCLK: %d Hz", HAL_RCC_GetSysClockFreq());
    LOG("HCLK  : %d Hz", HAL_RCC_GetHCLKFreq());
    LOG("PCLK1 : %d Hz", HAL_RCC_GetPCLK1Freq());
    LOG("PCLK2 : %d Hz", HAL_RCC_GetPCLK2Freq());

    LOG("Heap free: %d, min_ever: %d", xPortGetFreeHeapSize(), xPortGetMinimumEverFreeHeapSize());

    drv_icm20608_init();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(20)); /* 1s / 50Hz = 20ms */

        drv_icm20608_gyroscope_get(&gx, &gy, &gz);
        drv_icm20608_accelerometer_get(&ax, &ay, &az);

        LOG("Gyroscope    : %d, %d, %d", gx, gy, gz);
        LOG("Accelerometer: %d, %d, %d", ax, ay, az);
    }

    vTaskDelete(NULL);
}

int main(int argc, char *argv[])
{
    BaseType_t ret = pdPASS;

    HAL_Init();

    SEGGER_RTT_Init();

    SystemClock_Config();
    SystemCoreClockUpdate();

    HAL_DBGMCU_EnableDBGSleepMode();
    __HAL_RCC_SRAM1_CLK_SLEEP_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    ret = xTaskCreate(priv_app_task,
                      "app",
                      1024 * sizeof(StackType_t),
                      NULL,
                      15,
                      &s_app_task_tid);

    vTaskStartScheduler();

    /* should not be here */
    while (1);

    return 0;
}
