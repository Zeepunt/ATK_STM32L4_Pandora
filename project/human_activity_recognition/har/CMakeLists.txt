#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(har_incs
    "${CMAKE_CURRENT_SOURCE_DIR}/AI/App"
    "${CMAKE_CURRENT_SOURCE_DIR}/ai_generated_network"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/AI/Inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/AI/Misc/Inc"
)

set(har_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/AI/App/app_x-cube-ai.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/AI/App/network_weights.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/AI/App/user_init.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/ai_generated_network/network.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/ai_generated_network/network_data.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/AI/Misc/Src/ai_device_adaptor.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/AI/Misc/Src/aiTestHelper_ST_AI.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/AI/Misc/Src/aiTestUtility.c"
)

set(har_libs
    "${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/AI/Lib/NetworkRuntime1210_CM4_GCC.a"
    "m"
)

component_register(
    SRCS
        ${har_srcs}
    INCLUDE_DIRS
        ${har_incs}
    LIBS
        ${har_libs}
    REQUIRES
        "CMSIS"
        "stm32l4xx-hal-driver"
)

target_compile_definitions(${COMPONENT_LIB}
    PRIVATE
        "HAVE_NETWORK_INFO"
)