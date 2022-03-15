cmake_minimum_required(VERSION 3.20.1)
project(libc C)

include(BuildType)

# C Standard Library
list(APPEND SOURCE
	"${CMAKE_CURRENT_SOURCE_DIR}/assert/assert.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/errno/errno.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/stdio/printf.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/stdio/putchar.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/stdio/puts.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/stdio/vprintf.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/stdlib/abort.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/memchr.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/memcmp.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/memcpy.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/memmove.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/memset.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/strcat.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/strchr.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/strcmp.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/strcpy.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/strlen.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/strncat.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/strncmp.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/strncpy.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/string/strrchr.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/sys/util/itoa.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/sys/util/utoa.c")

# Stack Smashing Protector Library
list(APPEND SOURCE
	"${CMAKE_CURRENT_SOURCE_DIR}/libssp/stackchk.c")

add_library(stdk STATIC ${SOURCE})

target_include_directories(stdk
	PUBLIC include
	PRIVATE ${KERNEL_INCLUDE_DIR})

target_compile_definitions(stdk
	PRIVATE
		__is_libc
		__is_libk)

set(DEBUG_OPTIONS -fno-omit-frame-pointer -fno-eliminate-unused-debug-symbols)
target_compile_options(stdk BEFORE
	PRIVATE
		-ffreestanding
		$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>)

target_link_options(stdk BEFORE
	PRIVATE -nostdlib)

install(TARGETS stdk
	CONFIGURATIONS Debug
	RUNTIME DESTINATION Debug/bin
	LIBRARY DESTINATION Debug/lib
	ARCHIVE DESTINATION Debug/lib)

install(TARGETS stdk
	CONFIGURATIONS Release
	RUNTIME DESTINATION Release/bin
	LIBRARY DESTINATION Release/lib
	ARCHIVE DESTINATION Release/lib)

install(TARGETS stdk
	RUNTIME DESTINATION ${CMAKE_SYSROOT}/usr/bin
	LIBRARY DESTINATION ${CMAKE_SYSROOT}/usr/lib
	ARCHIVE DESTINATION ${CMAKE_SYSROOT}/usr/lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_SYSROOT}/usr/include
	FILES_MATCHING PATTERN "*.h*")

set(LIBC_LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)