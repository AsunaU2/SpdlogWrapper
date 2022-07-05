cmake_minimum_required(VERSION 3.21)
project(spdlogWrapper)

set(CMAKE_CXX_STANDARD 11)

include_directories(
        include/
        include/spdlogWrapper/
        include/spdlogWrapper/wrapper/
        include/spdlogWrapper/spdlog/
)

add_executable(spdlogWrapper src/main.cpp include/spdlogWrapper/wrapper/spdlog-api.h include/spdlogWrapper/wrapper/sink_manager.h)
