cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(sensor_hub)

target_include_directories(app PRIVATE inc)
target_sources(app PRIVATE src/main.c src/wenet_service.c src/notify.c)