cmake_minimum_required(VERSION 3.20.1)
project(moonlight)

if(NOT ARCH)
	message(FATAL_ERROR "Unspecified target architecture! Supported platforms are: i386")
endif()
string(TOLOWER ${ARCH} ARCH)
set_property(CACHE ARCH PROPERTY STRINGS "i386")

if(NOT ${CMAKE_CROSSCOMPILING})
	message(FATAL_ERROR "Cross compiler required!")
endif()

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts)

set(KERNEL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/kernel/include)
set(LIBC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libc/include)

add_subdirectory(libc)
add_subdirectory(kernel)

