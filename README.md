# Rendering Sandbox

# Build

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
