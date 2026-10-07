#
# toolchain.cmake
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

# 可配置选项
set(TOOLCHAIN "gcc" CACHE STRING "Compiler: gcc, armclang")
set_property(CACHE TOOLCHAIN PROPERTY STRINGS gcc armclang)

set(TOOLCHAIN_BIN_DIR "" CACHE PATH "Compiler bin directoy")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(ARCH_FLAGS
    "-mcpu=cortex-m4"
    "-mthumb"
    "-mfpu=fpv4-sp-d16"
    "-mfloat-abi=hard"
)

if(TOOLCHAIN STREQUAL "gcc")
    if(TOOLCHAIN_BIN_DIR)
        set(toolchain_prefix "${TOOLCHAIN_BIN_DIR}/arm-none-eabi-")
    else()
        set(toolchain_prefix "arm-none-eabi-")
    endif()

    set(CMAKE_C_COMPILER   "${toolchain_prefix}gcc")
    set(CMAKE_CXX_COMPILER "${toolchain_prefix}g++")
    set(CMAKE_ASM_COMPILER "${toolchain_prefix}gcc")
    set(CMAKE_SIZE         "${toolchain_prefix}size")

    set(cc_flags
        ${ARCH_FLAGS}
        "-fdata-sections"
        "-ffunction-sections"
    )

    set(link_flags
        ${ARCH_FLAGS}
    )
elseif(TOOLCHAIN STREQUAL "armclang")
    if(TOOLCHAIN_BIN_DIR)
        set(toolchain_prefix "${TOOLCHAIN_BIN_DIR}/")
    else()
        set(toolchain_prefix "")
    endif()

    set(CMAKE_C_COMPILER   "${toolchain_prefix}armclang")
    set(CMAKE_CXX_COMPILER "${toolchain_prefix}armclang++")
    set(CMAKE_ASM_COMPILER "${toolchain_prefix}armclang")
    set(CMAKE_OBJCOPY      "${toolchain_prefix}fromelf")
    set(CMAKE_SIZE         "${toolchain_prefix}fromelf")

    set(cc_flags
        "--target=arm-arm-none-eabi"
        ${ARCH_FLAGS}
        "-fdata-sections"
        "-ffunction-sections"
        "-funsigned-char"
        "-fshort-enums"
        "-fshort-wchar"
        "-fno-rtti"
    )

    set(link_flags
        "--cpu=Cortex-M4.fp"
    )
else()
    message(FATAL_ERROR "Unknown TOOLCHAIN='${TOOLCHAIN}'. Supported: gcc, armclang")
endif()

add_compile_options(${cc_flags})
add_link_options(${link_flags})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_ASM_STANDARD 11)

# cmake 探测编译器时会尝试试编译
# STATIC_LIBRARY 表示 试编译时不要链接成可执行文件, 只编译成静态库
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# NEVER: 不去 CMAKE_FIND_ROOT_PATH 下寻找
# ONLY : 只去 CMAKE_FIND_ROOT_PATH 下寻找
# BOTH : 在 CMAKE_FIND_ROOT_PATH 和宿主机默认路径 下寻找
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)