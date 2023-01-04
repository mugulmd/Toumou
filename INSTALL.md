# Install

> Note: for the moment, this project is available under Windows only.

## Required tools

- CMake >= 3.25
- C/C++ compiler with C++17 support

## Dependencies

- [spdlog >= 1.11.0](https://github.com/gabime/spdlog)
- [pybind11 >= 2.10.2](https://github.com/pybind/pybind11)
- [OpenEXR >= 3.1.5](https://github.com/AcademySoftwareFoundation/openexr)

## Windows

### Visual Studio

In a terminal:
```
cd Lava
mkdir build && cd build
cmake .. 
```

This will create a `Lava.sln` file, which you can open in Visual Studio to build the different targets of the project.

### CMake options

- `BUILD_LAVA_ENGINE`: enable/disable building the Lava engine
- `BUILD_EXAMPLE_APP`: enable/disable building the example application
- `BUILD_PYTHON_BINDINGS`: enable/disable building the Python bindings

## Test

If you built the example application, you can use it to test your installation: in a terminal, go to your installation directory and run `lava_example /path/to/image.exr` (replace `/path/to/image.exr` with the actual filepath you want to save the rendered image to).

Once this is done, open the saved image with the EXR viewer of your choice. You should get something like this:

![lava_example](docs/assets/lava_example.jpg)

> Note: to run the example application you might need to add the path to the OpenEXR, IMath and zlib DLLs in your `PATH` environment variable.
