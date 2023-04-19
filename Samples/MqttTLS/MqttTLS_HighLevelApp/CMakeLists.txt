#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required(VERSION 3.20)

project(Wifi_HighLevelApp C)

azsphere_configure_tools(TOOLS_REVISION "22.11")
azsphere_configure_api(TARGET_API_SET "15")

# add_executable(${PROJECT_NAME} main.c eventloop_timer_utilities.c)
add_executable(${PROJECT_NAME} main.c eventloop_timer_utilities.c udplog.c
                ./thirdparty/MQTT-C/src/mqtt.c
                ./thirdparty/MQTT-C/src/mqtt_pal.c
                "mqtt_utilities.c"
                "mbedtls_socket.c"
                "common.c"
                "parson.c"
                )

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
                ./thirdparty/MQTT-C/include
                ./thirdparty/mbedtls/include
                ./)

include_directories(./
                thirdparty/mbedtls/include)

                #target_link_libraries(${PROJECT_NAME} applibs gcc_s c)
# target_link_libraries(${PROJECT_NAME} applibs pthread gcc_s c)
target_link_libraries (${PROJECT_NAME} applibs pthread gcc_s c mbedtls mbedcrypto mbedx509)

# comment out the add_compile_definitions to use the standard Log_Debug
add_compile_definitions(USE_SOCKET_LOG)

add_compile_definitions(MQTT_USE_MBEDTLS)

add_compile_definitions(MBEDTLS_CONFIG_FILE="mbedtls_user_config.h")

# disabled the unnecessary configs for mbedtls 
set(ENABLE_PROGRAMS OFF CACHE BOOL "Disabled for ENABLE_PROGRAMS" FORCE)
set(ENABLE_TESTING OFF CACHE BOOL "Disabled for ENABLE_TESTING" FORCE)
add_subdirectory (thirdparty/mbedtls)

# TARGET_HARDWARE and TARGET_DEFINITION relate to the hardware definition targeted by this sample.
# When using this sample with other hardware, replace TARGET_HARDWARE with the name of that hardware.
# For example, to target the Avnet MT3620 Starter Kit, use the value "avnet_mt3620_sk".
# See https://aka.ms/azsphere-samples-hardwaredefinitions for further details on this feature.
# set(TARGET_HARDWARE "mt3620_rdb")
set(TARGET_HARDWARE "avnet_mt3620_sk")
set(TARGET_DEFINITION "sample_appliance.json")

# This finds the path to the target hardware definition, in one of two places.
# If the sample is cloned from the github repo, then the path to the target hardware is 
# "../../../HardwareDefinitions/${TARGET_HARDWARE}". If the sample is downloaded from the Microsoft
# samples browser, then the path is "HardwareDefinitions/${TARGET_HARDWARE}"
find_path(
    TARGET_DIRECTORY
    NAMES ${TARGET_DEFINITION}
    PATHS "HardwareDefinitions/${TARGET_HARDWARE}" "../../../HardwareDefinitions/${TARGET_HARDWARE}"
    NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

azsphere_target_hardware_definition(${PROJECT_NAME} TARGET_DIRECTORY "${TARGET_DIRECTORY}" TARGET_DEFINITION "${TARGET_DEFINITION}")

# azsphere_target_add_image_package(${PROJECT_NAME})
# azsphere_target_add_image_package(${PROJECT_NAME} RESOURCE_FILES "ca_wslee.crt")
azsphere_target_add_image_package(${PROJECT_NAME} RESOURCE_FILES "ca_wslee.der")
