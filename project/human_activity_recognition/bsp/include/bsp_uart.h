/**
 * bsp_uart.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

int bsp_uart_init(void);

int bsp_uart_write(const char *buf, int len);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_UART_H__ */
