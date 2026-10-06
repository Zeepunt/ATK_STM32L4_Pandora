#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(STM32_CHIP "STM32L475xx" CACHE STRING "Target STM32 chip")

component_register(
    SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/system_stm32l4xx.c"
    INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/Include"
    REQUIRES
        "CMSIS"
)

target_compile_definitions(${COMPONENT_LIB}
    PUBLIC
        ${STM32_CHIP}
)