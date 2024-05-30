cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_TARGET "I2CDRIVER" CACHE STRING "Specify build target (I2CDRIVER/LINUX/PICO)")

#set(I2CCONTROLLER_SRC i2cController.cpp
#        i2cController.h)

if ("${BUILD_TARGET}" STREQUAL "I2CDRIVER")
    message("Building for i2cdriver...")
    list(APPEND I2CCONTROLLER_SRC i2cControllerI2CDriver.h i2cControllerI2CDriver.cpp i2cdriver/i2cdriver.c i2cdriver/i2cdriver.h)
elseif ("${BUILD_TARGET}" STREQUAL "LINUX")
    message("Building for Linux...")
    list(APPEND I2CCONTROLLER_SRC i2cControllerLinux.h i2cControllerLinux.cpp)
elseif ("${BUILD_TARGET}" STREQUAL "PICO")
    message("Building for RPI Pico...")
    include(pico/pico_sdk_import.cmake)
    pico_sdk_init()
else ()
    message(FATAL_ERROR "Invalid build type")
endif ()

project(i2cController)

add_executable(i2cController main.cpp
        i2cControllerBase.cpp
        i2cControllerBase.h
        utilities.cpp
        utilities.h
        ${I2CCONTROLLER_SRC})

if ("${BUILD_TARGET}" STREQUAL "I2CDRIVER")
    target_compile_definitions(i2cController PUBLIC USE_I2CDRIVER=1)
elseif ("${BUILD_TARGET}" STREQUAL "LINUX")
    target_compile_definitions(i2cController PUBLIC USE_LINUX=1)
    target_link_libraries(i2cController i2c)
    target_link_options(i2cController PRIVATE -static-libstdc++ -static-libgcc)
elseif ("${BUILD_TARGET}" STREQUAL "PICO")
    target_compile_definitions(i2cController PUBLIC USE_PICO=1)
    target_link_libraries(${PROJECT_NAME} PUBLIC pico_stdlib hardware_i2c)

    pico_enable_stdio_usb(${PROJECT_NAME} 1)
    pico_enable_stdio_uart(${PROJECT_NAME} 1)

    pico_add_extra_outputs(${PROJECT_NAME})

endif ()