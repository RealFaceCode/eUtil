cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME eUtil)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

if(NOT DEFINED PARENT_SET)
    set(PARENT_SET TRUE)
    set(BUILD_EXAMPLES $ENV{eUtil_BUILD_EXAMPLES})
    set(BUILD_TESTING BUILD_EXAMPLES)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 0)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES 0)

if(${BUILD_EXAMPLES})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS  "libBuild/src/*.cpp")

if(NOT TARGET ${PROJECT_NAME})
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
    target_include_directories(${PROJECT_NAME} PUBLIC libBuild/hdr)
endif()

message(STATUS "eUtil build examples: " ${BUILD_EXAMPLES})
message(STATUS "eUtil build tests: " ${BUILD_TESTING})

if(${BUILD_EXAMPLES})
    add_subdirectory(buildtest)
endif()

if(${BUILD_TESTING})
enable_testing()
    add_subdirectory(tests)
endif()