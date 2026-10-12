#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(bsp_incs
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(bsp_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/source/bsp_uart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/bsp_i2c.c"
)

component_register(
    SRCS
        ${bsp_srcs}
    INCLUDE_DIRS
        ${bsp_incs}
    REQUIRES
        "stm32l4xx-hal-driver"
        "FreeRTOS"
)
