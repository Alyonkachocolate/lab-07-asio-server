#cmake_minimum_required(VERSION 3.12)
#
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
#option(BUILD_TESTS "Build tests" ON)
#option(BUILD_DOCS "Build documentation" OFF)
#option(BUILD_COVERAGE "Build code coverage" OFF)
#
#set(
#        HUNTER_CACHE_SERVERS
#        "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
#        CACHE STRING "Default cache server"
#)
#
#include("cmake/HunterGate.cmake")
#
#HunterGate(
#        URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
#        SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
#)
#
#project(lab-07-asio-server VERSION 0.1.0)
#string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
#string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
#string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")
#
#hunter_add_package(GTest)
#find_package(GTest CONFIG REQUIRED)
#hunter_add_package(Boost COMPONENTS log thread system)
#find_package(Boost CONFIG REQUIRED log thread system)
#
#add_library(${PROJECT_NAME} STATIC
#        ${CMAKE_CURRENT_SOURCE_DIR}/sources/server.cpp
#        )
#
#add_executable(tests
#        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
#        )
#add_executable(demo
#        ${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp
#        )
#
#target_include_directories(${PROJECT_NAME}
#        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
#        )
#target_include_directories(tests
#        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
#        )
#target_include_directories(demo
#        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
#        )
#
#target_link_libraries(tests GTest::gtest_main ${PROJECT_NAME})
#target_link_libraries(${PROJECT_NAME} Boost::log Boost::thread Boost::system)
#target_link_libraries(demo ${PROJECT_NAME} Boost::log Boost::thread
#        Boost::system)
#
#if (BUILD_COVERAGE)
#    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
#    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/coverage/cmake")
#    find_package(codecov)
#    add_coverage(${PROJECT_NAME})
#    add_coverage(tests)
#    coverage_evaluate()
#endif ()

cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
        HUNTER_CACHE_SERVERS
        "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
        CACHE STRING "Default cache server"
)

include("cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
        SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
)

project(server_lib)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)
hunter_add_package(Boost COMPONENTS log thread system)
find_package(Boost CONFIG REQUIRED log thread system)

add_library(server_lib STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/server.cpp
        )
add_executable(tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
        )
add_executable(demo
        ${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp
        )

target_include_directories(server_lib
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        )
target_include_directories(tests
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        )
target_include_directories(demo
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(tests GTest::gtest_main server_lib)
target_link_libraries(server_lib Boost::log Boost::thread Boost::system)
target_link_libraries(demo server_lib Boost::log Boost::thread Boost::system)

enable_testing()
add_test(NAME unit_tests COMMAND tests)

if (BUILD_COVERAGE)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/coverage/cmake")
    find_package(codecov)
    add_coverage(${PROJECT_NAME})
    add_coverage(tests)
    coverage_evaluate()
endif ()
