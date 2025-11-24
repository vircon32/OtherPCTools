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
set(CONVERTER_DIR "./"
    CACHE PATH "The path to the converter sources.")

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
set(PROJECT_VERSION_MAJOR 25)
set(PROJECT_VERSION_MINOR 11)
set(PROJECT_VERSION_PATCH 24)

# Set names for final executable
set(CONVERTER_BINARY_NAME "chip8tovircon32")


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

# (this program has no dependencies)

# -----------------------------------------------------
#   SHOW BUILD INFORMATION IN PRETTY FORMAT
# -----------------------------------------------------

message(STATUS "******** Chip8ToVircon32 ********")

# Show basic build properties
if(NOT TARGET_OS STREQUAL "mac")
    message(STATUS "Target OS: ${TARGET_OS} ${TARGET_BITS}bit")
else()
    message(STATUS "Target OS: ${TARGET_OS} ${TARGET_BITS}bit (SDK: ${CMAKE_OSX_SYSROOT})")
endif()

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# -----------------------------------------------------
#   FOLDERS FOR INCLUDES
# -----------------------------------------------------

# Define folders where the compiler should look for includes
set(ALL_INCLUDE_DIRS
    ${CONVERTER_DIR})

include_directories(${ALL_INCLUDE_DIRS})

# -----------------------------------------------------
#   LINKED LIBRARIES FILES
# -----------------------------------------------------

# Libraries to link with the converter
set(CONVERTER_LIBS
    ${CMAKE_DL_LIBS})

# -----------------------------------------------------
#   SOURCE FILES
# -----------------------------------------------------

# Source files to compile for the converter
set(CONVERTER_SRC
    ${CONVERTER_DIR}/Chip8ToVircon32.cpp)

# -----------------------------------------------------
#   EXECUTABLES
# -----------------------------------------------------

# Define the final executable
add_executable(${CONVERTER_BINARY_NAME} ${CONVERTER_SRC})
set_property(TARGET ${CONVERTER_BINARY_NAME} PROPERTY CXX_STANDARD 11)

# Libraries to link to the executable
target_link_libraries(${CONVERTER_BINARY_NAME} ${CONVERTER_LIBS})

# -----------------------------------------------------
#   DEFINE THE INSTALL PROCESS
# -----------------------------------------------------

if(TARGET_OS STREQUAL "windows")
    # Install all binaries
    install(TARGETS
        ${CONVERTER_BINARY_NAME}
        RUNTIME
        COMPONENT binaries
        DESTINATION OtherTools/Chip8ToVircon32)
else()
    # Install all binaries
    install(TARGETS
        ${CONVERTER_BINARY_NAME}
        RUNTIME
        COMPONENT binaries
        DESTINATION ${CMAKE_PROJECT_NAME}/OtherTools/Chip8ToVircon32)
endif()

# On Linux or Mac we will need to set install permissions
if(TARGET_OS STREQUAL "linux" OR TARGET_OS STREQUAL "mac")
    # Allow converter to create files in its own folder
    install(CODE "execute_process(COMMAND chmod 777 ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/OtherTools/Chip8ToVircon32)")
endif()
