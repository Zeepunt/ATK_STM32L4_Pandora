#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(stm32l4xx_hal_driver_incs
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc/Legacy"
)

set(stm32l4xx_hal_driver_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_cortex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_dma.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_dma_ex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_flash.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_flash_ex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_flash_ramfunc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_gpio.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_pwr.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_pwr_ex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_rcc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_rcc_ex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_uart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_uart_ex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_usart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32l4xx_hal_usart_ex.c"
)

component_register(
    SRCS
        ${stm32l4xx_hal_driver_srcs}
        "${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_msp.c"
    INCLUDE_DIRS
        ${stm32l4xx_hal_driver_incs}
    REQUIRES
        "CMSIS"
        "cmsis-device-l4"
)

target_compile_definitions(${COMPONENT_LIB}
    PUBLIC
        "USE_HAL_DRIVER"
)