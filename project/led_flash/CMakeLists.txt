#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

cmake_minimum_required(VERSION 3.22)

project(led_flash C ASM)

set(STM32_HAL_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config")
set(FREERTOS_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config")
set(RTT_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config")

add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/../.."
    "${CMAKE_BINARY_DIR}/sdk"
)

# 编译目标
add_executable(${PROJECT_NAME}
    "startup.c"
    "stm32l4xx_hal_msp.c"
    "retarget_syscall.c"
    "FreeRTOSHeap.c"
    "bsp/source/bsp_uart.c"
    "main.c"
)

# 统一输出后缀为 .elf
set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".elf"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/config"
    "${CMAKE_CURRENT_SOURCE_DIR}/bsp/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    "CMSIS"
    "cmsis-device-l4"
    "stm32l4xx-hal-driver"
    "FreeRTOS"
    "RTT"
)

# 链接脚本处理
if(TOOLCHAIN STREQUAL "gcc")
    set(LD_SCRIPT_IN  "${CMAKE_CURRENT_SOURCE_DIR}/link.ld.in")
    set(LD_SCRIPT     "${CMAKE_CURRENT_BINARY_DIR}/link.ld")

    add_custom_command(
        OUTPUT  ${LD_SCRIPT}
        COMMAND ${CMAKE_C_COMPILER} -E -P -x c -I${CMAKE_CURRENT_SOURCE_DIR}/config ${LD_SCRIPT_IN} -o ${LD_SCRIPT}
        DEPENDS ${LD_SCRIPT_IN}
        COMMENT "Generating ${LD_SCRIPT}"
        VERBATIM
    )
elseif(TOOLCHAIN STREQUAL "armclang")
    set(LD_SCRIPT_IN  "${CMAKE_CURRENT_SOURCE_DIR}/link.sct.in")
    set(LD_SCRIPT     "${CMAKE_CURRENT_BINARY_DIR}/link.sct")

    add_custom_command(
        OUTPUT  ${LD_SCRIPT}
        COMMAND ${CMAKE_C_COMPILER} --target=arm-arm-none-eabi -mcpu=cortex-m4 -E -P -x c -I${CMAKE_CURRENT_SOURCE_DIR}/config ${LD_SCRIPT_IN} -o ${LD_SCRIPT}
        DEPENDS ${LD_SCRIPT_IN}
        COMMENT "Generating ${LD_SCRIPT}"
        VERBATIM
    )
else()
    message(FATAL_ERROR "Unknown TOOLCHAIN='${TOOLCHAIN}'")
endif()

add_custom_target(link_script_gen DEPENDS ${LD_SCRIPT})

add_dependencies(${PROJECT_NAME} link_script_gen)

# 链接选项
if(TOOLCHAIN STREQUAL "gcc")
    target_link_options(${PROJECT_NAME} PRIVATE
        --specs=nano.specs
        --specs=nosys.specs
        -T ${LD_SCRIPT}
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map
        -Wl,--wrap=malloc
        -Wl,--wrap=_malloc_r
        -Wl,--wrap=calloc
        -Wl,--wrap=_calloc_r
        -Wl,--wrap=free
        -Wl,--wrap=_free_r
        -Wl,--gc-sections
    )
elseif(TOOLCHAIN STREQUAL "armclang")
    target_link_options(${PROJECT_NAME} PRIVATE
        --scatter=${LD_SCRIPT}
        --map
        --list=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map
        --info=sizes
        --info=totals
        --info=unused
        --info=veneers
        --strict
    )
else()
    message(FATAL_ERROR "Unknown TOOLCHAIN='${TOOLCHAIN}'")
endif()

if(TOOLCHAIN STREQUAL "gcc")
    # 生成 bin 文件
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin
        COMMENT "Generating ${PROJECT_NAME}.bin"
    )

    # 显示固件大小
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Firmware size:"
    )
elseif(TOOLCHAIN STREQUAL "armclang")
    # 生成 bin 文件
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} --bin $<TARGET_FILE:${PROJECT_NAME}> --output ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin
        COMMENT "Generating ${PROJECT_NAME}.bin"
    )

    # 显示固件大小
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_SIZE} --info=totals $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Firmware size:"
    )
endif()