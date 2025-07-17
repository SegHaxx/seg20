cmake_minimum_required(VERSION 3.13)

#add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Os>")

include(pico_sdk_import.cmake)

project(seg20_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(i8080_tests i8080_tests.c)
add_executable(seg20 seg20.c)

# pull in common dependencies
target_link_libraries(i8080_tests pico_stdlib)
target_link_libraries(seg20 pico_stdlib)

# enable usb output, disable uart output
pico_enable_stdio_usb(i8080_tests 1)
pico_enable_stdio_uart(i8080_tests 0)
pico_enable_stdio_usb(seg20 1)
pico_enable_stdio_uart(seg20 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(i8080_tests)
pico_add_extra_outputs(seg20)

include_directories(portable)
