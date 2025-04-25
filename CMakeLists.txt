cmake_minimum_required(VERSION 3.15)

#! Check every comment after the "#!"

#! CHANGE YOUR PROJECT NAME
#  It is used as your project's main executable name.
set(PROJECT_NAME my_vector)
project(${PROJECT_NAME} C CXX) # project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 20)

##########################################################
# User configurable options of the template
##########################################################
# Note: symbols like WARNINGS_AS_ERRORS in configuration are intentionally variables
# and not CMake options --using options creates too many problems for students.

#! It is a good practice to set "WARNINGS_AS_ERRORS" ON,
#  but sometimes it creates too much trouble, so default is OFF.
set(WARNINGS_AS_ERRORS OFF)

#! Always use PVS Studio while developing.
set(ENABLE_PVS_STUDIO OFF)

#! Select appropriate sanitizers.
#  Definitely enable sanitizers while developing.
#  Disable it for the production builds and before submitting for grading.
#  Only one of the Memory (MSAN), Address (ASAN), or Thread (TSan)
#  sanitizers is applicable at the time -- the first defined.

#! UndefinedBehaviorSanitizer (UBSan).
#  Info: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
set(ENABLE_UBSan OFF)
#! AddressSanitizer -- detects use after free or after scope exit,
#  memory overflows and leaks.
#  Info: https://github.com/google/sanitizers/wiki/AddressSanitizer
set(ENABLE_ASAN OFF)
#! ThreadSanitizer -- detects data races.
#  Info: https://clang.llvm.org/docs/ThreadSanitizer.html
set(ENABLE_TSan OFF)
#! MemorySanitizer -- detects uninitialized memory reads
#  Info: https://github.com/google/sanitizers/wiki/MemorySanitizer
set(ENABLE_MSAN OFF)

#! Be default -- build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

# Warnings as errors should be imported here -- do not move this line
include(cmake/CompilerWarnings.cmake)

##########################################################
# Project files, packages, libraries and so on
##########################################################

# Main executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Header-only libraries
add_library(my_array_lib INTERFACE)
target_include_directories(my_array_lib INTERFACE include)

add_library(my_vector_lib INTERFACE)
target_include_directories(my_vector_lib INTERFACE include)

# Link libraries to main executable
target_link_libraries(${PROJECT_NAME} PRIVATE
		my_array_lib
		my_vector_lib
)

#! Add external packages
# options_parser requires boost::program_options library
#find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
#target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::system)

##########################################################
# Google Test integration
##########################################################
find_package(GTest QUIET)

if(NOT GTest_FOUND)
	message(STATUS "GTest not found, downloading via FetchContent")
	include(FetchContent)
	FetchContent_Declare(
			googletest
			GIT_REPOSITORY https://github.com/google/googletest.git
			GIT_TAG release-1.12.1
	)
	FetchContent_MakeAvailable(googletest)

	if(NOT TARGET GTest::GTest)
		add_library(GTest::GTest ALIAS gtest)
		add_library(GTest::Main ALIAS gtest_main)
	endif()
endif()

enable_testing()

add_executable(test_my_array tests/test_my_array.cpp)
target_link_libraries(test_my_array PRIVATE
		my_array_lib
		GTest::GTest
		GTest::Main
)
add_test(NAME test_my_array COMMAND test_my_array)

add_executable(test_my_vector tests/test_my_vector.cpp)
target_link_libraries(test_my_vector PRIVATE
		my_vector_lib
		GTest::GTest
		GTest::Main
)
add_test(NAME test_my_vector COMMAND test_my_vector)
##########################################################
# Fixed CMakeLists.txt part
##########################################################

INSTALL(PROGRAMS
		$<TARGET_FILE:${PROJECT_NAME}> # ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
		DESTINATION bin)

# Define ALL_TARGETS variable to use in PVS and Sanitizers
set(ALL_TARGETS ${PROJECT_NAME})

# Include CMake setup
include(cmake/main-config.cmake)