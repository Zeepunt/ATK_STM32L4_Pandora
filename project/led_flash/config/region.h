/**
 * region.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __REGION_H__
#define __REGION_H__

#define __IROM_BASE    0x08000000
#define __IROM_SIZE    0x00080000

#define __RAM1_BASE    0x20000000
#define __RAM1_SIZE    0x00018000
#define __RAM1_END     (__RAM1_BASE + __RAM1_SIZE)

/* 栈提供给中断服务函数使用 (MSP), 4 KB */
#define __STACK_SIZE   0x1000

/* 堆提供给内存分配使用 (malloc), 48 KB */
#define __HEAP_SIZE    0xC000

#endif /* __REGION_H__ */
