/**
 * drv_common.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __DRV_COMMON_H__
#define __DRV_COMMON_H__

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRV_LOG_COLOR_GRAY      "\033[90m"
#define DRV_LOG_COLOR_GREEN     "\033[32m"
#define DRV_LOG_COLOR_YELLOW    "\033[33m"
#define DRV_LOG_COLOR_RED       "\033[31m"
#define DRV_LOG_COLOR_END       "\033[0m"

#define DRV_LOGD(TAG, fmt, ...)    printf(DRV_LOG_COLOR_GRAY "[D] %s: " fmt "\n" DRV_LOG_COLOR_END, TAG, ##__VA_ARGS__)
#define DRV_LOGI(TAG, fmt, ...)    printf(DRV_LOG_COLOR_GREEN "[I] %s: " fmt "\n" DRV_LOG_COLOR_END, TAG, ##__VA_ARGS__)
#define DRV_LOGW(TAG, fmt, ...)    printf(DRV_LOG_COLOR_YELLOW "[W] %s: " fmt "\n" DRV_LOG_COLOR_END, TAG, ##__VA_ARGS__)
#define DRV_LOGE(TAG, fmt, ...)    printf(DRV_LOG_COLOR_RED "[E] %s(%d): " fmt "\n" DRV_LOG_COLOR_END, TAG, __LINE__, ##__VA_ARGS__)

#define drv_delay_ms(ms)    vTaskDelay(pdMS_TO_TICKS(ms))

#ifdef __cplusplus
}
#endif

#endif /* __DRV_COMMON_H__ */
