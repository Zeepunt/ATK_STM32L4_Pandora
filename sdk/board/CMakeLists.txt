#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(board_incs
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/driver/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/device/include"
)

set(board_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/driver/source/drv_uart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/driver/source/drv_i2c.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/device/source/dev_icm20608.c"
)

component_register(
    SRCS
        ${board_srcs}
    INCLUDE_DIRS
        ${board_incs}
    REQUIRES
        "cmsis-device-l4"
        "stm32l4xx-hal-driver"
)

if(CONFIG_SDK_FREERTOS)
    target_link_libraries(${COMPONENT_LIB} PUBLIC "FreeRTOS")
endif()
