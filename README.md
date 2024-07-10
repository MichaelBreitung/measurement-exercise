# Measurement Exercise

## Info

This is a simple C exercise to get familiar with memory management. The task was to implement the algorithm that is specified via [the requirements](./doc/requirements.md).

## Prerequisites

This project uses a Dev Container to provide the required tools and toolchain for building C programs. You must have VS Code and the Dev Containers extension installed on your host machine as well as the Docker Engine. On Windows, you can use Docker Desktop, for example. To avoid problems with the mounting of ssh keys, it is recommended, though, to use WSL2 with a Ubuntu distribution and install Docker there.

Here are three video tutorials that will get you started with Docker and Dev Containers:
- [Where Dev Containers are helpful](https://youtu.be/9F-jbT-pHkg?si=yW4RThXZNC0SMIyl)
- [How to create a custom Dev Container](https://youtu.be/7P0pTECkiN8?si=51YPKbUzL7OlAs80)
- [How to configure VS Code Extenstions and Settings in a Dev Container](https://youtu.be/W84R1CxtF0c?si=YBhBRzKk1lgCKEyz)

The project uses [CMake](https://cmake.org/) to compile the resources. For a simple project like this it might look like overkill. But this can serve as boiler plate for more complex projects which is why it's included as default option.

CMake, gcc, and further build essential tools are already part of the Docker Container. There is no need to install further tools.

## Building

1. Create a "build" sub-folder.
2. Navigate to the "build" sub-folder.
3. Execute ``cmake ..`` to initialize CMake - it will initialize the default debug configuration. If you want logging, use ``cmake .. -DLOGGING=1``.
4. Execute ``cmake --build .`` to compile the program.

The resulting library is called "libMeasurement" and is located in the "build" folder together with the CMake meta data. In addition to that, a "Measurement-Test" executable will be generated.

## Testing

[Google Test](https://google.github.io/googletest/) is used for Unit Testing. The required packages are installed via CMake. To run the tests:

1. Perform the build.
2. Run ``ctest`` inside the _build_ folder or execute ``./Measurement-Test``.

To check for memory leaks, Valgrind can be used. It is installed as part of the Dev Container. You can wrap Google Test with it by calling the following command inside the _build_ folder:

````
valgrind --leak-check=yes ./Measurement-Test
````

## Debugging

A VSCode launch configuration is included in the project. You can use it to debug the application. It is currently set up to use the "Measurement-Test" executable during launch.

**IMPORTANT**: Breakpoins will only work for the default, debug CMake target.

## Release

For a Release Build, you must reconfigure CMake first. The default build includes many logout messages, which will be omitted in an optimized build together with other performance optimizations applied by the compiler. For convenience, we provide a script for release creation:

````
./scripts/create_release.sh
````

