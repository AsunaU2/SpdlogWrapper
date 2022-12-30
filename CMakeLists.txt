cmake_minimum_required(VERSION 3.21)
project(spdlogWrapper)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS -pthread)
endif()

include_directories(
        include/
        include/spdlogWrapper/
        include/spdlogWrapper/wrapper/
        include/spdlogWrapper/spdlog/
)

add_executable(spdlogWrapper
        src/main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(spdlogWrapper PRIVATE Threads::Threads)