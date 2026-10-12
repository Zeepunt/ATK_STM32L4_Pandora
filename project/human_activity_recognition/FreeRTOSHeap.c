/**
 * FreeRTOSHeap.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdint.h>
#include "FreeRTOSConfig.h"

#if ( configAPPLICATION_ALLOCATED_HEAP == 1 )
__attribute__((section(".freertos_heap"), aligned(8), used)) uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif
