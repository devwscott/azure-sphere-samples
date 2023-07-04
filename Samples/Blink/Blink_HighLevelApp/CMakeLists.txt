#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required (VERSION 3.20)

project (Blink)

azsphere_configure_tools(TOOLS_REVISION "22.11")
azsphere_configure_api(TARGET_API_SET "15")


# Create executable
# add_executable (${PROJECT_NAME} main.c)
# add_executable (${PROJECT_NAME} main.cpp Blink.cpp)
add_executable (${PROJECT_NAME} main.cpp Blink.cpp
                Coordinator.cpp
                WifiStation.cpp
                MqttClient.cpp
                BitReadWriter.cpp
                descriptor.cpp
                unknown_descriptor.cpp
                light_control_descriptor.cpp

                ./thirdparty/MQTT-C/src/mqtt.c
                ./thirdparty/MQTT-C/src/mqtt_pal.c
                ./thirdparty/jsonlib/src/parson.c

                )


# # Include Folders
# include_directories(${PROJECT_NAME} PUBLIC
#                     ./uClibcpp/include)

include_directories(${PROJECT_NAME} PUBLIC
                    ./thirdparty/MQTT-C/include
                    ./thirdparty/mbedtls/include
                    ./thirdparty/jsonlib/include
                    ./
                    ./modules/MqttMessage/include
                    )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-rtti")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

# cflags for MQTT-C
add_compile_definitions(MQTT_USE_MBEDTLS)

# cflags for mbedtls
add_compile_definitions(MBEDTLS_CONFIG_FILE="mbedtls_user_config.h")

# disabled the unnecessary configs for mbedtls 
set(ENABLE_PROGRAMS OFF CACHE BOOL "Disabled for ENABLE_PROGRAMS" FORCE)
set(ENABLE_TESTING OFF CACHE BOOL "Disabled for ENABLE_TESTING" FORCE)

add_subdirectory (uClibcpp)

add_subdirectory (thirdparty/mbedtls)
add_subdirectory (modules/MqttMessage)


# target_link_libraries (${PROJECT_NAME} applibs pthread gcc_s c)
target_link_libraries (${PROJECT_NAME} applibs pthread gcc_s c mbedtls mbedcrypto mbedx509 mqttmessage uClibcpp)

# azsphere_target_hardware_definition(${PROJECT_NAME} TARGET_DIRECTORY "HardwareDefinitions/mt3620_rdb" TARGET_DEFINITION "template_appliance.json")
azsphere_target_hardware_definition(${PROJECT_NAME} TARGET_DIRECTORY "HardwareDefinitions/avnet_mt3620_sk" TARGET_DEFINITION "template_appliance.json")

# azsphere_target_add_image_package(${PROJECT_NAME})
azsphere_target_add_image_package(${PROJECT_NAME} RESOURCE_FILES "ca_cert.der")
