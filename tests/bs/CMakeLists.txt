cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME bs)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS  "bs.cpp")

if(NOT TARGET ${PROJECT_NAME})
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
    target_include_directories(${PROJECT_NAME} PUBLIC libBuild/hdr)
    target_link_libraries(${PROJECT_NAME} PRIVATE eUtil)
    add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
endif()