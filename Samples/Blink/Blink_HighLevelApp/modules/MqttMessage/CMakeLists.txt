#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required (VERSION 3.20)

set(project_name mqttmessage)
project(${project_name} CXX)


set(rootdir ${CMAKE_SOURCE_DIR}/modules/MqttMessage)
set(sources 
	${rootdir}/src/MQTTMessage.cpp
    ${rootdir}/src/MQTTLightControlMessage.cpp
    ${rootdir}/src/MQTTLightStatusMessage.cpp
    ${rootdir}/src/MQTTMessageLightCodec.cpp
    ${rootdir}/src/MQTTMessageCodecFactoryImpl.cpp
)


include_directories(
	${rootdir}/include
)

add_library(${project_name} STATIC ${sources})

