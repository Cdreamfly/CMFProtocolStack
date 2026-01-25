cmake_minimum_required(VERSION 3.22)
project(CMFProtocolStack)

set(CMAKE_C_STANDARD 17)

add_definitions("-g -O2 -Wall -fPIC")

file(GLOB SRC_FILES
        "${PROJECT_SOURCE_DIR}/src/*"
        "${PROJECT_SOURCE_DIR}/src/net/*"
        "${PROJECT_SOURCE_DIR}/src/drv/*"
)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(CMFProtocolStack main.c ${SRC_FILES})

target_link_libraries(CMFProtocolStack -lpcap)