cmake_minimum_required(VERSION 3.20)


project(TelemetryPlatform)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
telemetry
src/main.cpp
)

target_include_directories(telemetry PRIVATE include)
