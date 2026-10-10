/**
 * bsp_common.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __BSP_COMMON_H__
#define __BSP_COMMON_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BSP_LOG_COLOR_GRAY      "\033[90m"
#define BSP_LOG_COLOR_GREEN     "\033[32m"
#define BSP_LOG_COLOR_YELLOW    "\033[33m"
#define BSP_LOG_COLOR_RED       "\033[31m"
#define BSP_LOG_COLOR_END       "\033[0m"

#define BSP_LOGD(TAG, fmt, ...)    printf(BSP_LOG_COLOR_GRAY "[D] %s: " fmt "\n" BSP_LOG_COLOR_END, TAG, ##__VA_ARGS__)
#define BSP_LOGI(TAG, fmt, ...)    printf(BSP_LOG_COLOR_GREEN "[I] %s: " fmt "\n" BSP_LOG_COLOR_END, TAG, ##__VA_ARGS__)
#define BSP_LOGW(TAG, fmt, ...)    printf(BSP_LOG_COLOR_YELLOW "[W] %s: " fmt "\n" BSP_LOG_COLOR_END, TAG, ##__VA_ARGS__)
#define BSP_LOGE(TAG, fmt, ...)    printf(BSP_LOG_COLOR_RED "[E] %s(%d): " fmt "\n" BSP_LOG_COLOR_END, TAG, __LINE__, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* __BSP_COMMON_H__ */