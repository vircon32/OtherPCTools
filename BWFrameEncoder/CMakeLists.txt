# minimum version of CMake that can parse this file
cmake_minimum_required(VERSION 2.8.12...3.19.1)

# configure some flags for compatibility across CMake versions
if(POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW) # Ignore Quoted Arguments
endif()
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW) # Root Variables
endif()

# -----------------------------------------------------
#   BASIC PROJECT CONFIGURATION
# -----------------------------------------------------

# These general project variables should be cached
set(ENCODER_DIR "FrameEncoder/"
    CACHE PATH "The path to the assembler sources.")
set(DATA_DIR "Data/"
    CACHE PATH "The path to data files to add when installing.")
set(RUNTIME_DIR "Runtime/"
    CACHE PATH "The path to the runtime files (DLLs).")

# Configure find_* commands to never try to find Mac frameworks, only packages
set(CMAKE_FIND_FRAMEWORK CACHE STRING "NEVER")

# By default, project configuration will be Release
# (must be done before project() statement)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# -----------------------------------------------------
#   DEFINE THE PROJECT
# -----------------------------------------------------

# Declare the project
project("Vircon32")

# Define version
set(PROJECT_VERSION_MAJOR 24)
set(PROJECT_VERSION_MINOR 12)
set(PROJECT_VERSION_PATCH 30)

# Set names for final executable
set(ENCODER_BINARY_NAME "encodebwframe")


# -----------------------------------------------------
#   IDENTIFY HOST ENVIRONMENT
# -----------------------------------------------------

# Detect architecture bits
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(TARGET_BITS "64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(TARGET_BITS "32")
endif()

# Detect operating system
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(TARGET_OS "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(TARGET_OS "linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(TARGET_OS "mac")
endif()

# -----------------------------------------------------
#   BUILD FLAGS / CONFIGURATION
# -----------------------------------------------------

# Set compilation flags for C and C++
if (MINGW OR TARGET_OS STREQUAL "linux")
    set(cxx_flags "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -Wno-unused-parameter")
    set(c_flags "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter")
elseif (MSVC)
    set(cxx_flags "${CMAKE_CXX_FLAGS} /W3 /EHsc /MP /GS /wd4267 /wd4244")
    set(c_flags "${CMAKE_C_FLAGS} /W3 /MP /GS /wd4267 /wd4244")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
endif()

set(CMAKE_CXX_FLAGS "${cxx_flags}"
    CACHE STRING "Flags used by the compiler during all build types." FORCE)
set(CMAKE_C_FLAGS "${c_flags}"
    CACHE STRING "Flags used by the compiler during all build types." FORCE)

# Mark executables as debug (*_d) when debug build is selected
if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" OR CMAKE_CFG_INTDIR STREQUAL "Debug" OR CMAKE_CFG_INTDIR STREQUAL "RelWithDebInfo")
	set(IS_DEBUG TRUE)
	set(CMAKE_DEBUG_POSTFIX "_d")
	if (TARGET_OS STREQUAL "windows")
		set(CMAKE_EXECUTABLE_SUFFIX "_d.exe")
	else()
		set(CMAKE_EXECUTABLE_SUFFIX "_d")
	endif()
else()
	set(IS_DEBUG FALSE)
endif()

# -----------------------------------------------------
#   FINDING ALL PROJECT DEPENDENCIES
# -----------------------------------------------------

# These are treated as independent (they don't depend on anything else)
find_library(PNG_LIBRARY NAMES png REQUIRED)

# -----------------------------------------------------
#   SHOW BUILD INFORMATION IN PRETTY FORMAT
# -----------------------------------------------------

message(STATUS "******** BWFrameEncoder ********")

# Show basic build properties
if(NOT TARGET_OS STREQUAL "mac")
    message(STATUS "Target OS: ${TARGET_OS} ${TARGET_BITS}bit")
else()
    message(STATUS "Target OS: ${TARGET_OS} ${TARGET_BITS}bit (SDK: ${CMAKE_OSX_SYSROOT})")
endif()

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# This function shows the status for a dependency in pretty format
function(show_dependency_status OUTPUT_NAME NAME)
    if(${NAME}_FOUND OR ${NAME}_LIBRARY)
        if(${NAME}_BUNDLED)
            message(STATUS " * ${OUTPUT_NAME} not found (using bundled version)")
        else()
            message(STATUS " * ${OUTPUT_NAME} found")
        endif()
    else()
        message(STATUS " * ${OUTPUT_NAME} not found")
    endif()
endfunction()

# Now use the function to show status of all dependencies
message(STATUS "System Dependencies:")
show_dependency_status("LIBPNG" PNG)

# -----------------------------------------------------
#   FOLDERS FOR INCLUDES
# -----------------------------------------------------

# Define folders where the compiler should look for includes
set(ALL_INCLUDE_DIRS
    ${ENCODER_DIR})

include_directories(${ALL_INCLUDE_DIRS})

# -----------------------------------------------------
#   LINKED LIBRARIES FILES
# -----------------------------------------------------

# Libraries to link with the encoder
set(ENCODER_LIBS
    ${PNG_LIBRARY}
    ${CMAKE_DL_LIBS})

# -----------------------------------------------------
#   SOURCE FILES
# -----------------------------------------------------

# Source files to compile for the encoder
set(ENCODER_SRC
    ${ENCODER_DIR}/Encode.cpp
    ${ENCODER_DIR}/Globals.cpp
    ${ENCODER_DIR}/Main.cpp
    ${ENCODER_DIR}/PNGImage.cpp)

# -----------------------------------------------------
#   EXECUTABLES
# -----------------------------------------------------

# Define the final executable; this tool needs C++17
# to have a portable way to iterate over folders
add_executable(${ENCODER_BINARY_NAME} ${ENCODER_SRC})
set_property(TARGET ${ENCODER_BINARY_NAME} PROPERTY CXX_STANDARD 17)

# Libraries to link to the executable
target_link_libraries(${ENCODER_BINARY_NAME} ${ENCODER_LIBS})

# -----------------------------------------------------
#   DEFINE THE INSTALL PROCESS
# -----------------------------------------------------

if(TARGET_OS STREQUAL "windows")
    # Install all binaries
    install(TARGETS
        ${ENCODER_BINARY_NAME}
        RUNTIME
        COMPONENT binaries
        DESTINATION OtherTools/BWFrameEncoder)
    
    # Copy the readme file
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Readme.txt
        DESTINATION OtherTools/BWFrameEncoder)
    
    # On Windows, copy the needed runtime files (DLLs)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${RUNTIME_DIR}/
        DESTINATION OtherTools/BWFrameEncoder)
else()
    # Install all binaries
    install(TARGETS
        ${ENCODER_BINARY_NAME}
        RUNTIME
        COMPONENT binaries
        DESTINATION ${CMAKE_PROJECT_NAME}/OtherTools/BWFrameEncoder)
    
    # Copy the readme file
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Readme.txt
        DESTINATION ${CMAKE_PROJECT_NAME}/OtherTools/BWFrameEncoder)
endif()

# On Linux or Mac we will need to set install permissions
if(TARGET_OS STREQUAL "linux" OR TARGET_OS STREQUAL "mac")
    # Allow encoder to create files in its own folder
    install(CODE "execute_process(COMMAND chmod 777 ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/OtherTools/BWFrameEncoder)")
endif()
