cmake_minimum_required(VERSION 3.21)
project(spdlogWrapper)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS -pthread)

include_directories(
        include/
        include/spdlogWrapper/
        include/spdlogWrapper/wrapper/
        include/spdlogWrapper/spdlog/
)

add_executable(spdlogWrapper include/spdlogWrapper/wrapper/wrapper_api.h include/spdlogWrapper/wrapper/sink_manager.h src/main.cpp include/spdlogWrapper/wrapper/logger_manager.h include/spdlogWrapper/wrapper/spdlog_manager.h)

find_package(Threads REQUIRED)
target_link_libraries(spdlogWrapper PRIVATE Threads::Threads)