# i2cController Library

## Description

This repository contains a project demonstrating a cross-platform class that abstracts the I2C interface. The library supports the following build targets:

| Build Target | Description                                     |
| ------------ | ----------------------------------------------- |
| I2CDRIVER    | Builds for the I2CDriver device.                |
| LINUX        | Builds for the Linux I2C `/dev/i2c-x`           |
| PICO         | Builds for Raspberry Pi Pico (Requires Pico SDK)|

## Prerequisites

Before building the library, ensure you have the following installed:

- CMake (version 3.10 or higher)
- A C++ compiler (GCC, Clang, etc.)
- Development headers for I2C (for Linux builds)
- Raspberry Pi Pico SDK (for Pico builds)

## Building with `cmake`

1. **Create a directory to build in.**
    ```sh
    mkdir build-directory
    ```
2. **Change into this new directory.**
    ```sh
    cd build-directory
    ```
3. **Execute `cmake` specifying the options needed for the build.**
    - `BUILD_TARGET`: The target platform to build for (e.g., LINUX, I2CDRIVER, PICO).
    - `CMAKE_BUILD_TYPE`: The build type (e.g., Debug, Release).
    ```sh
    cmake -DBUILD_TARGET=<TARGET> -DCMAKE_BUILD_TYPE=<TYPE> ..
    ```
4. **Run `make` to generate the executable.**
    ```sh
    make -j4
    ```

### Example Builds

#### Build for Linux Host

Requires I2C device development headers.

```sh
mkdir build-linux
cd build-linux
cmake -DBUILD_TARGET=LINUX -DCMAKE_BUILD_TYPE=Debug ..
make -j4
```

#### Build for I2CDriver Device

```sh
mkdir build-i2cdriver
cd build-i2cdriver
cmake -DBUILD_TARGET=I2CDRIVER -DCMAKE_BUILD_TYPE=Debug ..
make -j4
```

#### Build for Raspberry Pi Pico

Ensure you have the Pico SDK set up correctly.

```sh
mkdir build-pico
cd build-pico
cmake -DBUILD_TARGET=PICO -DCMAKE_BUILD_TYPE=Debug -DPICO_SDK_PATH=<path-to-pico-sdk> ..
make -j4
```

## Usage

After building the library, you can integrate it into your projects by including the relevant headers and linking against the compiled library.

## Contributing

Contributions are welcome! Please fork this repository, create a new branch, and submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or issues, please open an issue on GitHub or contact the maintainer.
