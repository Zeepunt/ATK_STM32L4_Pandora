/**
 * drv_icm20608.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __DRV_ICM20608_H__
#define __DRV_ICM20608_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int drv_icm20608_gyroscope_get(int16_t *x, int16_t *y, int16_t *z);

int drv_icm20608_accelerometer_get(int16_t *x, int16_t *y, int16_t *z);

int drv_icm20608_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_ICM20608_H__ */
