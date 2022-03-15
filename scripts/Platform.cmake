cmake_minimum_required(VERSION 3.20.1)

function(load_platform PLATFORM)
  # Obtain sources for the platform
  file(GLOB PLATFORM_SOURCES "${PLATFORM}/*.c" "${PLATFORM}/*.asm")
 
  # Load flags associated with platform
  #include("${PLATFORM}/flags.cmake")
 
  # Export output variables
  set(PLATFORM_SOURCES ${PLATFORM_SOURCES} PARENT_SCOPE)
  #set(PLATFORM_LAYOUT "${PLATFORM}/layout.ld" PARENT_SCOPE)
  set(PLATFORM_C_FLAGS ${PLATFORM_C_FLAGS} PARENT_SCOPE)
  set(PLATFORM_ASM_FLAGS ${PLATFORM_ASM_FLAGS} PARENT_SCOPE)
endfunction(load_platform)