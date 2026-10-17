/**
 * board_common.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __BOARD_COMMON_H__
#define __BOARD_COMMON_H__

#include <stdio.h>

#include "autoconf.h"

#include "stm32l4xx.h"

#ifdef CONFIG_SDK_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_LOG_COLOR_GRAY      "\033[90m"
#define BOARD_LOG_COLOR_GREEN     "\033[32m"
#define BOARD_LOG_COLOR_YELLOW    "\033[33m"
#define BOARD_LOG_COLOR_RED       "\033[31m"
#define BOARD_LOG_COLOR_END       "\033[0m"

#define BOARD_LOGD(TAG, fmt, ...)    printf(BOARD_LOG_COLOR_GRAY "[D] %s: " fmt "\n" BOARD_LOG_COLOR_END, TAG, ##__VA_ARGS__)
#define BOARD_LOGI(TAG, fmt, ...)    printf(BOARD_LOG_COLOR_GREEN "[I] %s: " fmt "\n" BOARD_LOG_COLOR_END, TAG, ##__VA_ARGS__)
#define BOARD_LOGW(TAG, fmt, ...)    printf(BOARD_LOG_COLOR_YELLOW "[W] %s: " fmt "\n" BOARD_LOG_COLOR_END, TAG, ##__VA_ARGS__)
#define BOARD_LOGE(TAG, fmt, ...)    printf(BOARD_LOG_COLOR_RED "[E] %s(%d): " fmt "\n" BOARD_LOG_COLOR_END, TAG, __LINE__, ##__VA_ARGS__)

#define board_delay_ms_hal(ms)       HAL_Delay(ms)

#ifdef CONFIG_SDK_FREERTOS
#define board_delay_ms_os(ms)        vTaskDelay(pdMS_TO_TICKS(ms))
#else
#define board_delay_ms_os(ms)        HAL_Delay(ms)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_COMMON_H__ */
