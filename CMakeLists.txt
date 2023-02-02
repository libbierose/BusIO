cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
#set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(BusIO C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
if (TARGET tinyusb_device)
    add_executable(BusIO
    main.cpp
            )

    # pull in common dependencies
    target_link_libraries(BusIO pico_stdlib hardware_i2c)

    # disable usb output, enable uart output
    pico_enable_stdio_usb(BusIO 0)
    pico_enable_stdio_uart(BusIO 1)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(BusIO)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building BusIO because TinyUSB submodule is not initialized in the SDK")
endif()