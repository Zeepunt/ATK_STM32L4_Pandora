/**
 * bsp_i2c.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int bsp_i2c_init(void);

int bsp_i2c_write(uint16_t dev_addr, const uint8_t *buf, int len);
int bsp_i2c_read(uint16_t dev_addr, uint8_t *buf, int len);

int bsp_i2c_reg_write_byte(uint16_t dev_addr, uint16_t reg_addr, const uint8_t *byte);
int bsp_i2c_reg_read_byte(uint16_t dev_addr, uint16_t reg_addr, uint8_t *byte);

int bsp_i2c_reg_write_buf(uint16_t dev_addr, uint16_t reg_addr, const uint8_t *buf, int len);
int bsp_i2c_reg_read_buf(uint16_t dev_addr, uint16_t reg_addr, uint8_t *buf, int len);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_I2C_H__ */
