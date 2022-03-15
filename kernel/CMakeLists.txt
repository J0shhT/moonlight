cmake_minimum_required(VERSION 3.20.1)
project(kernel C ASM_NASM)
include(BuildType)

# C Runtime Constructors + Deconstructors
if(ARCH STREQUAL "i386")
	# Find the compiler's crtbegin and crtend
	execute_process(COMMAND ${CMAKE_C_COMPILER} "-print-file-name=crtbegin.o" OUTPUT_VARIABLE CRTBEGIN_OBJ OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND ${CMAKE_C_COMPILER} "-print-file-name=crtend.o" OUTPUT_VARIABLE CRTEND_OBJ OUTPUT_STRIP_TRAILING_WHITESPACE)

	# Set custom linker order
	set(CRTI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/arch/i386/crti.asm)
	set(CRTI_OBJ ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/crti.asm.obj)
	set(CRTN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/arch/i386/crtn.asm)
	set(CRTN_OBJ ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/crtn.asm.obj)
	set(CRTBEGIN "${CRTI_OBJ} ${CRTBEGIN_OBJ}")
	set(CRTEND "${CRTEND_OBJ} ${CRTN_OBJ}")
	set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> ${CRTBEGIN} <OBJECTS> -o <TARGET> <LINK_LIBRARIES> ${CRTEND}")
endif()

list(APPEND SOURCE
	"${CMAKE_CURRENT_SOURCE_DIR}/boot/kinit.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/boot/kmain.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/drivers/video.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/kd/kdebug.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/ke/crash.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/rtl/kconvert.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/rtl/kmemory.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/rtl/kstring.c")

if(ARCH STREQUAL "i386")
	list(APPEND ASM_SOURCE
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/i386/boot.asm"
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/i386/gdti.asm"
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/i386/idti.asm"
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/i386/irq.asm"
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/i386/isr.asm")
	list(APPEND SOURCE
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/i386/gdt.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/i386/idt.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/i386/interrupt.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/i386/pic.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/boot/i386/timer.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/drivers/i386/serial.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/drivers/i386/vga.c")
endif()

set(TARGET_NAME ${CMAKE_PROJECT_NAME}krnl)
add_executable(${TARGET_NAME} ${ASM_SOURCE} ${SOURCE})
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(${TARGET_NAME}
	PUBLIC include
	PRIVATE ${LIBC_INCLUDE_DIR})

target_compile_definitions(${TARGET_NAME}
	PRIVATE __is_kernel)

set(DEBUG_OPTIONS -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-eliminate-unused-debug-symbols)
set(C_COMPILE_OPTIONS -ffreestanding -fstack-protector-strong $<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>)
target_compile_options(${TARGET_NAME} BEFORE
	PRIVATE
		$<$<COMPILE_LANGUAGE:C>:${C_COMPILE_OPTIONS}>)

target_link_options(${TARGET_NAME} BEFORE
	PRIVATE -nostdlib)

if(ARCH STREQUAL "i386")
	target_link_options(${TARGET_NAME} BEFORE
		PRIVATE "-T${CMAKE_CURRENT_SOURCE_DIR}/arch/i386/linker.ld")
endif()

target_link_directories(${TARGET_NAME}
	PRIVATE ${LIBC_LIBRARY_DIR})

set(DEBUG_LIBS stdkd)
set(RELEASE_LIBS stdk)
target_link_libraries(${TARGET_NAME}
	gcc
	$<$<CONFIG:DEBUG>:${DEBUG_LIBS}>
	$<$<CONFIG:RELEASE>:${RELEASE_LIBS}>)

if(ARCH STREQUAL "i386")
	add_custom_command(TARGET ${TARGET_NAME}
		PRE_LINK
		COMMAND bash "-c" "${CMAKE_ASM_NASM_COMPILER} ${CMAKE_ASM_NASM_FLAGS_INIT} -g -Fdwarf ${CRTI_SRC} -o ${CRTI_OBJ}"
		DEPENDS ${CRTI_SRC}
		COMMENT "Assembling ${CRTI_SRC} to ${CRTI_OBJ}...")
	add_custom_command(TARGET ${TARGET_NAME}
		PRE_LINK
		COMMAND bash "-c" "${CMAKE_ASM_NASM_COMPILER} ${CMAKE_ASM_NASM_FLAGS_INIT} -g -Fdwarf ${CRTN_SRC} -o ${CRTN_OBJ}"
		DEPENDS ${CRTN_SRC}
		COMMENT "Assembling ${CRTN_SRC} to ${CRTN_OBJ}...")
endif()

install(TARGETS ${TARGET_NAME}
	CONFIGURATIONS Debug
	RUNTIME DESTINATION Debug/bin
	LIBRARY DESTINATION Debug/lib
	ARCHIVE DESTINATION Debug/lib)

install(TARGETS ${TARGET_NAME}
	CONFIGURATIONS Release
	RUNTIME DESTINATION Release/bin
	LIBRARY DESTINATION Release/lib
	ARCHIVE DESTINATION Release/lib)

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CMAKE_SYSROOT}/boot)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_SYSROOT}/usr/include
	FILES_MATCHING PATTERN "*.h*")