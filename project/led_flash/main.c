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

void SystemClock_Config(void)
{
    HAL_StatusTypeDef status = HAL_OK;

    RCC_OscInitTypeDef osc_init = {0};
    RCC_ClkInitTypeDef clk_init = {0};

    /**
     * Configure the main internal regulator output voltage
     *
     * 1. PWR_REGULATOR_VOLTAGE_SCALE1
     *    typical output voltage at 1.2 V
     *    system frequency up to 80 MHz
     *
     * 2. PWR_REGULATOR_VOLTAGE_SCALE2
     *    typical output voltage at 1.0 V
     *    system frequency up to 26 MHz
     */
    status = HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
    if (status != HAL_OK) {
        LOG("HAL_PWREx_ControlVoltageScaling faield: %d", status);
    }

    /**
     * Initializes the RCC Oscillators
     *
     * RCC_HSE_BYPASS -> 有源晶振 (BYPASS Clock Source)
     * RCC_HSE_ON     -> 无源晶振 (Crystal/Ceramic Resonator)
     *
     * HSE (8 MHz) -> PLLM (/1) -> 8MHz -> PLLN (x20) -> VCO = 160 MHz
     *
     * VCO (Voltage Controlled Oscillator)
     *   -> PPLR (DIV2) -> PPLCLK = 160 MHz / 2 = 80 MHz
     *   -> PPLQ (DIV2) -> PPLQ = 160 MHz / 2 = 80 MHz
     *   -> PPLP (DIV7) -> PPLP = 160 MHz / 7 = 22.857143 MHz
     */
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

    /**
     * Initializes the CPU, AHB and APB buses clocks
     *
     * SYSCLK = PPLCLK = 80 MHz
     * HCLK   = SYSCLK / (AHBCLKDivider) = 80 MHz / 1 = 80 MHz
     * PCLK1  = HCLK / (APB1CLKDivider) = 80 MHz / 1 = 80 MHz
     * PCLK2  = HCLK / (APB2CLKDivider) = 80 MHz / 1 = 80 MHz
     *
     * For PWR_REGULATOR_VOLTAGE_SCALE1:
     * 1. HCLK <= 16 MHz -> FLASH_LATENCY_0
     * 2. HCLK <= 32 MHz -> FLASH_LATENCY_1
     * 3. HCLK <= 48 MHz -> FLASH_LATENCY_2
     * 4. HCLK <= 64 MHz -> FLASH_LATENCY_3
     * 5. HCLK <= 80 MHz -> FLASH_LATENCY_4
     */
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
    uint32_t cnt = 0;

    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET);
    HAL_GPIO_Init(GPIOE, &gpio_init);

    LOG("SYSCLK: %d Hz", HAL_RCC_GetSysClockFreq());
    LOG("HCLK  : %d Hz", HAL_RCC_GetHCLKFreq());
    LOG("PCLK1 : %d Hz", HAL_RCC_GetPCLK1Freq());
    LOG("PCLK2 : %d Hz", HAL_RCC_GetPCLK2Freq());

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

    SystemClock_Config();
    SystemCoreClockUpdate();

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
