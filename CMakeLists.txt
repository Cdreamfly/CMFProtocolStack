cmake_minimum_required(VERSION 3.22)
project(pcap)

set(CMAKE_C_STANDARD 23)

add_definitions("-g -O2 -Wall -Werror -fPIC")

file(GLOB SRC_FILES
        "${PROJECT_SOURCE_DIR}/include/*"
        "${PROJECT_SOURCE_DIR}/src/*"
)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(pcap main.c ${SRC_FILES})

target_link_libraries(pcap -lpcap)