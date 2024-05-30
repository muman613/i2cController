# Readme

## Description

This repository contains a project demonstrating a cross-platform class which abstracts the i2c interface. The library
currently supports the following build types:

| Build Type | Description                           |
|------------|---------------------------------------|
| I2CDRIVER  | Builds for the I2CDriver device.      |
| LINUX      | Builds for the Linux I2C `/dev/i2c-x` |
| PICO       | Build for Raspberry Pi Pico           |

## Building with `cmake`

1) Create a directory to build in.
2) Change into this new directory
3) Execute `cmake` specifying the options needed for the build.
    * `BUILD_TARGET`
    * `CMAKE_BUILD_TYPE`
