#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(driver_incs
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(driver_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/source/drv_icm20608.c"
)

component_register(
    SRCS
        ${driver_srcs}
    INCLUDE_DIRS
        ${driver_incs}
    REQUIRES
        "bsp"
)
