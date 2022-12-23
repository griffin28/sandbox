# Rendering Sandbox

# Build
If QT5 is in a non-standard location you can export an environment variable with the non-standard path `export Qt5_DIR=<path/to/Qt5>` or create a CMake cache entry using the `-D` option as shown below. 

```bash
mkdir build && cd build
cmake [-DCMAKE_BUILD_TYPE=Debug] [-DQt5_DIR:PATH=<path/to/Qt5>] </path/to/sandbox>
cmake --build . [-j <threads>]
```

# Build Documentation

```bash
mkdir build && cd build
cmake [-DCMAKE_BUILD_TYPE=Debug] [-DQt5_DIR:PATH=<path/to/Qt5>] -DBUILD_DOCS:BOOL=ON </path/to/sandbox>
cmake --build . [-j <threads>]
cmake --build . -t doxygen
```

# Memcheck
To use Valgrind's memcheck tool to debug memory issues, execute the **valgrind** target as shown below. You will need to have valgrind installed on your system.

```bash
cd build
cmake --build . -t valgrind
```