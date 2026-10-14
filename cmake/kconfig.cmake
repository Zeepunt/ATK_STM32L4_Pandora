#
# kconfig.cmake
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

# 找到 python
find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(KCONFIG_FILE "${CMAKE_SOURCE_DIR}/Kconfig" CACHE FILEPATH "Project Kconfig File")
set(KCONFIG_DOTCONFIG "${CMAKE_SOURCE_DIR}/.config" CACHE FILEPATH "Generated .config File")

set(KCONFIG_OUTPUT_DIR "${CMAKE_BINARY_DIR}/kconfig")
# 确保目录存在
file(MAKE_DIRECTORY "${KCONFIG_OUTPUT_DIR}")

# .config 变化时, cmake 自动重新配置
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${KCONFIG_DOTCONFIG}")

# 生成 config.cmake 和 autoconf.h (configure 阶段自动执行)
# 命令: python kconfig_to_cmake.py --kconfig <Kconfig> --config <.config> --cmake-out <...> --header-out <...>
execute_process(
    COMMAND ${Python3_EXECUTABLE}
            "${CMAKE_CURRENT_LIST_DIR}/kconfig_to_cmake.py"
            --kconfig    "${KCONFIG_FILE}"
            --config     "${KCONFIG_DOTCONFIG}"
            --cmake-out  "${KCONFIG_OUTPUT_DIR}/config.cmake"
            --header-out "${KCONFIG_OUTPUT_DIR}/autoconf.h"
    RESULT_VARIABLE ret_var
    OUTPUT_VARIABLE out_var
    ERROR_VARIABLE  err_var
)

if(NOT ret_var EQUAL 0)
    message(FATAL_ERROR
        "Kconfig generate config.cmake and autoconf.h failed\n"
        "RESULT: ${ret_var}\n"
        "OUTPUT: ${out_var}\n"
        "ERROR : ${err_var}\n")
endif()

# 导入当前作用域
include("${KCONFIG_OUTPUT_DIR}/config.cmake")

# 供 target_include_directories 使用
set(KCONFIG_INCLUDE_DIR "${KCONFIG_OUTPUT_DIR}" CACHE INTERNAL "")